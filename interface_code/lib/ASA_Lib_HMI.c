// Last Build : 2017/08/15 11:55
// Author : Chu LiYu
// encode in utf-8

#include "ASA_Lib_HMI.h"
#include <avr\io.h>
#include <avr\interrupt.h>
#include <stdio.h>
#include <util\delay.h>
#include <string.h>
#include <inttypes.h>

// baud 38400 -> 1 bit trismit use 1/38400=2.604e-5 sec
// baud 38400 -> 1 byte trismit use 8/38400=2.08e-4 sec
// timer is 11059200UL
// 11059200/15 =         737280
// uint32_t = 2^32 = 4294967296

// register define
#define HMI_UCSRA UCSR0A
#define HMI_UDR UDR0

// pin number define
#define HMI_UDRE UDRE0
#define HMI_RXC RXC0

//
#define ONESEC_COUNT 921

// global variable
uint32_t timeout_count=0;
uint32_t Timeout=921UL*1000UL;
uint8_t TimeoutMode=1;

// function
void uart_put(uint8_t data);
uint8_t uart_get(void);
char HMI_timeout_set(int count);
char M128_HMI_set(char LSBytes, char Mask, char shift, char Data);
char M128_HMI_put(int Bytes, char Type, void *Data_p);
char M128_HMI_get(int Bytes, void *Data_p);
char M128_HMI_Form_put(char *FormatString, int Bytes, void *Data_p);
char M128_HMI_Form_get(char *FormatString, int Bytes, void *Data_p);

// function content
void uart_put( unsigned char data )
{
	while ( bit_is_clear(HMI_UCSRA, HMI_UDRE));
	HMI_UDR = data;
}

uint8_t uart_get( void )
{
	while ( bit_is_clear(HMI_UCSRA, HMI_RXC));
	return HMI_UDR;
}

char M128_HMI_set(char LSBytes, char Mask, char shift, char Data)
{
	if (LSBytes < 200 || LSBytes>200) {
		return 1;
	}
	switch (LSBytes) {
		case 200:
			if (Mask != 0xFF || shift != 0) {
				return 1;
			}
			Timeout = Data;
			break;
		case 201:
			if (Mask != 0x01 || shift != 0) {
				return 1;
			}
			TimeoutMode = Data;
			break;
	}
	return 0;
	// error code:
	//   0 - correct
	//   1 - wrong settings of LSBytes, Mask, shift
};

char HMI_timeout_set(int count)
{
	if (count < 50 || count > 30000) {
		return 1;
	}
	Timeout = ONESEC_COUNT * count;
	return 0;
}


char M128_HMI_put(int Bytes, char Type, void *Data_p)
{
    uint8_t CheckSum = 0;
    uint16_t i;

	uart_put( 0xAA );
	uart_put( 0xAA );
	uart_put( 0xAA );
	uart_put( Type );
    uart_put( Bytes>>8 );
	uart_put( Bytes&0xFF );

    CheckSum += Bytes>>8;
    CheckSum += Bytes&0xFF;

	for(i=0; i<Bytes; i++)
	{
		uart_put( ((char*)Data_p)[i] );
		CheckSum += ((char*)Data_p)[i];
	}
	uart_put( CheckSum );

	return 0;
}

char M128_HMI_get(int Bytes, void *Data_p)
{
	int i;
    int GetBytes;
    uint8_t GetBytesH;
    uint8_t GetBytesL;
    uint8_t CheckSum;
    uint8_t GetCheckSum;
    uint8_t GetType;

	if( uart_get() != 0xAB )
		return 2;
	if( uart_get() != 0xAB )
		return 2;
	if( uart_get() != 0xAB )
		return 2;

	GetType  = uart_get();
    GetBytesH = uart_get();
	GetBytesL = uart_get();
    GetBytes  = (GetBytesH<<8) | GetBytesL;

	CheckSum = GetBytesH + GetBytesL;
	for(i=0; i<Bytes; i++)
	{
		((char*)Data_p)[i] = uart_get();
		CheckSum += ((char*)Data_p)[i];
	}
	GetCheckSum = uart_get();

	if( GetBytes != Bytes )
		return 3;

	if( GetCheckSum != CheckSum )
		return 4;

	return 0;
}

char M128_HMI_Form_put(char *FormatString, int Bytes, void *Data_p)
{
	uint16_t i;
    uint16_t TotalBytes = 0;
    uint8_t FormatBytes = 0;
    uint8_t CheckSum = 0;

	uart_put( 0xBB );
	uart_put( 0xBB );
	uart_put( 0xBB );

    if( strlen(FormatString)>=255 )	// 資料結構字串長度過長
        return 1;
    FormatBytes = strlen(FormatString);

    TotalBytes = 1+FormatBytes+Bytes;
    uart_put( TotalBytes>>8 );
	uart_put( TotalBytes&0xFF );
	CheckSum += TotalBytes>>8;
    CheckSum += TotalBytes&0xFF;

	uart_put( FormatBytes );
	CheckSum += FormatBytes;

	for(i=0; i<FormatBytes; i++)
	{
		uart_put( ((char*)FormatString)[i] );
		CheckSum += ((char*)FormatString)[i];
	}

	for(i=0; i<Bytes; i++)
	{
		uart_put( ((char*)Data_p)[i] );
		CheckSum += ((char*)Data_p)[i];
	}
	uart_put( CheckSum );

	return 0;
}

char M128_HMI_Form_get(char *FormatString, int Bytes, void *Data_p)
{
	int i;
    int FormatBytes = 0;
    int GetTotalBytes;
    uint8_t GetTotalBytesH;
    uint8_t GetTotalBytesL;
    uint8_t GetFormatBytes;
    uint8_t GetBytes;
    uint8_t CheckSum;
    uint8_t GetCheckSum;

	if( uart_get() != 0xAB )
		return 2;
	if( uart_get() != 0xAB )
		return 2;
	if( uart_get() != 0xAB )
		return 2;
    GetTotalBytesH = uart_get();
    GetTotalBytesL = uart_get();
    GetTotalBytes  = GetTotalBytesH<<8 | GetTotalBytesL;
	CheckSum = GetTotalBytesH + GetTotalBytesL;

	GetFormatBytes = uart_get();
	CheckSum = GetFormatBytes;
	FormatBytes = strlen(FormatString);

	GetBytes = GetTotalBytes - GetFormatBytes;

	for(i=0; i<FormatBytes; i++)
	{
		((char*)FormatString)[i] = uart_get();
		CheckSum += ((char*)FormatString)[i];
	}

	for(i=0; i<Bytes; i++)
	{
		((char*)Data_p)[i] = uart_get();
		CheckSum += ((char*)Data_p)[i];
	}
	GetCheckSum = uart_get();

	if( GetBytes != Bytes )
		return 3;

	if( GetCheckSum != CheckSum )
		return 4;

	return 0;
}

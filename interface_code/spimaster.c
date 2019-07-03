#define F_CPU  11059200UL
#include "ASA_Lib.h"
#include<stdlib.h>


void set_spi()
{
	DDRB = (1<<PB5)|(1<<PB6)|(1<<PB7)|(1<<PB2)|(1<<PB1)|(1<<PB0); // asa address use PB 5:7 set output
	PORTB = (1<<PB5);// address寫入
	DDRF = 0x10; //chipselect用ASA訂的PF4設為輸出
	PORTF = 0<<4;//SS腳位設在PF4不在PB0
	SPCR |= (1<<MSTR)|(1<<SPE)|(1<<SPR0); //設定m128為主動端MSTR=1 SPE=1致能SPI功能.SPR0 set baud rate

}
int main()
{
	int data;
	ASA_M128_set();
	set_spi();


	while(1)
	{

		PORTF = 1<<4;      //標準版使用PF為chip select 接角
 		SPDR=0b10101010;   // SPDR寫入資料開始SPI通訊
		while(!(SPSR & (1<<SPIF))); //
		data=SPDR;     //read the data of the end of exchanged data
		PORTF = 0<<4; // close chipselect pin PF4 not m128 provide PB0
		_delay_ms(250);
		printf("%d\n",data);

	}


}

#define F_CPU  11059200UL
#include "ASA_Lib.h"
#include<stdlib.h>

volatile int data;

void set_spi()
{
	DDRB = (1<<PB5)|(1<<PB6)|(1<<PB7)|(1<<PB3)|(0<<PB0); // asa address use PB 5:7 set output
	PORTB = (1<<PB5);
	// PORTB =  (0<<PB5)&(0<<PB6);
	DDRF = 0x00;
	 PORTF = 1<<4;
	//SS腳位設在PF4不在PB0 DDR=0 PORT=0 |1 PUD(in SFIOR) X|1-> tri-state
	SPCR |= (1<<SPE)|(1<<SPR0)|(1<<SPIE);
  SPCR &= ~(1<<MSTR);
}
ISR(SPI_STC_vect)
{
	cli();
	// printf("100\n" );
	data=SPDR;
	SPDR=data+1;
	sei();
}

int main()
{
	ASA_M128_set();
	set_spi();
	sei();
	printf("123\n" );
	while(1)
	{
		// printf("%d\n",SPIF);
	}
}

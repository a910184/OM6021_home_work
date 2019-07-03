#define F_CPU  11059200UL
#include "ASA_Lib.h"
#include<stdlib.h>


void set_spi()
{
	DDRB = (1<<PB5)|(1<<PB6)|(1<<PB7)|(1<<PB2)|(1<<PB1)|(1<<PB0); // asa address use PB 5:7 set output
	PORTB = (1<<PB5);
	DDRF = 0x10;
	PORTF = 0<<4;//SS腳位設在PF4不在PB0
	SPCR |= (1<<MSTR)|(1<<SPE)|(1<<SPR0);
	_delay_ms(250);
	
}
int main()
{
	int data[2];
	double temp;
	ASA_M128_set();
	set_spi();

	while(1)
	{

		PORTF = 1<<4;
		SPDR=0;
		while(!(SPSR & (1<<SPIF)));
		data[0]=SPDR;
		SPDR=0;
		while(!(SPSR & (1<<SPIF)));
		data[1]=SPDR;
		PORTF = 0<<4;
		_delay_ms(250);
		temp = (double)(( (((int)data[0]) <<8) + ((int)data[1]) )*0.0078125);
		printf("%f\n",temp);


	}


}

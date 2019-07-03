#define F_CPU  11059200UL
#include "ASA_Lib.h"
#include <stdlib.h>


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


    PORTF = 1<<4;
    SPDR=0x0C;
    while(!(SPSR & (1<<SPIF)));
    SPDR=1;
    while(!(SPSR & (1<<SPIF)));
    PORTF = 0<<4;
    _delay_us(1);
    PORTF = 1<<4;
    SPDR=0x0F;
    while(!(SPSR & (1<<SPIF)));
    SPDR=0;
    while(!(SPSR & (1<<SPIF)));
    PORTF = 0<<4;
    _delay_us(1);
    PORTF = 1<<4;
    SPDR=0x0B;
    while(!(SPSR & (1<<SPIF)));
    SPDR=7;
    while(!(SPSR & (1<<SPIF)));
    PORTF = 0<<4;
    _delay_us(1);
    PORTF = 1<<4;
    SPDR=0x0A;
    while(!(SPSR & (1<<SPIF)));
    SPDR=0;
    while(!(SPSR & (1<<SPIF)));
    PORTF = 0<<4;
    _delay_us(1);
    PORTF = 1<<4;
    SPDR=0x0A;
    while(!(SPSR & (1<<SPIF)));
    SPDR=0;
    while(!(SPSR & (1<<SPIF)));
    PORTF = 0<<4;
    _delay_us(1);
    for (uint8_t i = 0; i < 8; i++) {
        PORTF = 1<<4;
        SPDR=i+1;
        while(!(SPSR & (1<<SPIF)));
        SPDR=0xAA;
        while(!(SPSR & (1<<SPIF)));
        PORTF = 0<<4;
        _delay_us(1);
    }



}

#include "ASA_lib.h"
#include <inttypes.h>

void adc_init()
{
    ADMUX  = (1<<REFS0);
    ADMUX |= 0x01;
    // ADC Enable and prescaler of 128
    // 11059200/128 = 86400
    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

// read adc value
uint16_t adc_read(uint8_t ch)
{
    // start single conversion
    // write '1' to ADSC
    ADCSRA |= (1<<ADSC);

    // wait for conversion to complete
    // ADSC becomes '0' again
    // till then, run loop continuously
    while(ADCSRA & (1<<ADSC));

    return (ADC);
}

int main() {

    ASA_M128_set();

    // initialize adc
    adc_init();
    lcd_init(LCD_DISP_ON_CURSOR);

    printf("srart---------!\n");

    uint16_t res;
    while(1) {
        res = adc_read();
        printf("res = %d\n", res);
    }
}

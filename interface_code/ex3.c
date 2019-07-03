#include "ASA_Lib.h"
#include <avr/interrupt.h>
#include "lib/SDC00/ASA_lib_SDC00.h"
#include "math.h"
#define HMI_TYPE_UI8  4


void adc_en(){
    ADMUX|= (1<<REFS1)|(1<<REFS0); //internal v ref 2.56
    ADMUX |= (0<<ADLAR);

    ADCSRA |= (1<<ADEN)|(0<<ADPS2)|(1<<ADPS1)|(0<<ADPS0);
}
// timer set sample rate adc write data to FIFO then FIFO write to SDCARD

int main() {
    ASA_M128_set();
    cli();
    adc_en();
    uint8_t yes,input;
    volatile uint16_t result0v,result1v;
    while (1) {
        printf("measurement?1or0:\n");
        scanf("%d",&yes );
        if(yes == 1) {
            result0v=0;
            result1v = 0;
            ADMUX |=(1<<MUX4)|(1<<MUX3)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0);
            ADCSRA|=(1<<ADSC);
            while(!(ADCSRA&(1<<ADIF))) {
                ;
            }
            result0v =ADC;
            printf("%d\n",result0v );
            ADMUX &=(0<<MUX0);
            ADMUX|= (1<<REFS1)|(1<<REFS0);     //internal v ref 2.56
            ADMUX |= (0<<ADLAR);
            ADMUX |=(1<<MUX4)|(1<<MUX3)|(1<<MUX2)|(1<<MUX1);
            ADCSRA|=(1<<ADSC);
            while(!(ADCSRA&(1<<ADIF))) {
                ;
            }

            result1v =ADC;
            printf("%d\n",result1v);

        }
        else if(yes == 0) {
            break;
        }
    }
    return 0;
}

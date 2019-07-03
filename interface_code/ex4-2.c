#include "ASA_Lib.h"
#include <avr/interrupt.h>
#include "math.h"

volatile uint16_t i=0;
void pwm_set(int channel,int prescaler,int invert){
    TCCR2 = 0;
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1C = 0;
    switch (channel) {
    case 1:
        cli();
        i=0;
        TIMSK = 0;
        ETIMSK = 0;
        TCCR1A =0;
        TCCR1B = 0;
        switch (prescaler) {
        case 1:
            TCCR1B|=(0<<CS12)|(0<<CS11)|(1<<CS10);
            break;
        case 2:
            TCCR1B|=(0<<CS12)|(1<<CS11)|(0<<CS10);

            break;
        case 3:
            TCCR1B|=(0<<CS12)|(1<<CS11)|(1<<CS10);

            break;
        case 4:
            TCCR1B|=(1<<CS12)|(0<<CS11)|(0<<CS10);

            break;
        case 5:
            TCCR1B|=(1<<CS12)|(0<<CS11)|(1<<CS10);
            break;
        default:
            printf("default\n" );
            break;
        }
        switch (invert) {
        case 1:
            TCCR1A|=(1<<COM1C1)|(1<<COM1C0);
            break;
        case 0:
            TCCR1A |= (1<<COM1C1)|(0<<COM1C0);
            break;
        default:
            printf("default\n" );
            break;

        }
        TCCR1A|=(1<<WGM10);//Fast PWM 8bit
        TCCR1B|=(1<<WGM12);//Fast PWM 8bit
        DDRB|=(1<<PB7);
        ETIMSK|=(1<<OCIE1C);
        printf("1A%d\n",TCCR1A );
        printf("1B%d\n",TCCR1B );
        printf("1C%d\n",TCCR1C );
        printf("%d\n",TIMSK );
        printf("%d\n",ETIMSK );
        sei();

        break;
    case 2:
        cli();
        i=0;
        TIMSK = 0;
        ETIMSK = 0;
        TCCR2 = 0;
        switch (prescaler) {
        case 1:
            TCCR2|=(0<<CS22)|(0<<CS21)|(1<<CS20);
            printf("here\n" );
            break;
        case 2:
            TCCR2|=(0<<CS22)|(1<<CS21)|(0<<CS20);

            break;
        case 3:
            TCCR2|=(0<<CS22)|(1<<CS21)|(1<<CS20);

            break;
        case 4:
            TCCR2|=(1<<CS22)|(0<<CS21)|(0<<CS20);

            break;
        case 5:
            TCCR2|=(1<<CS22)|(0<<CS21)|(1<<CS20);

            break;
        default:
            printf("default\n" );
            break;

        }
        switch (invert) {
        case 1:
            TCCR2|=(1<<COM21)|(1<<COM20);
            break;
        case 0:
            TCCR2 |= (1<<COM21)|(0<<COM20);
            break;
        default:
            printf("default\n" );
            break;
        }
        TCCR2 |= (1<<WGM21)|(1<<WGM20); //Fast PWM mode　
        DDRB|=(1<<PB7);
        TIMSK|= (1<<OCIE2);
        printf("%d\n",TCCR2 );
        printf("%d\n",TIMSK );
        printf("%d\n",ETIMSK );
        sei();
        break;
    default:
        printf("default\n" );
        break;

    }
}
volatile uint8_t data[100] = {127,134,142,150,158,166,173,181,188,195,201,207,213,219,224,229,234,238,241,245,247,250,251,
                              252,253,254,253,252,251,250,247,245,241,238,234,229,224,219,213,207,201,195,188,181,173,166,158,150,142,134,127,119,
                              111,103,95,87,80,72,65,58,52,46,40,34,29,24,19,15,12,8,6,3,2,1,0,0,0,1,2,3,6,8,12,15,19,24,29,34,40,46,52,58,65,
                              72,80,87,95,103,111,119};
volatile uint8_t data1[100] = {127,134,142,150,158,166,173,181,188,195,201,207,213,219,224,229,234,238,241,245,247,250,251,
                               252,253,254,253,252,251,250,247,245,241,238,234,229,224,219,213,207,201,195,188,181,173,166,158,150,142,134,127,119,
                               111,103,95,87,80,72,65,58,52,46,40,34,29,24,19,15,12,8,6,3,2,1,0,0,0,1,2,3,6,8,12,15,19,24,29,34,40,46,52,58,65,
                               72,80,87,95,103,111,119};
ISR(TIMER1_COMPC_vect){
    // OCR1C = 128;
    // DDRA = 1;
    // PORTA =1;
    OCR1C =data[i];
    i++;
    if(i>=100  ) {
        i=0;
    }
}

ISR(TIMER2_COMP_vect){
    OCR2 =data1[i];
    i++;
    if(i>=100 ) {
        i=0;
    }
}
int main() {
    ASA_M128_set();
    int channel;
    int prescaler;
    int invert;
    while (1) {
        PORTA = 0;
        printf("PWM1or2\n" );
        scanf("%d",&channel );
        printf("prescaler1 1,8 2,64 3,256 4,1024 5 \n" );
        scanf("%d",&prescaler );
        printf("invert 1or 0\n" );
        scanf("%d",&invert);
        pwm_set(channel,prescaler,invert);
    }
    return 0;
}

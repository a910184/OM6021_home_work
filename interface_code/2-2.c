#include "lib/ASA_LIB.h"
#include "lib/DAC00/ASA_Lib_DAC00.h"
#include <avr/interrupt.h>
#include <avr/iom128.h>
#include <math.h>
/*timer interrupt*/
#define HMI_TYPE_F32  8
int j = 0;
char ID_DAC = 1;
volatile uint16_t u[100] = {0};
volatile uint16_t y[100] = {0};
ISR(TIMER0_COMP_vect){
    ASA_DAC00_set(ID_DAC, 200, 0x30, 4, 0x00); // 輸出通道1 S1S2

    ASA_DAC00_put(ID_DAC, 0, 2, &u[j]); //DAC卡輸出波型
    ASA_DAC00_set(ID_DAC, 200, 0x30, 4, 0x01); // 輸出通道1 S1S2

    ASA_DAC00_put(ID_DAC, 0, 2, &y[j]); //DAC卡輸出波型
    j=j+2;

    if(j>99  ) {
        j = 0;
    }
}
void timerset(int frequency){

    TCCR0 |= (1<<WGM01)|(0<<CS02)|(1<<CS01)|(0<<CS00);
    OCR0  = 173;
    TIMSK |= (1<<OCIE0);//8k
    sei();
}
int main() {
    ASA_M128_set();
    for(int i = 0; i<100; i++) {
        u[i] = (uint16_t)2047*sin(0.02*3.14159*i)+2047;
    }
    for(int i = 0; i<100; i++) {
        y[i] = (uint16_t)2047*cos(0.02*3.14159*i)+2047;
    }
    ASA_DAC00_set(ID_DAC, 200, 0x80, 7, 0x01);
    int start,end;
    while(1) {
        
        printf("start 1\n" );
        scanf("%d",&start);
        int a;
        if(start==1) {
            timerset(16);
            printf("break 1\n" );
            while(1) {
                scanf("%d",&a );
                if(a==1) {
                    cli();
                    break;
                }
            }
        }
    }
    return 0;
}

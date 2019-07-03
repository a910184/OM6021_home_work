
#include "E:/avr-gcc-7.2.0-x64-mingw/avr/include/avr/iom128.h"
#include "lib/ASA_Lib.h"
#include "lib/DAC00/ASA_Lib_DAC00.h"
#include <avr/interrupt.h>
#include "math.h"
/*timer interrupt*/
#define HMI_TYPE_F32  8
    int j = 0;
char ID_DAC = 1;
volatile uint16_t u[100] = {0};
volatile uint16_t datasin;
ISR(TIMER0_COMP_vect){
    ASA_DAC00_set(ID_DAC, 200, 0x30, 4, 0x00); // 輸出通道1 S1S2

    ASA_DAC00_put(ID_DAC, 0, 2, &u[j]); //DAC卡輸出波型
    j++;
    if(j%99 == 0 ) {
        j = 0;
    }
}
void timerset(int frequency){
    TCCR0 |= (1<<WGM01)|(0<<CS02)|(1<<CS01)|(0<<CS00);
    OCR0  = 173;
    TIMSK |= (1<<OCIE0);//8k
}
int main() {
    ASA_M128_set();
    for(int i = 0; i<100; i++) {
        u[i] = (uint16_t)2047*sin(0.02*3.14159*i)+2047;
    }
    ASA_DAC00_set(ID_DAC, 200, 0x80, 7, 0x01); // 單通道非同步模式
    int start;
    while(1) {

        printf("start 1\n" );
        scanf("%d",&start);
        int a;
        if(start==1) {
          sei();
            timerset(16);
            printf("break 1\n" );
            while(1) {
                scanf("%d",&a );
                if(a==1) {
                    TIMSK = 0;
                    cli();
                    break;
                }
            }
        }
    }
    return 0;
}

/**
 * @file exp2_dac00.c
 * @author LiYu
 * @date 2018.03.10
 * @brief Example of using ADC00 to output an assigned volt.
 *
 */

#include "lib/ASA_Lib.h"
#include "lib/DAC00/ASA_Lib_DAC00.h"
#include <avr/interrupt.h>
#include <math.h>
/*timer interrupt*/
#define HMI_TYPE_F32  8
int j =0;
char ID_DAC = 1;
volatile float u[100]={0};
volatile uint16_t datasin;
ISR(TIMER0_COMP_vect){
    ASA_DAC00_set(ID_DAC, 200, 0x30, 4, 0x00); // 輸出通道1 S1S2
    datasin=(uint16_t)2047*u[j]+2047;
    ASA_DAC00_put(ID_DAC, 0, 2, &datasin); //DAC卡輸出波型
    j++;
    if(j%99 == 0 ) {
        j = 0;
    }
}
int main() {
    ASA_M128_set();

    char d_bytes = sizeof(float);
    for(int i =0; i<100; i++) {
        M128_HMI_get(d_bytes, &u[i]);  // send f32  array data
    }
    for(int i = 0; i<100; i++) {
        M128_HMI_put(d_bytes, HMI_TYPE_F32, &u[i]);    // send f32  array data

    }
    TCCR0 |= (1<<WGM01)|(0<<CS02)|(1<<CS01)|(0<<CS00);
    OCR0  = 173;
    TIMSK|=(1<<OCIE0);//8k

    int datasin = 20;
    int datacos = 20;

    ASA_DAC00_set(ID_DAC, 200, 0x80, 7, 0x01); // 單通道非同步模式


    while(1) {
        ;
    }
    return 0;
}

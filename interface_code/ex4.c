#include "ASA_Lib.h"
#include <string.h>
#include "stdlib.h"
int main() {
  ASA_M128_set();
  // PWM set
  TCCR0 = (1<<COM01|1<< COM00 |1<<WGM00|1<<WGM01|0<<CS02|1<<CS01|1<<CS00);
  TIMSK = (1<<OCIE0|1<< TOIE0);
  TIFR &= ~(0x01 << OCF0 );
int freq = 180;
  OCR0 = freq;
  DDRB = 1<<PB4;

}

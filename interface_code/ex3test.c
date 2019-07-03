#include "ASA_Lib.h"
#include <avr/interrupt.h>
#include "lib/SDC00/ASA_lib_SDC00.h"
#include "math.h"
#define sizeFIFO (256)
typedef struct {
    volatile uint8_t tail;
    volatile uint8_t head;
    volatile uint8_t data[256];
} FIFO;
static volatile FIFO adcdata;
void FIFO_ini(volatile FIFO* str){
    str->tail = 0;
    str->head = 0;
}

void adc_en(){
    ADMUX |= (1<<ADLAR);
    ADMUX |= 1<<MUX0;
    ADCSRA |= (1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(0<<ADPS1)|(0<<ADPS0);
}
// timer set sample rate adc write data to FIFO then FIFO write to SDCARD
void timer0_set(){
    TCCR0 =0b00001010;
    OCR0= 173;
    TIMSK|=1<<OCIE0;
}


int FIFO_push(volatile FIFO*  str,uint8_t data){
    cli();
    uint8_t head_next = str->head+1;
    if(head_next>= sizeFIFO) {
        head_next = 0;
    }
    if(head_next == str->tail) {
        sei();
        return -1;
    }
    str->data[str->head] = data;
    str->head = head_next;
    sei();
    return 0;
}
int FIFO_pop(volatile FIFO* str,uint8_t* data){
    cli();
    if (str->tail == str->head) {
        sei();
        return -1;
    }
    uint8_t tail_next = str->tail+1;
    if(tail_next >= sizeFIFO) {
        tail_next = 0;
    }
    *data = str->data[str->tail];
    str->tail = tail_next;
    sei();
    return 0;
}
ISR(TIMER0_COMP_vect){
    ADCSRA|=(1<<ADSC);

}
uint8_t debug;
ISR(ADC_vect){
// FIFO_push(&adcdata,ADCH);
    debug=ADCH;
}

int main() {
    ASA_M128_set();
    adc_en();
    timer0_set();
// FIFO_ini(&adcdata);
    sei();
// uint8_t value;
// unsigned char ASA_ID = 4, Mask = 0xFF, Shift = 0, Setting = 0xFF;
//
// // 宣告 與SDC00交換資料的資料陣列緩衝區
//
// char check = 0; // 模組通訊狀態旗號
//
// // 以覆寫模式建立檔案，檔案名為 "testa.txt”
// // *** 開啟檔案 ***/
// printf("start\n" );
//      // 設定檔案名稱
//      ASA_SDC00_put(ASA_ID, 64, 8, "ADC");
//      // 設定檔案副檔名
//      ASA_SDC00_put(ASA_ID, 72, 3, "bin");
//
//      // 使用覆寫模式開檔，送出0x05至LSbyte 200
//      Setting = 0x05;
//      // 寫入旗標組合
//      check = ASA_SDC00_set(ASA_ID, 200, Mask, Shift, Setting);
//      if (check != 0) {  // 檢查回傳值做後續處理
//          // 開檔失敗，回傳錯誤資訊(錯誤碼請查閱規格書)
//          printf("Debug point 1, error code <%d>\n", check);
//          return 1;
//      }
//      /*** 開啟檔案完成 ***/
    uint16_t count=0;
    while(1) {
        // while(FIFO_pop(&adcdata, &value)){
        printf("%d\n",debug);
        // ASA_SDC00_put(ASA_ID,0,1,&value);

        // }
        // count++;
    }
// /*** 關閉檔案 ***/
// check = ASA_SDC00_set(ASA_ID, 200, Mask, Shift, 0x00);
// if (check != 0) {  // 檢查回傳值做後續處理
//     printf("Debug point 2, error code <%d>\n", check);
//     return 2;
// }
// /*** 關閉檔案完成 ***/

    printf("end\n" );
//  return 0;
}

#include "ASA_Lib.h"
#include <avr/interrupt.h>
#include "lib/SDC00/ASA_lib_SDC00.h"
#include "math.h"
#define HMI_TYPE_F32  8
#define HMI_TYPE_I8  0

#define sizeFIFO (512)
typedef struct {
    volatile uint16_t tail;
    volatile uint16_t head;
    volatile int8_t data[512];
} FIFO;
static volatile FIFO adcdata;
void FIFO_ini(volatile FIFO* str){
    str->tail = 0;
    str->head = 0;
}

void adc_en(){
    ADMUX |= (1<<REFS1)|(1<<REFS0);
    ADMUX |= (1<<ADLAR);              //internal v ref 2.56
    ADMUX |= (1<<MUX3)|(1<<MUX1)|(1<<MUX0);
    ADCSRA |= (1<<ADEN)|(0<<ADPS2)|(1<<ADPS1)|(0<<ADPS0);
}
// timer set sample rate adc write data to FIFO then FIFO write to SDCARD
void timer0_set(){
    TCCR0 |= (1<<WGM01)|(0<<CS02)|(1<<CS01)|(0<<CS00);
    OCR0= 173;
    TIMSK|=1<<OCIE0;
}


int fifo_pop( uint8_t *data){

    if(adcdata.tail == adcdata.head) {

        return -1;
    }
    else{
        *data = adcdata.data[adcdata.tail];
        adcdata.tail++;
        if(adcdata.tail == sizeFIFO) {
            adcdata.tail = 0;
        }

        return 0;
    }

}
int fifo_push(volatile uint8_t data){
    if((adcdata.head+1)%sizeFIFO == adcdata.tail) {
        return -1;
    }
    else{
        adcdata.data[adcdata.head]=data;
        adcdata.head++;
        if(adcdata.head == sizeFIFO) {
            adcdata.head = 0;
        }
        return 0;
    }
}


ISR(TIMER0_COMP_vect){
    cli();
    ADCSRA|=(1<<ADSC);
    while(!(ADCSRA&(1<<ADIF))) {
        ;
    }
    fifo_push(ADCH);
    ADCSRA|= (1<<ADIF);
    sei();

}

int main() {
    ASA_M128_set();
    adc_en();
    timer0_set();
    FIFO_ini(&adcdata);
    cli();
    uint16_t count=0,count_max=0;
    printf("count 0-1500\n" );
    scanf("%d",&count );
    unsigned char ASA_ID = 4, Mask = 0xFF, Shift = 0, Setting = 0xFF;

// 宣告 與SDC00交換資料的資料陣列緩衝區

    char check = 0;             // 模組通訊狀態旗號

// 以覆寫模式建立檔案，檔案名為 "testa.txt”
// *** 開啟檔案 ***/
    // printf("start\n" );
    // 設定檔案名稱
    ASA_SDC00_put(ASA_ID, 64, 8, "ADC");
    // 設定檔案副檔名
    ASA_SDC00_put(ASA_ID, 72, 3, "bin");

    // 使用覆寫模式開檔，送出0x05至LSbyte 200
    Setting = 0x05;
    // 寫入旗標組合
    check = ASA_SDC00_set(ASA_ID, 200, Mask, Shift, Setting);
    if (check != 0) {             // 檢查回傳值做後續處理
        // 開檔失敗，回傳錯誤資訊(錯誤碼請查閱規格書)
        printf("Debug point 1, error code <%d>\n", check);
        return 1;
    }
    /*** 開啟檔案完成 ***/
    int8_t value;
    int8_t re;
    sei();
    while(count_max<count) {

        re = fifo_pop(&value);
        if(re==-1) {

        }else{

            ASA_SDC00_put(ASA_ID,0,1,&value);
            // rawdata[count] = value;
            count_max++;
        }
    }
/*** 關閉檔案 ***/
    check = ASA_SDC00_set(ASA_ID, 200, Mask, Shift, 0x00);
    if (check != 0) {             // 檢查回傳值做後續處理
        printf("Debug point 2, error code <%d>\n", check);
        return 2;
    }
/*** 關閉檔案完成 ***/
    cli();
/*** 開啟檔案 ***/
// 設定檔案名稱
    char d_bytes = sizeof(uint8_t);
    ASA_SDC00_put(ASA_ID, 64, 8, "ADC");
// 設定檔案副檔名
    ASA_SDC00_put(ASA_ID, 72, 3, "bin");

// 使用覆寫模式開檔，送出0x05至LSbyte 200
    Setting = 0x01;
// 寫入旗標組合
    check = ASA_SDC00_set(ASA_ID, 200, Mask, Shift, Setting);
    if (check != 0) { // 檢查回傳值做後續處理
        // 開檔失敗，回傳錯誤資訊(錯誤碼請查閱規格書)
        printf("Debug point 1, error code <%d>\n", check);
        return;
    }
// Read file parameter (File last modify date)
    int rec = 0;
    int8_t put_buffer;

    memset(put_buffer, 0, sizeof(put_buffer));
    printf("start\n" );
    for (int i = 0; i<count; i++) {
        rec = ASA_SDC00_get(ASA_ID, 0, 1,&put_buffer); // 檢查get函式回傳值做後續處理
        M128_HMI_put(1, HMI_TYPE_I8, &put_buffer);  // send f32  array data
        if (rec) { // 當遇到錯誤或是已讀取至檔案尾巴進入此
            break;
        } // 顯示讀取結果
    }

/*** 關閉檔案完成 ***/
/*** 關閉檔案 ***/
    check = ASA_SDC00_set(ASA_ID, 200, Mask, Shift, 0x00);
    if (check != 0) { // 檢查回傳值做後續處理
        printf("Debug point 2, error code <%d>\n", check);
        return;
    }
    // printf("fuck full count%d head%d end\n",count,adcdata.head);
//  return 0;
    sei();

    return 0;
}

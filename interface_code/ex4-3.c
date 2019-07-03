#include "ASA_Lib.h"
#include <avr/interrupt.h>
#include "string.h"
#include "math.h"

void pwm_set(){
    TCCR1A|=(1<<COM1A1)|(1<<COM1A0)|(1<<WGM10);//set compare mode noninverting
    // TCCR1A|=(1<<COM1B1)|(1<<COM1B0);//set compare mode noninverting
    // TCCR1A|=(1<<COM1C1)|(1<<COM1C0);//set compare mode noninverting

    TCCR1B|=(1<<WGM12)|(0<<CS12)|(0<<CS11)|(1<<CS10);
    TCCR1C|=(1<<FOC1A)|(1<<FOC1B)|(1<<FOC1C);
    OCR1A = 0x0000;
    // OCR1B = 0x0040;
    // OCR1C = 0x0020;
    DDRB|=(1<<PB5)|(1<<PB6)|(1<<PB7);
    TIMSK|=(1<<OCIE1A)|(0<<OCIE1B);
    ETIMSK|=(0<<OCIE1C);

}
volatile uint8_t data[1000];
volatile uint16_t i = 0;
ISR(TIMER1_COMPA_vect){
    OCR1A = data[i*5];
    i++;
    if(i%55 == 0) {
        i=0;
    }
}
// 初始化資料陣列緩衝區記憶體
int main() {
    ASA_M128_set();
    uint8_t swap_buffer[64];
    memset(swap_buffer, 0, sizeof(swap_buffer));
    unsigned char ASA_ID = 4, Mask = 0xFF, Shift = 0, Setting = 0xFF;
    char check = 0; // 模組通訊狀態旗號

/*** 開啟檔案 ***/
// 設定檔案名稱
    ASA_SDC00_put(ASA_ID, 64, 8, "101");
// 設定檔案副檔名
    ASA_SDC00_put(ASA_ID, 72, 3, "BIN");

// 使用覆寫模式開檔，送出0x05至LSbyte 200
    Setting = 0x01;
// 寫入旗標組合
    check = ASA_SDC00_set(ASA_ID, 200, Mask, Shift, Setting);
    if (check != 0) { // 檢查回傳值做後續處理
        // 開檔失敗，回傳錯誤資訊(錯誤碼請查閱規格書)
        printf("Debug point 1, error code <%d>\n", check);
        return -1;
    }

// Read file parameter (File last modify date)
    printf("[Start] Start reading file data!!\n");
    int rec = 0;
    for (int i = 0; i<15; i++) {
        /*** Read data from SDC00 ****/
        memset(swap_buffer, 0, sizeof(swap_buffer));
        rec = ASA_SDC00_get(ASA_ID, 0, 64, swap_buffer);// 檢查get函式回傳值做後續處理
        for(int j=0; j<64; j++ ) {
            data[i*64+j]= swap_buffer[j];
        }
        if (rec) {// 當遇到錯誤或是已讀取至檔案尾巴進入此
            printf("error or EOF");
            break;
        }                                                                                      // 顯示讀取結果

    }
    printf("[Finish] Reading file data \n");
/*** 關閉檔案完成 ***/
/*** 關閉檔案 ***/
    check = ASA_SDC00_set(ASA_ID, 200, Mask, Shift, 0x00);
    if (check != 0) { // 檢查回傳值做後續處理
        printf("Debug point 2, error code <%d>\n", check);
        return -1;
    }
    /*** 關閉檔案完成 ***/
    pwm_set();
    printf("play");
    sei();
    while(1) {
        ;
    }
    return 0;
}

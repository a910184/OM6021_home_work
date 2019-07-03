 #include "lib/ASA_LIB.h"
 #include "lib/DAC00/ASA_Lib_DAC00.h"
 #include "lib/SDC00/ASA_lib_SDC00.h"
 #include <avr/interrupt.h>
 #include "math.h"
/*timer interrupt*/
 #define HMI_TYPE_F32  8
int j =0;
char ID_DAC = 1;
volatile float u[100]={0};
volatile uint16_t datasin;
ISR(TIMER0_COMP_vect){
    ASA_DAC00_set(ID_DAC, 200, 0x30, 4, 0x00);  // 輸出通道1 S1S2
    datasin=(uint8_t)2047*u[j]+2047;
    ASA_DAC00_put(ID_DAC, 0, 2, &datasin);  //DAC卡輸出波型
    j++;
    if(j%99 == 0 ) {
        j = 0;
    }
}
int main() {
    ASA_M128_set();
    // /* 建立set函數所需之參數變數 */
  unsigned char ASA_ID = 4, Mask = 0xFF, Shift = 0, Setting = 0xFF;
    char d_bytes = sizeof(float);
    for(int i =0; i<100; i++) {
        M128_HMI_get(d_bytes, &u[i]); // send f32  array data
    }
    for(int i = 0; i<100; i++) {
        M128_HMI_put(d_bytes, HMI_TYPE_F32, &u[i]); // send f32  array data

    }
  // for(int i; i<100; i++) {
//     u[i] = sin(i*2*3.14/100);
  // }
    char check = 0; // 模組通訊狀態旗號
    printf("----start-----\n" );
    // 以覆寫模式建立檔案，檔案名為 "testa.txt”
    // *** 開啟檔案 ***/
    // 設定檔案名稱
    ASA_SDC00_put(ASA_ID, 64, 8, "sin");
    // 設定檔案副檔名
    ASA_SDC00_put(ASA_ID, 72, 3, "txt");

    // 使用覆寫模式開檔，送出0x05至LSbyte 200
    Setting = 0x05;
    // 寫入旗標組合
    check = ASA_SDC00_set(ASA_ID, 200, Mask, Shift, Setting);
    if (check != 0) {     // 檢查回傳值做後續處理
        // 開檔失敗，回傳錯誤資訊(錯誤碼請查閱規格書)
        printf("Debug point 1, error code <%d>\n", check);
        return;
    }
    // printf("here\n" );
    /*** 開啟檔案完成 ***/
    TCCR0 |= (1<<WGM01)|(0<<CS02)|(1<<CS01)|(0<<CS00);
    OCR0  = 173;
    TIMSK|=(1<<OCIE0);   //8k



    char ID_DAC = 1;

    ASA_DAC00_set(ID_DAC, 200, 0x80, 7, 0x01); // 單通道非同步模式
    for(int i= 0; i<100; i++) {
        cli();
        ASA_SDC00_put(ASA_ID, 0, 4, &u[i]);
        sei();
    }
    /*** 關閉檔案 ***/
    cli();
    check = ASA_SDC00_set(ASA_ID, 200, Mask, Shift, 0x00);
    if (check != 0) {  // 檢查回傳值做後續處理
        printf("Debug point 2, error code <%d>\n", check);
        return;
    }
    /*** 關閉檔案完成 ***/
    /*** 開啟檔案 ***/
    // 設定檔案名稱
    ASA_SDC00_put(ASA_ID, 64, 8, "sin");
    // 設定檔案副檔名
    ASA_SDC00_put(ASA_ID, 72, 3, "txt");

    // 使用覆寫模式開檔，送出0x05至LSbyte 200
    Setting = 0x01;
    // 寫入旗標組合
    check = ASA_SDC00_set(ASA_ID, 200, Mask, Shift, Setting);
    if (check != 0) {  // 檢查回傳值做後續處理
        // 開檔失敗，回傳錯誤資訊(錯誤碼請查閱規格書)
        printf("Debug point 1, error code <%d>\n", check);
        return;
    }
    sei();
    // Read file parameter (File last modify date)
    int rec = 0;
    float put_buffer[100];

    memset(put_buffer, 0, sizeof(put_buffer));
    for (int i = 0; i<100; i++) {
        cli();
        rec = ASA_SDC00_get(ASA_ID, 0, 4,&put_buffer[i]);   // 檢查get函式回傳值做後續處理
        if (rec) {    // 當遇到錯誤或是已讀取至檔案尾巴進入此
            break;
        }    // 顯示讀取結果
        sei();
    }
    for (int i = 0; i<100; i++) {
        M128_HMI_put(d_bytes, HMI_TYPE_F32, &put_buffer[i]);     // send f32  array data
    }

    cli();
    /*** 關閉檔案完成 ***/
    /*** 關閉檔案 ***/
    check = ASA_SDC00_set(ASA_ID, 200, Mask, Shift, 0x00);
    if (check != 0) {  // 檢查回傳值做後續處理
        printf("Debug point 2, error code <%d>\n", check);
        return;
    }
    sei();
    while(1) {
        ;
    }
    return 0;
}

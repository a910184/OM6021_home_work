#include "lib/ASA_Lib.h"
#include "lib/SDC00/ASA_lib_SDC00.h"
#include "lib/DAC00/ASA_Lib_DAC00.h"
#include <avr/interrupt.h>
#include "lib/ASA_Lib_HMI.h"
/*timer interrupt*/
#define HMI_TYPE_F32  8
#define COVERWRITE  0x07
#define REWRITE 0x05
#define READFILE  0x01
int j = 0;
char ID_DAC = 1;
unsigned char ASA_ID = 4, Mask = 0xFF, Shift = 0;

volatile float u[100] = {0};
volatile uint16_t datasin;
ISR(TIMER0_COMP_vect){
    ASA_DAC00_set(ID_DAC, 200, 0x30, 4, 0x00); // 輸出通道1 S1S2
    datasin = (uint16_t)2045*u[j]+2047;
    ASA_DAC00_put(ID_DAC, 0, 2, &datasin); //DAC卡輸出波型
    j++;
    if(j%99 == 0 ) {
        j = 0;
    }
}
void SDCclose(){
    /*** 關閉檔案 ***/
    int check,a;
    check = ASA_SDC00_set(ASA_ID, 200, Mask, Shift, 0x00);
    if (check != 0) { // 檢查回傳值做後續處理
        printf("Debug point 2, error code <%d>\n", check);
        scanf("%d",&a );
        return;
    }
    /*** 關閉檔案完成 ***/
}
void SDCopen(int Setting){
    ASA_SDC00_put(ASA_ID, 64, 8, "sin");
    // 設定檔案副檔名
    ASA_SDC00_put(ASA_ID, 72, 3, "txt");
    int check,a;
    // 使用覆寫模式開檔，送出0x05至LSbyte 200

    // 寫入旗標組合
    check = ASA_SDC00_set(ASA_ID, 200, Mask, Shift, Setting);
    if (check != 0) {  // 檢查回傳值做後續處理
        // 開檔失敗，回傳錯誤資訊(錯誤碼請查閱規格書)
        printf("Debug point 1, error code <%d>\n", check);
        scanf("%d",&a );
        return;
    }
}
int main() {
    ASA_M128_set();
    // /* 建立set函數所需之參數變數 */
    unsigned char ASA_ID = 4, Mask = 0xFF, Shift = 0, Setting = 0xFF;
    char d_bytes = sizeof(float);
    float temp[100];
    printf("send 100 float recods\n" );
    scanf("%s" );
    M128_HMI_get(100*d_bytes, temp); // send f32  array data
//     printf("check the data you sent\n" );
//     scanf("%s");
//     M128_HMI_put(100*d_bytes, HMI_TYPE_F32, temp); // send f32  array data
// // for(int i; i<100; i++) {
//     //     temp[i] = sin(i*2*3.14/100);
//     // }
    char check = 0; // 模組通訊狀態旗號

    SDCopen(REWRITE);

    char ID_DAC = 1;

    ASA_DAC00_set(ID_DAC, 200, 0x80, 7, 0x01); // 單通道非同步模式
    for(int i = 0; i<100; i++) {
        cli();
        ASA_SDC00_put(ASA_ID, 0, 4, &temp[i]);
        sei();
    }
    SDCclose();
    SDCopen(READFILE);
    // Read file parameter (File last modify date)
    int rec = 0;
    TCCR0 |= (1<<WGM01)|(0<<CS02)|(1<<CS01)|(0<<CS00);
    OCR0  = 173;
    TIMSK |= (1<<OCIE0);  //8k
    sei();
    for (int i = 0; i<100; i++) {
        cli();
        rec = ASA_SDC00_get(ASA_ID, 0, 4,&u[i]);  // 檢查get函式回傳值做後續處理
        if (rec) {   // 當遇到錯誤或是已讀取至檔案尾巴進入此
          i--;
          break;
        }
        sei();   // 顯示讀取結果
    }
    cli();
    SDCclose();
    sei();
    uint8_t a;
    while(1) {
          // printf("break 1");
          // scanf("%s" );
          // M128_HMI_get(sizeof(a), a); // send f32  array data
          // if(a==1){
          //   cli();
          //   break;
          // }
          ;
    }
    return 0;
}

#include "lib/ASA_Lib.h"
#include "lib/SDC00/ASA_lib_SDC00.h"
#include "lib/DAC00/ASA_Lib_DAC00.h"
#include <avr/interrupt.h>
#include "math.h"
#include "lib/ASA_Lib_HMI16.h"
/*timer interrupt*/
#define HMI_TYPE_F32  8
#define COVERWRITE  0x07
#define REWRITE 0x05
#define READFILE  0x01
volatile int j = 0;
unsigned char ASA_ID = 4, Mask = 0xFF, Shift = 0;
char d_bytes = sizeof(uint16_t);

char ID_DAC = 1;

typedef struct {
    volatile uint16_t tail;
    volatile uint16_t head;
    volatile uint16_t data[512];
} FIFO;
#define sizeFIFO (512)

static volatile FIFO dacdata;

void FIFO_ini(volatile FIFO* str){
    dacdata.tail = 0;
    dacdata.head = 0;
}
int fifo_pop( uint16_t *data){

    if(dacdata.tail == dacdata.head) {

        return -1;
    }
    else{
        *data = dacdata.data[dacdata.tail];
        dacdata.tail++;
        if(dacdata.tail == sizeFIFO) {
            dacdata.tail = 0;
        }

        return 0;
    }

}
int fifo_push(volatile uint16_t data){
    if((dacdata.head+1)%sizeFIFO == dacdata.tail) {
        return -1;
    }
    else{
        dacdata.data[dacdata.head] = data;
        dacdata.head++;
        if(dacdata.head == sizeFIFO) {
            dacdata.head = 0;
        }
        return 0;
    }
}


volatile uint16_t u[100] = {0};
volatile uint16_t y[100] = {0};
ISR(TIMER0_COMP_vect){
    int re;
    ASA_DAC00_set(ID_DAC, 200, 0x30, 4, 0x00); // 輸出通道1 S1S2
    ASA_DAC00_put(ID_DAC, 0, 2, &u[j]); //DAC卡輸出波型
    re = fifo_push(u[j]);

    ASA_DAC00_set(ID_DAC, 200, 0x30, 4, 0x01); // 輸出通道1 S1S2
    ASA_DAC00_put(ID_DAC, 0, 2, &y[j]); //DAC卡輸出波型
    fifo_push(y[j]);
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
    uint8_t datanumber;
    float temp[100];
    uint8_t size = sizeof(float);
    uint16_t Bytes = size*100;
    printf("Send float 100 records to m128\n");
    scanf("%s" );
    M128_HMI_get( Bytes,temp);
    // 以覆寫模式建立檔案，檔案名為 "testa.txt”
    // *** 開啟檔案 ***/
    // 設定檔案名稱
    SDCopen(REWRITE);
    // printf("here\n" );
    /*** 開啟檔案完成 ***/
    ASA_DAC00_set(ID_DAC, 200, 0x80, 7, 0x01); // 單通道非同步模式
    TCCR0 |= (1<<WGM01)|(0<<CS02)|(1<<CS01)|(0<<CS00);
    OCR0  = 173;
    TIMSK |= (1<<OCIE0); //8k
    uint8_t start;
    while(1) {

        printf("start answer char number 1\n" );
        scanf("%s");
        M128_HMI_get(1, &start);
        int a = 0;
        if(start==1) {
            sei();
            while( a<200 ) {
                int re;
                int value;
                re = fifo_pop(&value);
                if(re==-1) {
                    //    printf("eat\n" );
                }else{
                    cli();
                    ASA_SDC00_put(ASA_ID,0,2,&value);
                    // rawdata[count] = value;
                    a++;
                    sei();
                }
            }
            cli();
            SDCclose();
            SDCopen(READFILE);
// Read file parameter (File last modify date)
            int rec = 0;
            uint16_t put_buffer[200];

            memset(put_buffer, 0, sizeof(put_buffer));
            for (int i = 0; i<200; i++) {
                rec = ASA_SDC00_get(ASA_ID, 0, 2,&put_buffer[i]); // 檢查get函式回傳值做後續處理
                if (rec) { // 當遇到錯誤或是已讀取至檔案尾巴進入此
                    break;
                } // 顯示讀取結果
            }
            uint16_t bytes = 400;
            printf("M128 send play 200*2 bytes of uint16 data  \n" );
            scanf("%s" );
            M128_HMI_put(bytes,HMI_TYPE_UI16, put_buffer);     // send f32  array data

            SDCclose();
        }

    }
    return 0;
}

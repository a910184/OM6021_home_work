/**
 * @file exp2_sdc_3.c
 * @author Cheng-Wei
 * @date 2018.03.05
 * @brief Example of open file with read only mode,
 * Using Register table to fetch data and resolve.
 *
 */

#include "ASA_Lib.h"
#include "lib\SDC00\ASA_lib_SDC00.h"
#include <string.h>

int main(void) {

    ASA_M128_set();
    printf("Start --------!");

    /* 建立set函數所需之參數變數 */
    unsigned char ASA_ID = 1, Mask = 0xFF, Shift = 0, Setting = 0xFF;

    // 宣告 與SDC00交換資料的資料陣列緩衝區
    uint8_t swap_buffer[64];
    // 初始化資料陣列緩衝區記憶體
    memset(swap_buffer, 0, sizeof(swap_buffer));

    char check = 0; // 模組通訊狀態旗號

    // 以讀取模式開啟檔案，既有之檔案名為 "testa.txt”

    /*** 開啟檔案 ***/
    // 設定檔案名稱
    ASA_SDC00_put(ASA_ID, 64, 8, "testa");
    // 設定檔案副檔名
    ASA_SDC00_put(ASA_ID, 72, 3, "txt");

    // 使用讀取模式開檔，送出0x01至LSbyte 200
    Setting = 0x01;
    // 寫入旗標組合
    check = ASA_SDC00_set(ASA_ID, 200, Mask, Shift, Setting);
    if (check != 0) {  // 檢查回傳值做後續處理
        // 開檔失敗，回傳錯誤資訊(錯誤碼請查閱規格書)
        printf("Debug point 1, error code <%d>\n", check);
        return 0;
    }
    /*** 開啟檔案完成 ***/

    // Read file parameter (File last modify date)
    unsigned char date_buffer[2];
    unsigned char time_buffer[2];
    unsigned char sz_buffer[4];

    // Read file parameter (File last modify date)
    check = ASA_SDC00_get(ASA_ID, 78, 2, date_buffer);

    // Years: 1980 + (bit 15:9)
    // Month: (bit 8:5)
    // Day:   (bit 4:0)
    uint16_t date_rawdata = date_buffer[0];
    date_rawdata += date_buffer[1] << 8;
    printf("<OUT> combine Date %0x %0x   -> %0x\n",
    date_buffer[1],
    date_buffer[0],
    date_rawdata);
    printf("<OUT> Date %u/%u/%u\n",
    1980 + (uint16_t)(date_rawdata >> 9),
    (uint16_t)(date_rawdata >> 5) & 0x0F,
    (uint16_t)(date_rawdata) & 0x1f);

    // Read file parameter (File last modify time)
    check = ASA_SDC00_get(ASA_ID, 76, 2, time_buffer);
    // Hour: (bit 15:11)
    // Min:  (bit 10:5)
    // Sec:  (bit 4:0)/2
    uint16_t time_rawdata = time_buffer[0];  time_rawdata += time_buffer[1] << 8;
    printf("<OUT> combine Time %0x%0x   -> %0x\n",
    time_buffer[1],
    time_buffer[0],
    time_rawdata);
    printf("<OUT>Time %u:%u:%u\n",
    time_rawdata >> 11,
    (time_rawdata >> 5) & 0x3f,
    (time_rawdata & 0x1f) / 2);
    // Show the raw data from Register "78" & "76"
    printf("<Rec Raw data>fdate = %0x, ftime = %0x\n", date_rawdata, time_rawdata);

    // Read file parameter (File size)
    check = ASA_SDC00_get(ASA_ID, 82, 4, sz_buffer);

    for(int i=0; i<4; i++) {
        printf("<OUT> sz_buffer[%d]: %d\n",i,  (int)(sz_buffer[i]));
    }
    // File size(byte) : (bit 31:0)
    printf("<OUT> Sz: %ld\n",
    ((long int)(sz_buffer[0])) +
    ((long int)(sz_buffer[1]) << 8) +
    ((long int)(sz_buffer[2]) << 16) +
    ((long int)(sz_buffer[3]) << 24));

    /*** 關閉檔案 ***/
    check = ASA_SDC00_set(ASA_ID, 200, Mask, Shift, 0x00);
    if (check != 0) {  // 檢查回傳值做後續處理
        // 關檔失敗，回傳錯誤資訊(錯誤碼請查閱規格書)
        printf("Debug point 2, error code <%d>\n", check);
        return 0;
    }
    /*** 關閉檔案完成 ***/

    return 0;
}

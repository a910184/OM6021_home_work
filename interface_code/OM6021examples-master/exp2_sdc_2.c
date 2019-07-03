/**
 * @file exp2_sdc_2.c
 * @author Cheng-Wei
 * @date 2018.03.05
 * @brief Example of open file with append mode, and write data
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

    // 以續寫模式開啟檔案，既有之檔案名為 "testa.txt”

    /*** 開啟檔案 ***/
    // 設定檔案名稱
    ASA_SDC00_put(ASA_ID, 64, 8, "testa");
    // 設定檔案副檔名
    ASA_SDC00_put(ASA_ID, 72, 3, "txt");

    // 使用續寫模式開檔，送出0x07至LSbyte 200
    Setting = 0x07;
    // 寫入旗標組合
    check = ASA_SDC00_set(ASA_ID, 200, Mask, Shift, Setting);
    if (check != 0) {  // 檢查回傳值做後續處理
        // 開檔失敗，回傳錯誤資訊(錯誤碼請查閱規格書)
        printf("Debug point 1, error code <%d>\n", check);
        return 0;
    }
    /*** 開啟檔案完成 ***/


    printf("[Start] Write data to file with append mode !!\n");
    for (int i = 1; i<50; i++) {
        // 寫入旗標組合
        sprintf(swap_buffer, "There is a string from M128 %d\r\n", i);

        // 計算swap_buffer[]中的字串大小
        int sizeof_string = 64;
        for (int i = 0; i<sizeof(swap_buffer); i++) {
            if (swap_buffer[i] == '\0') {
                sizeof_string = i;
                break;
            }
        }

        // 寫入資料至指定檔案
        ASA_SDC00_put(ASA_ID, 0, sizeof_string, swap_buffer);
    }
    printf("[Finish] Write data to file with append mode!!\n");

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

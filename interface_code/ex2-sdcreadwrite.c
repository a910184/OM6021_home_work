/**
 * @file exp2_sdc_1.c
 * @author Cheng-Wei
 * @date 2018.03.05
 * @brief Example of open file with overlap mode, and write data
 *
 */
 #include "ASA_Lib.h"
 #include "lib/SDC00/ASA_lib_SDC00.h"
 #include "math.h"
 void main(void) {

     ASA_M128_set();

     /* 建立set函數所需之參數變數 */
     unsigned char ASA_ID = 4, Mask = 0xFF, Shift = 0, Setting = 0xFF;

     // 宣告 與SDC00交換資料的資料陣列緩衝區
     uint8_t swap_buffer[64];
     // 初始化資料陣列緩衝區記憶體
     memset(swap_buffer, 0, sizeof(swap_buffer));

     char check = 0; // 模組通訊狀態旗號

     // 以覆寫模式建立檔案，檔案名為 "testa.txt”
    // *** 開啟檔案 ***/
          // 設定檔案名稱
          ASA_SDC00_put(ASA_ID, 64, 8, "testa");
          // 設定檔案副檔名
          ASA_SDC00_put(ASA_ID, 72, 3, "txt");

          // 使用覆寫模式開檔，送出0x05至LSbyte 200
          Setting = 0x05;
          // 寫入旗標組合
          check = ASA_SDC00_set(ASA_ID, 200, Mask, Shift, Setting);
          if (check != 0) {  // 檢查回傳值做後續處理
              // 開檔失敗，回傳錯誤資訊(錯誤碼請查閱規格書)
              printf("Debug point 1, error code <%d>\n", check);
              return;
          }
          /*** 開啟檔案完成 ***/


          printf("[Start] Write data to file with overlap(or new file) mode !!\n");
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
          printf("[Finish] Write data to file with overlap mode!!\n");

          /*** 關閉檔案 ***/
          check = ASA_SDC00_set(ASA_ID, 200, Mask, Shift, 0x00);
          if (check != 0) {  // 檢查回傳值做後續處理
              printf("Debug point 2, error code <%d>\n", check);
              return;
          }
          /*** 關閉檔案完成 ***/

     /*** 開啟檔案 ***/
     // 設定檔案名稱
     ASA_SDC00_put(ASA_ID, 64, 8, "testa");
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

     // Read file parameter (File last modify date)
     printf("[Start] Start reading file data!!\n");
     int rec = 0;
     for (int i = 0; i<8000; i++) {
         /*** Read data from SDC00 ****/
           memset(swap_buffer, 0, sizeof(swap_buffer));
         rec = ASA_SDC00_get(ASA_ID, 0, 64, swap_buffer);// 檢查get函式回傳值做後續處理
           if (rec) {// 當遇到錯誤或是已讀取至檔案尾巴進入此
           for (int i = 0; i<64; i++) {printf("%c", swap_buffer[i]);}
           printf("\nReading finish! Get %ld bytes %d \n", (long)i * 64 + swap_buffer[63], rec);break;}// 顯示讀取結果
       for (int i = 0; i<64; i++) {
         printf("%c", swap_buffer[i]);
      }
    }
   printf("[Finish] Reading file data \n");
     /*** 關閉檔案完成 ***/
     /*** 關閉檔案 ***/
     check = ASA_SDC00_set(ASA_ID, 200, Mask, Shift, 0x00);
     if (check != 0) {  // 檢查回傳值做後續處理
         printf("Debug point 2, error code <%d>\n", check);
         return;
     }
     /*** 關閉檔案完成 ***/

     return;
 }

/**
 * @file exp2_dac00.c
 * @author LiYu
 * @date 2018.03.10
 * @brief Example of using ADC00 to output an assigned volt.
 *
 */

#include "ASA_Lib.h"
#include "lib/DAC00/ASA_Lib_DAC00.h"

int main() {
    ASA_M128_set();
    printf("ASA_DAC example code. by LiYu 2017.12.21\n");

    char ID_DAC = 1;
    int data = 20;

	ASA_DAC00_set(ID_DAC, 200, 0x80, 7, 0x01); // 單通道非同步模式
	ASA_DAC00_set(ID_DAC, 200, 0x30, 4, 0x00); // 輸出通道1 S1S2

    while (1) {
        printf("input data:");
        scanf("%d", &data);

        ASA_DAC00_put(ID_DAC, 0, 2, &data); //DAC卡輸出波型
    }

    return 0;
}

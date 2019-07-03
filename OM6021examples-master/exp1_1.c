/**
 * @file exp1_1.c
 * @author LiYu
 * @date 2017.11.26
 * @brief Example of sending array data by using HMI function.
 *
 * Sending array data by using HMI function From M128. Can catch
 * the array data by use ASA_HMI_Data_Agent or Matlab. Matlab
 * example is "exp1_1.m" in "matlab" folder.
 */

#include "ASA_Lib.h"
#include <string.h>

#define HMI_TYPE_I8   0
#define HMI_TYPE_I16  1
#define HMI_TYPE_I32  2
#define HMI_TYPE_I64  3
#define HMI_TYPE_UI8  4
#define HMI_TYPE_UI16 5
#define HMI_TYPE_UI32 6
#define HMI_TYPE_UI64 7
#define HMI_TYPE_F32  8
#define HMI_TYPE_F64  9

int main() {
    ASA_M128_set();
    char s[20];

    float f32_data[5] = { 1.5, -1, 0, 1, -2.5 };
    char f32_bytes = 5*sizeof(float); // float is 4 bytes => Bytes = 20
    // NOTE float and double both are 4 bytes (32 bits)
    // This is the only supported floating point format in AVR-GCC

    uint16_t ui16_data[5] = { 1, 2, 3, 4, 65535 };
    int32_t  i32_data[8]  = { 0, 1, 2, 3, 4, 5, 6, 7};
    char  ui16_bytes = 5*sizeof(uint16_t);
    char  i32_bytes  = 8*sizeof(int32_t);

    printf("M128 Start -------------!\n");
    printf("Plz say something to M128\n");
    scanf("%s", s);

    M128_HMI_put(f32_bytes , HMI_TYPE_F32 , f32_data ); // send f32  array data
    M128_HMI_put(ui16_bytes, HMI_TYPE_UI16, ui16_data); // send ui16 array data
    M128_HMI_put(i32_bytes , HMI_TYPE_I32 , i32_data ); // send i32  array data

    printf("M128 is ready to receive data\n");

    M128_HMI_get(f32_bytes, f32_data); // get
    f32_data[1] +=1;
    M128_HMI_put(f32_bytes , HMI_TYPE_F32 , f32_data ); // send f32 array data

    return 0;
}

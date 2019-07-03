#include "LIB/ASA_Lib.h"
#include "LIB/ASA_HMI.h"
#include <string.h>

int main() {
    ASA_M128_set();

    char num = 50;
    char Bytes = num*sizeof(float); // float is 4 bytes => Bytes = 200
    float Data[num];
    // float Data_re[num];

    for (uint8_t i = 0; i < num; i++) {
        Data[i] = i+0.1;
    }
    M128_HMI_put(Bytes, HMI_TYPE_F32, Data); // send

    return 0;
}

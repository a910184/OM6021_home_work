#include "LIB/ASA_Lib.h"
#include "LIB/ASA_HMI.h"
#include <string.h>

int main() {
    ASA_M128_set();

    float Data[5]={1.1,-1,0,1,-2.1};
    float Data_re[5];
    char num = 5;
    char Bytes = num*sizeof(float); // float is 4 bytes => Bytes = 20
    // NOTE float and double both are 4 bytes (32 bits)
    // This is the only supported floating point format in AVR-GCC

    M128_HMI_put(Bytes, HMI_TYPE_F32, Data); // send

    M128_HMI_get(Bytes, Data_re); // get
    Data_re[1] +=1;
    M128_HMI_put(Bytes, HMI_TYPE_F32,Data_re); // re send

    return 0;
}

// m_strncmp(const char* s1, const char* s2,)

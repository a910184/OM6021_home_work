#ifndef __ASA_HMI_H__
#define __ASA_HMI_H__

char M128_HMI_set(char LSBytes, char Mask, char shift, char Data);
char M128_HMI_put(int Bytes, char Type, void *Data_p);
char M128_HMI_get(int Bytes, void *Data_p);
char M128_HMI_Form_put(char *FormatString, int Bytes, void *Data_p);
char M128_HMI_Form_get(char *FormatString, int Bytes, void *Data_p);

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

#endif

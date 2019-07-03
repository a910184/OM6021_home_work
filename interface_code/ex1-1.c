#include ".\lib\ASA_LIB.h"
#include <string.h>
#include "stdlib.h"
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
typedef struct{
  float a[2][2];
  float b[2][1];
  float c[1][2];
  float d[1][1];
  float x[2][1];
} struct_trans;
int main() {
    ASA_M128_set();
    char s;
    char a_bytes = 4*sizeof(float); // float is 4 bytes => Bytes = 20
    char b_bytes = 2*sizeof(float); // float is 4 bytes => Bytes = 20
    char c_bytes = 2*sizeof(float); // float is 4 bytes => Bytes = 20
    char d_bytes = sizeof(float); // float is 4 bytes => Bytes = 20
    //  NOTE float and double both are 4 bytes (32 bits)
    // This is the only supported floating point format in AVR-GCC
    struct_trans data;
    data.a[0][0] = 1.35;
     data.a[0][1] =  0.55;
     data.a[1][0] = -0.45;
     data.a[1][1] = 0.35 ;
     data.b[0][0] =  0.5;
     data.b[0][1] =  0.5;
     data.c[0][0] = 3.0;
     data.c[0][1] =  1.0 ;
     data.d[0][0] =  1.0 ;
     data.x[0][0]=0;
     data.x[1][0]=0;
     printf("M128 Start -------------!\n");
     printf("Plz say something to M128\n");
     scanf("%s", s);
     int byte_trans = sizeof(struct_trans);
     int byte_state = sizeof(struct_trans);
     M128_HMI_Form_put("f32x4,f32x2,f32x1,f32x2,f32x2", byte_trans, &data);

     printf("M128 is ready to receive data\n");

    float u[100]={0} ;
    for(int i = 1;i<99;i++){
      u[0]=0;
      u[i]=1;
    }
    float y[100]={0} ;
    M128_HMI_Form_get("f32x4,f32x2,f32x1,f32x2,f32x2", byte_trans, &data);

   printf("updated state\n" );
  float Y=0,X=0,X1=0;
      for (int i = 0; i < 100; i++) {
        X=data.a[0][0]*data.x[0][0]+data.a[0][1]*data.x[1][0]+data.b[0][0]*u[i];
        X1=data.a[1][0]*data.x[0][0]+data.a[1][1]*data.x[1][0]+data.b[0][1]*u[i];
        Y = data.c[0][0]*data.x[0][0]+data.c[0][1]*data.x[1][0]+data.d[0][0];
        data.x[0][0]= X;
        data.x[1][0] = X1;
        y[i]= Y;
        M128_HMI_put(d_bytes , HMI_TYPE_F32 , &X );
        M128_HMI_put(d_bytes , HMI_TYPE_F32 , &X1 );
        M128_HMI_put(d_bytes , HMI_TYPE_F32 , &Y );
      }
    return 0;
}

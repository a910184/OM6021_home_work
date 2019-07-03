#include "ASA_Lib.h"
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

int main() {
    ASA_M128_set();
    char s[20];
    float y,x=0,x1=0;
    float a[4] = { 1.35,0.55,-0.45,0.35 };
    float b[2] = { 0.5, 0.5 };
    float c[2] = { 3.0, 1.0 };
    float d[1] = { 1.0 };
     char a_bytes = 4*sizeof(float); // float is 4 bytes => Bytes = 20
     char b_bytes = 2*sizeof(float); // float is 4 bytes => Bytes = 20
     char c_bytes = 2*sizeof(float); // float is 4 bytes => Bytes = 20
     char d_bytes = sizeof(float); // float is 4 bytes => Bytes = 20
    //  NOTE float and double both are 4 bytes (32 bits)
    // This is the only supported floating point format in AVR-GCC
    float u[100] ;

    printf("M128 Start -------------!\n");
    printf("Plz say something to M128\n");
    scanf("%s", s);

    M128_HMI_put(a_bytes , HMI_TYPE_F32 , a ); // send f32  array data
    M128_HMI_put(b_bytes , HMI_TYPE_F32 , b ); // send f32  array data
    M128_HMI_put(c_bytes , HMI_TYPE_F32 , c ); // send f32  array data
    M128_HMI_put(d_bytes , HMI_TYPE_F32 , d ); // send f32  array data

   // send f32  array data
   printf("M128 is ready to receive data\n");
   M128_HMI_get(a_bytes  , a ); // send f32  array data
   M128_HMI_get(b_bytes  , b ); // send f32  array data
   M128_HMI_get(c_bytes  , c ); // send f32  array data
   M128_HMI_get(d_bytes  , d );
   for(int i =0;i<100;i++){
   M128_HMI_get(d_bytes  , &u[i]);    // send f32  array data
 }
   M128_HMI_put(a_bytes , HMI_TYPE_F32 , a ); // send f32  array data
   M128_HMI_put(b_bytes , HMI_TYPE_F32 , b ); // send f32  array data
   M128_HMI_put(c_bytes , HMI_TYPE_F32 , c ); // send f32  array data
   M128_HMI_put( d_bytes, HMI_TYPE_F32 , d ); // send f32  array data
   for(int i =0;i<100;i++){
   M128_HMI_put(d_bytes , HMI_TYPE_F32 , &u[i]);    // send f32  array data
  }
  //  printf("updated state\n" );
  float Y=0,X=0,X1=0;


      for (int i = 0; i < 100; i++) {
        Y = c[0]*x+c[1]*x1+d[0];
        X=a[0]*x+a[1]*x1+b[0]*u[i];
        X1=a[2]*x+a[3]*x1+b[1]*u[i];
        x=X;
        x1=X1;
        M128_HMI_put(d_bytes , HMI_TYPE_F32 , &X );
        M128_HMI_put(d_bytes , HMI_TYPE_F32 , &X1 );
        M128_HMI_put(d_bytes , HMI_TYPE_F32 , &Y);
      }

         // send f32  array data

    return 0;
}

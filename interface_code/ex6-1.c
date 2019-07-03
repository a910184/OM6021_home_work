/* interface code for ex6 twi api
 * Eznob.Yen 2018.4.17
 */
 #include "ASA_Lib.h"
 #include <inttypes.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <avr/io.h>
 #include <util/twi.h>
 #define W 0
 #define R 1
typedef enum prescaler {one,four,sixteen,sixtyfour} prescaler_e;
typedef enum condition {START,CONTINUE,STOP,RCONTINUE} condition_e;
typedef struct buffer {volatile uint8_t *data; uint8_t slave_add; uint8_t WR; uint8_t bytes;} buffer_t;
buffer_t buf;
condition_e cond;

prescaler_e pre;
volatile int err;
void twi_baudrate_set (uint8_t br,uint16_t prescaler){
    TWBR = br;
    if(prescaler == 1) {
        pre = one;
        printf("PRE->1\n");
    }
    else if(prescaler == 4) {
        pre = four;
        printf("PRE->4\n");
    }
    else if(prescaler==16) {
        pre = sixteen;
        printf("PRE->16\n");
    }
    else if(prescaler == 64) {
        pre = sixtyfour;
        printf("PRE->64\n");
    }
    switch (pre) {
    case one:
        TWSR &= (0<<TWPS1)&(0<<TWPS0);
        printf("PRE->1\n");
        break;
    case four:
        TWSR |= (1<<TWPS0);
        printf("PRE->4\n");
        break;
    case sixteen:
        TWSR |= (1<<TWPS1);
        printf("PRE->16\n");
        break;
    case sixtyfour:
        TWSR |= (1<<TWPS1)|(1<<TWPS0);
        printf("PRE->64\n");
        break;
    }
}
void twi_handle (uint8_t condition){

    cond = condition;
    switch (cond) {
    case START:
        TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
        printf("START-\n");
        break;
    case CONTINUE:
        TWCR = (1<<TWINT)|(1<<TWEN);
        printf("CONTINUE-\n");
        break;
    case STOP:
        TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
        printf("STOP-\n");
        break;
    case RCONTINUE:
        TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
        printf("RCONTINUE-\n");
        break;
    }
}
void twi_ini (uint8_t *data,uint8_t bytes,uint8_t slave_add){
    buf.data = data;
    buf.bytes = bytes;
    buf.WR = W;
    buf.slave_add = slave_add;
    DDRB = (1<<PB5)|(1<<PB6)|(1<<PB7);
    PORTB = (1<<PB5);
    TWCR |= (1<<TWIE);
    twi_handle(START);
}
int twi_read(){

    while(err !=1) {
    }
    printf("err1:%d\n",err );;
    twi_handle(CONTINUE);
    buf.WR = R;
    twi_handle(START);
    while(err!=4 ) {
        printf("err2:%d\n",err );
    }

}
int twi_write(){
    while(err==1) {
        twi_handle(CONTINUE);
    }
    while(err != 2) {
        ;
    }

}
ISR (TWI_vect){
    switch((TWSR)&(0xf8)) {
    case TW_START:
    case TW_REP_START:
        TWDR = (buf.slave_add<<1)|(buf.WR);
        twi_handle(CONTINUE);
        err = 0;
        break;
    case TW_MT_SLA_ACK:
        TWDR = *buf.data;
        err = 1;
        break;
    case TW_MT_DATA_ACK:
        for(int i = 1; i<buf.bytes; i++) {
            TWDR = *(buf.data+i);
            twi_handle(CONTINUE);
        }
        twi_handle(STOP);
        err = 2;
        break;
    case TW_MR_SLA_ACK:
        twi_handle(CONTINUE);
        err = 3;
        break;
    case TW_MR_DATA_ACK:
        for(int i = 0; i<buf.bytes; i++) {
            buf.data[i] = TWDR;
            twi_handle(CONTINUE);
        }
        twi_handle(STOP);
        err = 4;
        break;
    case TW_MT_SLA_NACK:
    case TW_MT_ARB_LOST:
    case TW_MT_DATA_NACK:
    case TW_MR_SLA_NACK:
    case TW_MR_DATA_NACK:
        err = 5;
        break;
    }
}
int main() {
    ASA_M128_set();
    printf("----S-----\n");
    twi_baudrate_set(4,4);

    //while(1) {
    uint8_t data = 0x75;
    uint8_t slave_add = 0b01101000;
    twi_ini(&data,1,slave_add);
    twi_read();
    printf("%x\n",*buf.data );
    // while (!(TWCR&(1<<TWINT)));
    // printf("1:%x\n", (TWSR&0xf8));
    // TWDR = (buf.slave_add<<1)|(buf.WR);
    // printf("ADD+W:%d\n", TWDR);
    // twi_handle(CONTINUE);
    // while (!(TWCR&(1<<TWINT)));
    // _delay_ms(20);
    //
    // printf("2:%x\n", (TWSR&0xf8));
    // TWDR = *buf.data;
    // printf("%x\n",*buf.data );
    // twi_handle(CONTINUE);
    // while (!(TWCR&(1<<TWINT)));
    // printf("3:%x\n", (TWSR&0xf8));
    //
    // _delay_ms(20);
    // uint8_t res;
    // twi_parameter(&res, 1, R,slave_add);
    // twi_handle(START);
    // while (!(TWCR&(1<<TWINT)));
    //
    // _delay_ms(20);
    // printf("4:%x\n",(TWSR&0xf8));
    // TWDR = (buf.slave_add<<1)|(buf.WR);
    // printf("ADD+R:%d\n", TWDR);
    // _delay_ms(20);
    // twi_handle(CONTINUE);
    // while (!(TWCR&(1<<TWINT)));
    // printf("5:%x\n",(TWSR&0xf8));
    // //*buf.data = TWDR;
    // //printf("%x\n", TWDR );
    // _delay_ms(20);
    // twi_handle(CONTINUE);
    // while (!(TWCR&(1<<TWINT)));
    // printf("6:%x\n",(TWSR&0xf8));
    // *buf.data = TWDR;
    // printf("%x\n",TWDR );
    //
    // twi_handle(STOP);

    // printf("result%x",res);
    return 0;
}

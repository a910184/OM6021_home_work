#ifndef _TWI_H
#define _TWI_H
/**twi master trasitor***/
#define START 0x08
#define RESTART 0x10
#define SLAW_A 0x18
#define SLAW_N 0x20
#define DATA_A 0x28
#define DATA_N 0x30
#define LOST 0x38
#define TWSRMSK 0xf8
typedef struct {
    uint8_t ASAID;
    uint8_t ADDRESS;
    uint8_t BAUD;
    uint8_t *DATA;
} TWI_STRUCT ;
//twi API
uint8_t TwiTX(TWI_STRUCT TXdata);
uint8_t TwiRX(uint8_t *RXdata);
/****twi master reciever***/
//basic action 
void TwiIni(uint8_t ASAID,uint8_t Baud);
int TwiStart(void);
int MSend(int data);
void MRecieveAck(void);
int MRecieve(int* data);
int TwiStop();
uint8_t ProtocolStateCheck(uint8_t sate);
uint8_t MasterResend(uint8_t data);

#endif

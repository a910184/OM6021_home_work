#define F_CPU  11059200UL
#include "lib/ASA_Lib.h"
#include<stdlib.h>
#include "twi.h"
main()
{
ASA_M128_set();
    uint8_t slave_addr = 0b01001111;
    int SLAR = (slave_addr<<1) | 0x01;
    int SLAW = (slave_addr<<1)&0xfe;
    printf("SLAR %x\nSLAW %x\n",SLAR,SLAW );
    int tmppoint = 0x01;
    int tmpreso = (1<<6|1<<5);
    int data[2];
    double temp;
    int twsr;
    uint8_t ASAID  = 1,Baud = 4;

    twiini(ASAID,Baud);
    while(1)
    {
/**************************************************/
        twsr = twistart();
        printf("start %x\n",twsr);

        twsr = msend(SLAW);
        printf("address %x\n",twsr);
        tmppoint = 0x01;
        twsr = msend(tmppoint);
        printf("pointer %x\n",twsr);

        twsr = msend(tmpreso);
        printf("resolution %x\n",twsr);

        twistop();
/********************************************/
        twsr = twistart();
        printf("start %x\n",twsr);
        tmppoint = 0;
        twsr = msend(SLAW);
        printf("SLAW %x\n",twsr);

        twsr = msend(tmppoint);
        printf("pointer %x\n",twsr);
        twistop();
        /************************************************/
        twsr = twistart();
        printf("start %x\n",twsr);

        twsr = msend(SLAR);
        printf("address %x\n",twsr);

        mrecieveack();
        twsr = mrecieve(&data[0]);
        //printf("data %x\n",twsr);
        printf("memory %x\n",&data[0] );
        twsr = mrecieve(&data[1]);
        // printf("data %x\n",twsr);
        printf("memory%x\n",data[1] );
        twistop();
        temp = (data[0]<<4 | data[1]>>4);
        printf("Temperature: %f\n",temp*0.0625);
        _delay_ms(100);
    }

}

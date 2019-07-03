// F_CPU is used by utils/delay.h for _delay_ms function calcutation delay ticks
#define F_CPU 11059200UL

// Main header for ASA M128 Dev board
#include "ASA_Lib.h"

#include "twi_comm.h"

#include <avr/pgmspace.h>

#define TWI_STATUS_MASK	0xF8

#define MPU9255_SLA		0b01101000

char ASA_ID_set(unsigned char ASA_ID);

int32_t total_value = 0;
uint16_t count = 0;
void measure(int16_t sample) {
	printf("Sample : %d\n", sample);
	total_value += sample*4;
	count++;

}




void main() {
	
	// Initialize the ASA M128 Library
	ASA_M128_set();

	ASA_ID_set(1);

	TwiComm_init();

	StrTWICommPack packet;

	uint8_t rx_data[128];
	uint8_t tx_data[128];

	//tx_data[0] = 0x3B;
	//TwiComm_master_write_byte(&packet, MPU9255_SLA, tx_data);
	//_delay_ms(10);
//
	//TwiComm_master_read_bytes(&packet, MPU9255_SLA, rx_data, 1);
	//_delay_ms(10);
//
	//printf("Get Data Reg[%02X] =  %02X\n",
	  //tx_data[0],  rx_data[0]);
	//
	//while(1);
	typedef struct {
		uint8_t acc_x_h;
		uint8_t acc_x_l;
		uint8_t acc_y_h;
		uint8_t acc_y_l;
		uint8_t acc_z_h;
		uint8_t acc_z_l;
	} StrIMU_Acc_packet;

	typedef struct {
		uint8_t gryo_x_h;
		uint8_t gryo_x_l;
		uint8_t gryo_y_h;
		uint8_t gryo_y_l;
		uint8_t gryo_z_h;
		uint8_t gryo_z_l;
	} StrIMU_Gryo_packet;

	StrIMU_Acc_packet acc_data;
	StrIMU_Gryo_packet gryo_data;

	// Write average value into offset register
	tx_data[0] = 0x77;
	tx_data[1] = 0 ;
	tx_data[2] = 0 ;
	TwiComm_master_write_bytes(&packet, MPU9255_SLA, tx_data, 3);
	_delay_ms(10);

	for(int i=0; i<300; i++) {
		TwiComm_master_write_byte(&packet, MPU9255_SLA, tx_data);
		_delay_ms(10);

		TwiComm_master_read_bytes(&packet, MPU9255_SLA, &acc_data, sizeof(StrIMU_Acc_packet));
		_delay_ms(10);

		tx_data[0] = 0x43;
		TwiComm_master_write_byte(&packet, MPU9255_SLA, tx_data);
		_delay_ms(10);

		TwiComm_master_read_bytes(&packet, MPU9255_SLA, &gryo_data, sizeof(StrIMU_Gryo_packet));
		_delay_ms(10);

		measure(acc_data.acc_x_h << 8 | acc_data.acc_x_l);
		
	}

	int avg_4 = total_value / count ;
	printf("Before :%d\n", avg_4);

	_delay_ms(1000);

	// Write average value into offset register
	tx_data[0] = 0x77;
	tx_data[1] = -(avg_4) >> 8 ;
	tx_data[2] = -(avg_4) ;
	TwiComm_master_write_bytes(&packet, MPU9255_SLA, tx_data, 3);
	_delay_ms(10);

	total_value = 0;
	count = 0;

	for(int i=0; i<300; i++) {
		TwiComm_master_write_byte(&packet, MPU9255_SLA, tx_data);
		_delay_ms(10);

		TwiComm_master_read_bytes(&packet, MPU9255_SLA, &acc_data, sizeof(StrIMU_Acc_packet));
		_delay_ms(10);

		tx_data[0] = 0x43;
		TwiComm_master_write_byte(&packet, MPU9255_SLA, tx_data);
		_delay_ms(10);

		TwiComm_master_read_bytes(&packet, MPU9255_SLA, &gryo_data, sizeof(StrIMU_Gryo_packet));
		_delay_ms(10);

		measure(acc_data.acc_x_h << 8 | acc_data.acc_x_l);
		
	}
	avg_4 = total_value / count ;

	printf("After :%d\n", avg_4);

	while(1);

	while(1) {

		tx_data[0] = 0x3B;
		TwiComm_master_write_byte(&packet, MPU9255_SLA, tx_data);
		_delay_ms(10);

		TwiComm_master_read_bytes(&packet, MPU9255_SLA, &acc_data, sizeof(StrIMU_Acc_packet));
		_delay_ms(10);

		tx_data[0] = 0x43;
		TwiComm_master_write_byte(&packet, MPU9255_SLA, tx_data);
		_delay_ms(10);

		TwiComm_master_read_bytes(&packet, MPU9255_SLA, &gryo_data, sizeof(StrIMU_Gryo_packet));
		_delay_ms(10);



		
		//printf("Get Data Reg[%02X:%02X] =  %02X%02X Dec: %d \n", 
			//tx_data[0], tx_data[0]+1,  rx_data[0], rx_data[1], rx_data[0] << 8 | rx_data[1]);
		printf("A_x:%5d \tA_y%5d \tA_z%5d \tG_x:%5d \tG_y:%5d \tG_z:%5d\n",
			acc_data.acc_x_h << 8 | acc_data.acc_x_l,
			acc_data.acc_y_h << 8 | acc_data.acc_y_l,
			acc_data.acc_z_h << 8 | acc_data.acc_z_l,
			gryo_data.gryo_x_h << 8 | gryo_data.gryo_x_l,
			gryo_data.gryo_y_h << 8 | gryo_data.gryo_y_l,
			gryo_data.gryo_z_h << 8 | gryo_data.gryo_z_l);

	}

}
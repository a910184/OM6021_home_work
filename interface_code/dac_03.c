#define F_CPU 11059200U
#include "ASA_Lib.h"

#define	DAC00_ID (1)

typedef struct{
	uint16_t channel_a;
	uint16_t channel_b;
} DACValue;

#define TYPE_FIFO_DATA	DACValue
#define SIZE_FIFO		(256)

/****************** Ring buffer ******************/
typedef struct{
	volatile uint16_t tail;
	volatile uint16_t head;
	volatile TYPE_FIFO_DATA data[SIZE_FIFO];
} FIFO;

static volatile FIFO dac_data_fifo;

void FIFO_init(FIFO* volatile p_fifo_str) {
	// Initialize the FIFO struct
	//memset(p_fifo_str, 0, sizeof(FIFO));
	p_fifo_str->tail = 0;
	p_fifo_str->head = 0;
}

int FIFO_pop(FIFO* volatile p_fifo_str, TYPE_FIFO_DATA* volatile p_data) {

	/*** Make sure there is atomic operation at FIFO ***/
	cli();
	PORTC |= 0X04;
	// There is empty FIFO
	if(p_fifo_str->tail == p_fifo_str->head) {
		PORTC &= ~0X04;
		sei();
		return -1;
	}

	uint16_t tail_next = p_fifo_str->tail + 1;

	if(tail_next >= SIZE_FIFO) {
		tail_next = 0;
	}

	// Get the data at head
	//p_data->channel_a = p_fifo_str->data[p_fifo_str->tail].channel_a;
	//p_data->channel_b = p_fifo_str->data[p_fifo_str->tail].channel_b;
	*p_data = p_fifo_str->data[p_fifo_str->tail];

	//printf("FIFO pop: %d \n",*p_data);
	//debug_write_data(0);
	//debug_write_data(*p_data);

	p_fifo_str->tail = tail_next;
	PORTC &= ~0X04;
	sei();
	/**************************************************/

	return 0;
}

int FIFO_push(FIFO* volatile p_fifo_str, TYPE_FIFO_DATA* data) {


	/*** Make sure there is atomic operation at FIFO ***/
	cli();
	PORTC |= 0X02;

	uint16_t head_next = p_fifo_str->head + 1;

	// Make sure there is not large than SIZE_FIFO
	if(head_next >= SIZE_FIFO) {
		head_next = 0;
	}
	if(head_next == p_fifo_str->tail) {
		// The FIFO was full
		PORTC &= ~0X02;
		printf("Full!!\n");
		sei();
		return -1;
	}

	//p_fifo_str->data[p_fifo_str->head].channel_a = data->channel_a;
	//p_fifo_str->data[p_fifo_str->head].channel_b = data->channel_b;
	p_fifo_str->data[p_fifo_str->head] = *data;

	p_fifo_str->head = head_next;
	PORTC &= ~0X02;
	sei();
	/**************************************************/
	return 0;
}
/***************************************************/

ISR(TIMER0_COMP_vect) {
	static int tick=0;
	tick++;
	if(tick == 100) {
		tick = 0;
	}

	float rad = (float)tick * 2.0 * M_PI * 0.01;

	float sin_output = sin(rad) + 1.0;
	float cos_output = cos(rad) + 1.0;

	uint16_t dac_output1 = (uint16_t)(sin_output * 4095.0) / 2 ;
	uint16_t dac_output2 = (uint16_t)(cos_output * 4095.0) / 2 ;

	ASA_DAC00_set(DAC00_ID, 200, 0x30, 4, 0x00); // 輸出通道1 S1S2
	ASA_DAC00_put(DAC00_ID, 0, 2, &dac_output1); //DAC卡輸出波型
	ASA_DAC00_set(DAC00_ID, 200, 0x30, 4, 0x01); // 輸出通道2 S5S6
	ASA_DAC00_put(DAC00_ID, 0, 2, &dac_output2); //DAC卡輸出波型

	DACValue dac_data;
	dac_data.channel_a = dac_output1;
	dac_data.channel_b = dac_output2;


	FIFO_push(&dac_data_fifo, &dac_data);
}

// Initialize the DAC00
void init_dac() {
	ASA_DAC00_set(DAC00_ID, 200, 0x80, 7, 0x01); // 單通道非同步模式
	ASA_DAC00_set(DAC00_ID, 200, 0x30, 4, 0x00); // 輸出通道1 S1S2
}

// Initizlize the TIME0 with CTC mode, interrupt at 2000 Hz
void init_timer() {
	//TCCR0 = 0b00001111;
	// Pre-scale 32
	//TCCR0 = 0b00001011;
	// Pre-scale 64
	//TCCR0 = 0b00001100;
	// Pre-scale 128
	TCCR0 = 0b00001101;
	OCR0 = 172;
	TIMSK |= 0x01 << OCIE0;
}

void main(void) {

	ASA_M128_set();
	DDRC |= 0XFF;

	/* Variables for ASA lib */
	unsigned char ASA_ID = 4, Mask = 0xFF, Shift = 0, Setting = 0xFF;

	char check = 0;	// module communication result state flag

	int store_sec = 0;
	printf("How many sample to be recorded? \n");
	scanf("%d", &store_sec);
	uint32_t store_count = 2000L * store_sec;
	printf("Sample %ld sample  \n", store_count);

	/*** Open file ***/
	// Select target file
	ASA_SDC00_put(ASA_ID, 64, 8, "dacdata");
	ASA_SDC00_put(ASA_ID, 72, 3, "bin");

	// Configure to open file
	Setting = 0x05;		// Setting mode for openFile (overlapp or create)
	check = ASA_SDC00_set(ASA_ID, 200, Mask, Shift, Setting);
	if( check != 0 ) {  // 檢查回傳值做後續處理
		printf("Debug point 41, error code <%d>\n", check);
		return;
	}

	//printf("Write data to file with overlapp(or create file) mode [Start]!!\n");

	uint32_t record_count = 0;

	DACValue dac_value;
	dac_value.channel_a = 0;
	dac_value.channel_b = 0;
	ASA_SDC00_put(ASA_ID, 0, sizeof(DACValue), &dac_value);

	// Start recording ADC data
	FIFO_init(&dac_data_fifo);
	init_dac();
	init_timer();

	sei();

	while(record_count < store_count) {
		// Blocking while there was no data available
		while(FIFO_pop(&dac_data_fifo,&dac_value));
		// write binary data to SD card
		cli();
		ASA_SDC00_put(ASA_ID, 0, sizeof(DACValue), &dac_value);
		sei();
		record_count++;
	}
	cli();

	//printf("Write data to file with overlap mode [Finish]!!\n");


	// Close file
	check = ASA_SDC00_set(ASA_ID, 200, Mask, Shift, 0x00);
	if( check != 0 ) {				// 檢查回傳值做後續處理
		printf("Debug point 42, error code <%d>\n", check);
		return;
	}

	/********* Read the file *********/
	// Configure to open file
	Setting = 0x01;		// Setting mode for openFile (read only)
	check = ASA_SDC00_set(ASA_ID, 200, Mask, Shift, Setting);
	if( check != 0 ) {  // 檢查回傳值做後續處理
		printf("Debug point 41, error code <%d>\n", check);
		return;
	}

	// Reading file
	// read binary data from SD card to MATLAB via HMI
	for(uint32_t i=0; i<store_count*sizeof(DACValue); i++) {
		int8_t data_record;
		ASA_SDC00_get(ASA_ID, 0, 1, &data_record);
		while((UCSR0A&(1<<UDRE0))==0);
		UDR0 = data_record;
	}


	// Close file
	check = ASA_SDC00_set(ASA_ID, 200, Mask, Shift, 0x00);
	if( check != 0 ) {				// 檢查回傳值做後續處理
		printf("Debug point 42, error code <%d>\n", check);
		return;
	}
	/*********************************/

	return;

	return;
}

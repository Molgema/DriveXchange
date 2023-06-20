#include <atmel_start.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <string.h>
enum State {
	HEFFEN,
	VOORMAGNEET,
	START,
	NAMAGNEET,
	WACHTEN,
};

uint16_t adc_res = 0;
uint16_t adc_res1 = 0; 
uint16_t adc_res2 = 0;
uint16_t adc_res3 = 0;
uint16_t adc_res4 = 0;
uint16_t adc_res5 = 0;
uint16_t adc_res6 = 0;
uint16_t adc_res7 = 0;
uint16_t adc_resm = 0;

enum State motorState = START;

int switchpressed = 0;
int usartflag = 0;
volatile uint8_t dummy = 0; //Initialization of dummy for flushing the receiver buffer
volatile uint8_t HighLen = 0; //Initialization of Most Significant Byte
volatile uint8_t LowLen = 0; //Initialization of Least Significant Byte

volatile uint16_t distance = 0; //checken van afstand in mm
volatile uint16_t Len_mm = 0; //berekende afstand in mm
volatile uint16_t samples = 0; //hoeveelheid samples voor gemiddelde
volatile uint16_t sum_Len = 0; //som van alle samples
volatile uint16_t avg_Len = 0; //gemiddelde van de samples

//Initialisatie van variabelen voor implementeren delay tussen versturen puls en ontvangen puls
volatile unsigned int send_flag = 0; //flag voor sturen van puls
volatile unsigned int timer_flag = 0; //flag voor wanneer data weer ontvangen moet worden
volatile unsigned int countAS = 0;
volatile unsigned int countRead = 0;
volatile int sendFlag = 0; 

char buffer[24]; //Initialization of buffer for displaying data through USART3

volatile unsigned int reed1_flag = 0;
volatile unsigned int reed2_flag = 0;



ISR (PORTE_PORT_vect) {
	if (motorState == HEFFEN) {
		_delay_ms(50); 
		
		if (!REED1_get_level()) {
			reed1_flag = 1;
			reed2_flag = 0;
		}
		
	}
	
	if (motorState == WACHTEN) {
		_delay_ms(50);
		
		if (!REED2_get_level()) {
			reed1_flag = 0;
			reed2_flag = 1;
		}
	}		
	
	PORTE.INTFLAGS = PIN2_bm;
	PORTE.INTFLAGS = PIN1_bm;
}


ISR(PORTB_PORT_vect) {
		switchpressed = 1;	
		PORTB.INTFLAGS = PIN2_bm;
}


ISR (ADC0_RESRDY_vect) 
{
	switch (ADC0_MUXPOS) {
		case 0x06:
			adc_res = ADC0_RES >> 4;
			ADC0.MUXPOS = 0x03;
			sendFlag += 1; 
			break;  
			//IR0
			
		case 0x03:
			adc_res1 = ADC0_RES >> 4;
			ADC0.MUXPOS = 0x02;
			sendFlag += 1;
			break;
		//IR1
		case 0x02:
			adc_res2 = ADC0_RES >> 4;
			ADC0.MUXPOS = 0x01;
			sendFlag += 1;
			break;
		//IR2
		
		case 0x01:
			adc_res3 = ADC0_RES >> 4;
			ADC0.MUXPOS = 0x00;
			sendFlag += 1;
		break;
		//IR3
		case 0x00:
			adc_res4 = ADC0_RES >> 4;
			ADC0.MUXPOS = 0x07;
			sendFlag += 1;
		break;
		//IR4
		case 0x07:
			adc_res5 = ADC0_RES >> 4;
			ADC0.MUXPOS = 0x05;
			sendFlag += 1;
			break;
		//IR5
		case 0x05:
			adc_res6 = ADC0_RES >> 4;
			ADC0.MUXPOS = 0x04;
			sendFlag += 1;
		break;
		//IR6
		case 0x04:
			adc_res7 = ADC0_RES >> 4;
			ADC0.MUXPOS = 0x0B;
			sendFlag += 1;
			break;
		//IR7
		case 0x0B:
			adc_resm = ADC0_RES >> 4;
			ADC0.MUXPOS = 0x06;
			sendFlag += 1;
		break;
		//MAGNEET
	}	
		
	ADC0.INTFLAGS = ADC_RESRDY_bm; 
}

uint8_t USART_2_get_data ()
{
	return USART2.RXDATAL;
}


void USART_2_write (volatile uint8_t data)
{
	/*Waits until register of Transmitter has shifted out*/
	while(!(USART3.STATUS & USART_DREIF_bm))
	{
		;
	}
	
	USART2.TXDATAL = data;
}

/*Flushes the receiver buffer of micro controller*/
void FlushReceiver ( )
{
	/*Waits until register of Receiver has shifted out and proceeds to store the remaining data in a dummy*/
	while (USART2.STATUS & USART_RXCIF_bm) {
		dummy = USART_2_get_data();
	}
}
//

void USART3_sendChar(char c)
{
	while(!(USART3.STATUS & USART_DREIF_bm));
	
	USART3.TXDATAL = c;
}

/*Testing with USART3*/
void USART3_sendString(char *str)
{
	for(size_t i = 0; i < strlen(str); i++)
	{
		USART3_sendChar(str[i]);
	}
}

void timerAS ()
{
	if (countAS == 3) {
		timer_flag = 1; //usart2 kan nu waardes uitlezen
	}

	if (send_flag) {
		countAS++; //counter wordt opgeteld zodra puls is verstuurd
		countRead;
	}
}

/*Gets the distance data from the US-100 and then displays it through USART3 */
void GetDistance ()
{
	if (!send_flag) {
		FlushReceiver();
		USART_2_write(0x55); //0x55 is the input used for the US-100 to send distance data
		
		send_flag = 1;
	}
	
	if (timer_flag)
	{
		
		HighLen = USART_2_get_data(); //Verkrijgen van most significant byte uit de US-100
		LowLen = USART_2_get_data(); //Verkrijgen van least significant byte uit de US-100
		Len_mm = (HighLen * 256) + LowLen; //Omzetten van bytes naar geheel getal om afstand in mm te krijgen
		//		dist = Len_mm;
		// 		samples++;
		// 		sum_Len += Len_mm;
		//
		// 		if (samples == 4) {
		// 			avg_Len = sum_Len / samples;
		// 			samples = 0;
		// 			sum_Len = 0;
		// 		}
		
		if ((Len_mm >= 20) && (Len_mm <= 500)) {
			sprintf(buffer, "%d\n", Len_mm);
			USART3_sendString(buffer);
		}
		
		if (countRead >= 2560 && Len_mm <= 200){
			motorState = START;
			countRead = 0;
		}
		
		//Len_mm = 0; //Resets the data for displaying through USART3
		timer_flag = 0;
		send_flag = 0;
		countAS = 0;
	}
}

//void motorControl(uint16_t IR[8];)
void motorControl(uint16_t IR0, uint16_t IR1, uint16_t IR2, uint16_t IR3, uint16_t IR4, uint16_t IR5, uint16_t IR6, uint16_t IR7, uint16_t magneetWaarde) {


    switch (motorState) {
		case START:
		//TCA0.SPLIT.HCMP2 = 0;
		
		TCA0.SPLIT.LCMP0 = 0;
		TCA0.SPLIT.LCMP2 = 0;
		LED0_set_level(1);
		if (switchpressed == 1){
			switchpressed = 0;
			motorState = VOORMAGNEET;
		}	
		break;
		
	    case VOORMAGNEET:
		if (magneetWaarde >= 100) {
			motorState = HEFFEN;
		}
		if (switchpressed == 1){
			switchpressed = 0;
			motorState = START;
		}
		if (IR0 > 180 && IR7 > 180 && IR4 < 180){
			TCA0.SPLIT.LCMP0 = 255;
			TCA0.SPLIT.LCMP2 = 255;
			
		}
		TCA0.SPLIT.LCMP0 = ((270-(IR0 + IR1 + IR2 + IR3)/4));
		TCA0.SPLIT.LCMP2 = ((270-(IR4 + IR5 + IR6 + IR7)/4));
		LED0_set_level(0);
		break;
		
		
	    case HEFFEN:
		TCA0.SPLIT.HCMP0 = (255);
		TCA0.SPLIT.LCMP1 = 0;
	
		if (reed1_flag == 1)
		{
			motorState = WACHTEN;
	
		}
	
		TCA0.SPLIT.LCMP0 = 0;
		TCA0.SPLIT.LCMP2 = 0;
		LED0_set_level(1);
	    break;
		
		
	    case WACHTEN:
		TCA0.SPLIT.LCMP0 = 0;
		TCA0.SPLIT.LCMP2 = 0;
		
		TCA0.SPLIT.HCMP0 = (0);
		TCA0.SPLIT.LCMP1 = (255);
		
		//_delay_ms(6600);
				
		if (reed2_flag == 1) {
			TCA0.SPLIT.LCMP1 = (0);
			TCA0.SPLIT.HCMP0 = (0);
			motorState = NAMAGNEET;
		}
		
		break;
		
		case NAMAGNEET:
		
		TCA0.SPLIT.LCMP0 = ((260-(IR0 + IR1 + IR2 + IR3)/4));
		TCA0.SPLIT.LCMP2 = ((260 -(IR4 + IR5 + IR6 + IR7)/4));
		LED0_set_level(0);
		
		timerAS();
		GetDistance();
		
		if (switchpressed == 1){
			switchpressed = 0;
			motorState = START;
		}
		break;
    }
}

int main(void){
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	PORTB_PIN2CTRL |= PORT_ISC_FALLING_gc;	
	VREF_ADC0REF |= VREF_REFSEL_VDD_gc; 
	PORTE_PIN1CTRL |= PORT_ISC_FALLING_gc;	
	PORTE_PIN2CTRL |= PORT_ISC_FALLING_gc;	

	/* Replace with your application code */
	while (1) {
		sleep_mode(); 
		
		//sprintf(buffer, "%d\n", Len_mm);
		
// 		if (sendFlag  == 9) 
// 		{
// 			USART3_sendString(buffer);
// 			sendFlag = 0;
// 		}

		motorControl(adc_res, adc_res1, adc_res2, adc_res3, adc_res4, adc_res5, adc_res6, adc_res7, adc_resm); 
		//motorControl(adc_res[8]); 
	}
}

#include <atmel_start.h>
#include <avr/sleep.h>

uint16_t adc_res = 0;
uint16_t adc_res1 = 0; 
char buffer[32];
volatile int sendFlag = 0; 

ISR (ADC0_RESRDY_vect) 
{
	switch (ADC0_MUXPOS) {
		case 0x06:
			adc_res = ADC0_RES >> 4;
			ADC0.MUXPOS = 0x04;
			sendFlag += 1; 
			break;  
		case 0x04:
			adc_res1 = ADC0_RES >> 4; 
			ADC0.MUXPOS = 0x06;
			sendFlag += 1; 
			break;
	}
	
	ADC0.INTFLAGS = ADC_RESRDY_bm; 
}

void motorControl(uint16_t IR0, uint16_t IR7) {
	if (IR7 < IR0) {
		TCA0.SPLIT.LCMP0 = 220;
		TCA0.SPLIT.LCMP2 = 140;
	}
	
	if ( IR0< IR7) {
		TCA0.SPLIT.LCMP0 = 220;
		TCA0.SPLIT.LCMP2 = 140; 
	}
	else{
	TCA0.SPLIT.LCMP0 = 200;
	TCA0.SPLIT.LCMP2 = 200;
	
	}
	
	}

/*Testing with USART3*/
void USART_3_sendChar(uint8_t data)
{
	while (!(USART3.STATUS & USART_DREIF_bm));
	USART3.TXDATAL = data;
}

/*Testing with USART3*/
void USART_3_sendString(char * str)
{
	for(volatile size_t i = 0; i < strlen(str); i++)
	{
		USART_3_sendChar(str[i]);
	}
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	VREF_ADC0REF |= VREF_REFSEL_VDD_gc; 

	/* Replace with your application code */
	while (1) {
		sleep_mode(); 
		
		sprintf(buffer, "%d %d\n", adc_res, adc_res1);
		
		if (sendFlag  == 2) 
		{
			USART_3_sendString(buffer);
			//USART_3_sendChar(adc_res);
			sendFlag = 0;
		}
		
		motorControl(adc_res, adc_res1); 
	}
}

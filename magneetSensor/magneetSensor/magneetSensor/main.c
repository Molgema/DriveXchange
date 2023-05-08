#include <atmel_start.h>
#include <avr/sleep.h>

uint16_t adc_res = 0;

ISR (ADC0_RESRDY_vect)
{
	adc_res = ADC0_RES;
	
	ADC0.INTFLAGS = ADC_RESRDY_bm;
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
		
		uint8_t magneetWaarde = adc_res >> 4;
		
		LED0_set_level(true);
		
		if (magneetWaarde >= 150) {
			LED0_set_level(false);
		}
		
		USART_3_sendChar(adc_res >> 4);
	}
}

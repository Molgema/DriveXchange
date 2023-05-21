#include <atmel_start.h>
#include <avr/sleep.h>

volatile double magVal = 0; 
volatile double	 magSum = 0;
volatile double magAvg = 0; 
volatile double magCounter = 0;
char ValBuffer[24];  
char AvgBuffer[24]; 

ISR (ADC0_RESRDY_vect)
{
	magVal = ADC0.RES / 16.0;
	magSum += magVal;
	magCounter = magCounter + 1.0;
	
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
		
		sprintf(ValBuffer, "Huidige magneetwaarde is %.2lf. \n", magVal);
		
		if (magCounter == 9.0) {
			magAvg = magSum / magCounter;
			sprintf(AvgBuffer, "Gemiddelde magneetwaarde is %.2lf. \n", magAvg);
			USART_3_sendString(AvgBuffer);
			
			magCounter = 0;
			magSum = 0; 
		}
		
		if (magAvg >= 150) LED_set_level(1);
		else LED_set_level(0);

	}
}

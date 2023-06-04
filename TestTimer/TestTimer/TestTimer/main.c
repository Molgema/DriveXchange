#include <atmel_start.h>
#include <avr/sleep.h>

volatile unsigned int button_flag = 0; 
volatile unsigned int counterHef = 0; 

ISR (ADC0_RESRDY_vect) {
	if (button_flag) counterHef++;
	
	ADC0_INTFLAGS = ADC_RESRDY_bm;
}

ISR (PORTB_PORT_vect) {
	button_flag = 1; 

	PORTB.INTFLAGS = PIN2_bm; 
}
	

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	PORTB.PIN2CTRL |= PORT_ISC_FALLING_gc; //Setting up interrupt for pressing SW0 (PB2)

	/* Replace with your application code */
	while (1) {
		sleep_mode(); 
		
		if (counterHef == 2560) {
			LED0_toggle_level();
			button_flag = 0; 
			counterHef = 0;
		}
			
	}
}

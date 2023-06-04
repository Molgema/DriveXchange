#include <atmel_start.h>
#include <avr/sleep.h>

volatile unsigned int reset_flag = 0; 
volatile unsigned int reed1_flag = 0; 
volatile unsigned int reed2_flag = 0; 

ISR (PORTB_PORT_vect) {
	reset_flag = 1; 
	
	PORTB.INTFLAGS = PIN2_bm; 
}

ISR (PORTE_PORT_vect) {
	reed1_flag = 1; 
	
	PORTE.INTFLAGS = PIN2_bm; 
	PORTE.INTFLAGS = PIN1_bm; 
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	PORTE.PIN1CTRL |= PORT_ISC_FALLING_gc;
	PORTE.PIN2CTRL |= PORT_ISC_FALLING_gc;
	PORTB.PIN2CTRL |= PORT_ISC_FALLING_gc;

	/* Replace with your application code */
	while (1) {
		sleep_mode(); 
		
		if (reset_flag) 
		{
			reed1_flag = 0; 
			reset_flag = 0; 
		}
		
		if (reed1_flag) LED0_set_level(0); 
		else LED0_set_level(1);
	}
}

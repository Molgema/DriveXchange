#include <atmel_start.h>
#include <avr/sleep.h>
#include <util/delay.h>

unsigned int  switch_flag = 0; 

ISR (PORTB_PORT_vect) {
	switch_flag = !switch_flag; 	
	
	PORTB.INTFLAGS = PIN2_bm;
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	PORTB_PIN2CTRL |= PORT_ISC_BOTHEDGES_gc; 

	/* Replace with your application code */
	while (1) {
// 		PB3_toggle_level();
// 		_delay_ms(1000);
// 		PB3_toggle_level();
		
		
		sleep_mode(); 
		
		if (switch_flag) {
			TCA0.SPLIT.LCMP0 = 150;
		}
		else {
			TCA0.SPLIT.LCMP1 = 150;
		} 

	
	}
}

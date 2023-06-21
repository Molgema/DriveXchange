#include <atmel_start.h>
#include <util/delay.h>

unsigned int switch_flag = 0; 

ISR (PORTB_PORT_vect) {
	switch_flag = !switch_flag; 
	
	PORTB.INTFLAGS = PIN2_bm;
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	PORTB.PIN2CTRL |= PORT_ISC_FALLING_gc; 

	/* Replace with your application code */
	while (1) {
// 		L_toggle_level(); 
// 		_delay_ms(500);
// 		L_toggle_level();
		
		if (switch_flag) {
			//L_set_level(0);
			TCA0.SPLIT.HCMP0 = 255; 
		}
		else {
			TCA0.SPLIT.HCMP0 = 0;  
		}
		
	}
}

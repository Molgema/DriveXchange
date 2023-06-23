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
		if (!SW0_get_level())
		{
			//TCA0.SPLIT.HCMP0  = 200; //omlaag voor robot 2
			//TCA0.SPLIT.HCMP1  = 150; not
			TCA0.SPLIT.LCMP1  = 200; //omhoog voor robot 2
			//TCA0.SPLIT.HCMP2  = 255; not
		}
		else {
			//TCA0.SPLIT.HCMP0 = 0;
			//TCA0.SPLIT.HCMP1  = 0; not 
			TCA0.SPLIT.LCMP1  = 0;
			//TCA0.SPLIT.HCMP2  = 0; not
		}
	
		
	}
}

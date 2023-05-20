#include <atmel_start.h>
#include <avr/sleep.h>
#include <util/delay.h>

volatile unsigned int delay_flag = 0; 
volatile unsigned int rtc_counter = 0; 

ISR (PORTB_PORT_vect) {
	delay_flag = 1; 
	rtc_counter = 0; 
	
	PORTB.INTFLAGS = PIN2_bm;  	
}

ISR (RTC_CNT_vect) {
	if (delay_flag) rtc_counter++; 
	
	RTC.INTFLAGS = RTC_OVF_bm;
}


int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	PORTB.PIN2CTRL |= PORT_ISC_FALLING_gc; 

	/* Replace with your application code */
	while (1) {
		sleep_mode();
		
		if (delay_flag && rtc_counter == 80) {
			LED0_toggle_level(0); 
			delay_flag = 0;
			rtc_counter = 0;  	
		}

	}
}

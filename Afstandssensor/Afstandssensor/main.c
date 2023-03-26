#include <atmel_start.h>
#include <util/delay.h>
#include <string.h>
#include <avr/sleep.h>

volatile uint8_t dummy = 0; //Initialization of dummy for flushing the receiver buffer
volatile uint8_t HighLen = 0; //Initialization of Most Significant Byte
volatile uint8_t LowLen = 0; //Initialization of Least Significant Byte

volatile uint16_t Len_mm = 0; //Initialization of distance

volatile unsigned int buttonFlag = 0; //Initialization of flag for sending and receiving through USART
 
char buffer[24]; //Initialization of buffer for displaying data through USART3

/*Receives and returns the data from the receiver of the US-100*/
uint8_t USART_2_get_data () 
{	
	return USART2.RXDATAL;
}

/*Writes the input to the transmitter of the US-100*/
void USART_2_write (volatile uint8_t data) 
{
 	/*Waits until register of Transmitter has shifted out*/
	 while(!(USART3.STATUS & USART_DREIF_bm))
     {
         ;
     }
	
	USART2.TXDATAL = data; 
}


void USART3_sendChar(char c)
{
	while(!(USART3.STATUS & USART_DREIF_bm));
	
	USART3.TXDATAL = c;
}

/*Sends the contents character for character toward the USART3 connection*/
void USART3_sendString(char *str)
{
	for(size_t i = 0; i < strlen(str); i++)
	{
		USART3_sendChar(str[i]);
	}
}

/*Flushes the receiver buffer of micro controller*/
void FlushReceiver ( )
{
	/*Waits until register of Receiver has shifted out and proceeds to store the remaining data in a dummy*/
	while (USART2.STATUS & USART_RXCIF_bm) {
		dummy = USART_2_get_data();
	}	 
}

/*Gets the distance data from the US-100 when given the amount of samples and then displays it through USART3 */
void GetDistance (volatile int samples) 
{
	for (volatile int counter = 0; counter <= samples; counter++) {
		USART_2_write(0x55); //0x55 is the input used for the US-100 to send distance data
		_delay_ms(20); //Giving sensor some time to get non-zero data and to gather 31 samples between 0s and 600 ms
		
		HighLen = USART_2_get_data(); //Reads the most significant byte from the US-100
		LowLen = USART_2_get_data(); //Reads the least significant byte from the US-100
		Len_mm = (HighLen * 256) + LowLen; //Calculates distance from object
	
		if ((Len_mm >= 20) && (Len_mm <= 4500)) {
			sprintf(buffer, "%d\n", Len_mm);
			USART3_sendString(buffer);	
			
			Len_mm = 0; //Resets the data for displaying through USART3
		}
	} 
}

/*Generates an interrupt when SW0 (PB2) is pressed*/
ISR (PORTB_PORT_vect) {
  	buttonFlag = 1; //Setting button flag
  	PORTB.INTFLAGS = PIN2_bm; //Clearing the interrupt of PB2
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	PORTB.PIN2CTRL |= PORT_ISC_FALLING_gc; //Setting up interrupt for pressing SW0 (PB2)

	/* Replace with your application code */
	while (1)
	{	 
		sleep_mode(); 
		/*The micro controller will wake up here from idle mode and start with the interrupt*/
		
		_delay_ms(75); //Giving delay to able to measure
		FlushReceiver();
		
		/*When the button flag is set the USART3 will display 30 samples of the US-100*/
  		if ( buttonFlag == 1 ) {
  			GetDistance(30);
  		}
 		
 		buttonFlag = 0; //Clearing button flag	
	}
}

#include <atmel_start.h>
#include <util/delay.h>
#include <string.h>

volatile uint8_t dummy = 0; //Initialization of dummy for flushing the receiver buffer
volatile uint8_t HighLen = 0; //Initialization of Most Significant Byte
volatile uint8_t LowLen = 0; //Initialization of Least Significant Byte

volatile uint16_t Len_mm = 0; //Initialization of distance
volatile uint16_t samples = 0; 
volatile uint16_t sum_Len = 0; 
volatile uint16_t avg_Len = 0; 

volatile unsigned int distCounter = 0; 
volatile unsigned int dist_timer_flag = 0; 

char buffer[24]; //Initialization of buffer for displaying data through USART3

ISR (RTC_CNT_vect) {
	if (counter == 32) {
		timer_flag = 1;
		counter = 0;  
	}
	
	counter++; //counter bijhouden hoeveelheid uitlezen
	
 	RTC.INTFLAGS = RTC_OVF_bm; 
}

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

/*Gets the distance data from the US-100 and then displays it through USART3 */
void GetDistance ()
{
	//if (dist_send_flag) {
		if (!send_flag) {
			send_flag = 1; 
			USART_2_write(0x55);
		} //0x55 is the input used for the US-100 to send distance data
			//dist_send_flag = 0; 
	//} 
	
	if (timer_flag)
	{ 
		//_delay_ms(50); //Giving sensor some time to get non-zero data
		
		HighLen = USART_2_get_data(); //Reads the most significant byte from the US-100
		LowLen = USART_2_get_data(); //Reads the least significant byte from the US-100
		Len_mm = (HighLen * 256) + LowLen; //Calculates distance from object
		samples++;
		sum_Len += Len_mm;
		
		if (samples == 4) {
			avg_Len = sum_Len / samples; 
			samples = 0; 
			sum_Len = 0;
		}
		
		if ((avg_Len >= 20) && (avg_Len <= 500)) {
			sprintf(buffer, "%d\n", avg_Len);
			USART3_sendString(buffer);
		}
		
		Len_mm = 0; //Resets the data for displaying through USART3	
		timer_flag = 0;	
	}
}



int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	/* Replace with your application code */
	while (1)
	{		
		FlushReceiver();
		GetDistance ();
		//_delay_ms(50);	
		/*When the button flag is set the USART3 will display 30 samples of the US-100*/

	}
}


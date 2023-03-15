#include <atmel_start.h>
#include <util/delay.h>
#include <usart_basic.h>

unsigned int HighLen = 0;  
unsigned int LowLen = 0;
unsigned int Len_mm = 0; 
volatile unsigned int i = 0;

/* Checks how many bytes are available */
bool AvailableBytes (unsigned int bytes) 
{
	while (i < bytes) 
	{
		if ( USART_0_is_rx_ready() ) 
		{
			i++;		
		} 
	}
	
	return true;
}

/* Clear receive buffer */
void FlushReceiver ( )
{
	while ( !USART_0_is_rx_ready() )
	{
		USART_0_read();
	}		
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/* Replace with your application code */
	while (1) 
	{
		FlushReceiver(); /* Clear receive buffer */
					
		_delay_ms(500);
		USART_0_write(0x55);
		
		if (AvailableBytes(2)) 
		{
			HighLen = USART_0_read();
			LowLen = USART_0_read();
			
			Len_mm = (HighLen*256) + LowLen; 
		}
	}
}
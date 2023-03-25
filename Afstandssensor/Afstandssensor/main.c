#include <atmel_start.h>
#include <util/delay.h>
#include <usart_basic.h>
#include <string.h>

unsigned int HighLen = 0;
unsigned int LowLen = 0;
unsigned int Len_mm = 0;
volatile unsigned int i = 0;

/* Checks how many bytes are available */
bool AvailableBytes (unsigned int bytes)
{
	while (i < bytes)
	{
		if ( USART_2_is_rx_ready() )
		{
			i++;
		}
	}
	
	return true;
}

/* Clear receive buffer */
void FlushReceiver ( )
{
	while ( !USART_2_is_rx_ready()) {
		USART_2_get_data();
	}

}

void USART3_sendChar(char c)
{
	while(!(USART3.STATUS & USART_DREIF_bm));
	
	USART3.TXDATAL = c;
}

void USART3_sendString(char *str)
{
	for(size_t i = 0; i < strlen(str); i++)
	{
		USART3_sendChar(str[i]);
	}
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/* Replace with your application code */
	while (1)
	{
		//FlushReceiver(); /* Clear receive buffer */
		
		_delay_ms(50);
		USART_2_write(0x55);
		USART_3_write(USART_2_get_data()); 
		
		//for ()
			//Len_mm += USART_2_get_data();
		
		
		//if (AvailableBytes(2))
		//{
		//	HighLen = USART_2_get_data();
		//	LowLen = USART_2_get_data();
		//	
		//	Len_mm = (HighLen*256) + LowLen;
			
			//USART3_sendString(HighLen);
			//USART3_sendString(LowLen);
		//}
	}
}

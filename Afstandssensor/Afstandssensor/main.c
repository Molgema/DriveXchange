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
		if ( USART_3_is_rx_ready() ) 
		{
			i++;		
		} 
	}
	
	return true;
}

/* Clear receive buffer */
void FlushReceiver ( )
{
	while ( !USART_3_is_rx_ready() )
	{
		USART_3_read();
	}		
}

/* Waarschijnlijk niet nodig omdat USART_0_read() al bytes uitlest van RX */
/* Read low byte from RX*/
//uint8_t USART_3_readLow()
//{
//	while (!(USART3.STATUS & USART_RXCIF_bm))
//	;
//	return USART3.RXDATAL;
//}

/* Read high byte from RX*/
//uint8_t USART_3_readHigh()
//{
//	while (!(USART3.STATUS & USART_RXCIF_bm))
//	;
//	return USART3.RXDATAH;
//}


int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/* Replace with your application code */
	while (1) 
	{
		FlushReceiver(); /* Clear receive buffer */
					
		_delay_ms(500);
		USART_3_write(0x55);
		
		if (AvailableBytes(2)) 
		{
			HighLen = USART_3_read();
			LowLen = USART_3_read();
			
			Len_mm = (HighLen*256) + LowLen; 
		}
	}
}
#include <atmel_start.h>
#include <util/delay.h>
#include <string.h>
#include <avr/sleep.h>

//Initialisatie  van v ariabelen voor berekenen van afstand dmv van US-100
volatile uint8_t dummy = 0; //dummy voor het flushen van de receiver buffer
volatile uint8_t HighLen = 0; //Most Significant Byte ontvangen data
volatile uint8_t LowLen = 0; //Least Significant Byte ontvangen data

volatile uint16_t distance = 0; //checken van afstand in mm
volatile uint16_t Len_mm = 0; //berekende afstand in mm
volatile uint16_t samples = 0; //hoeveelheid samples voor gemiddelde
volatile uint16_t sum_Len = 0; //som van alle samples
volatile uint16_t avg_Len = 0; //gemiddelde van de samples

//Initialisatie van variabelen voor implementeren delay tussen versturen puls en ontvangen puls
volatile unsigned int send_flag = 0; //flag voor sturen van puls
volatile unsigned int timer_flag = 0; //flag voor wanneer data weer ontvangen moet worden
volatile unsigned int countAS = 0; //counter voor US100 geeft sample rate van uitlezing (vb voor counter = 3 geeft 32768/(128*3)= 85.33 samples per seconde)
volatile unsigned int countRead = 0;  

//Variabele voor weergeven data via USART3
char buffer[24]; //buffer voor testen van uitlezing van US-100

ISR (ADC0_RESRDY_vect) {
	if (countAS == 3) {
		timer_flag = 1; //usart2 kan nu waardes uitlezen 
	}
	
	if (send_flag) {
		countAS++; //counter wordt opgeteld zodra puls is verstuurd
		countRead++; 
	}
	
	ADC0.INTFLAGS = ADC_RESRDY_bm;
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

void ControlLED (volatile uint16_t dist) {
	if ((countRead >= 2560) && (dist <= 200)) {
		LED0_set_level(false);
	}
// 	else {
// 		LED0_set_level(false);
// 	}
}

/*Gets the distance data from the US-100 and then displays it through USART3 */
void GetDistance ()
{
	if (!send_flag) {
		FlushReceiver();
		USART_2_write(0x55); //0x55 is the input used for the US-100 to send distance data
		
		send_flag = 1;
	} 
	
	if (timer_flag)
	{		
		HighLen = USART_2_get_data(); //Verkrijgen van most significant byte uit de US-100
		LowLen = USART_2_get_data(); //Verkrijgen van least significant byte uit de US-100		
		Len_mm = (HighLen * 256) + LowLen; //Omzetten van bytes naar geheel getal om afstand in mm te krijgen
//		dist = Len_mm;
// 		samples++;
// 		sum_Len += Len_mm;
// 		
// 		if (samples == 4) {
// 			avg_Len = sum_Len / samples;
// 			samples = 0;
// 			sum_Len = 0;
// 		}
		
		if ((Len_mm >= 0) && (Len_mm <= 500)) {
			sprintf(buffer, "%d\n", Len_mm);
			USART3_sendString(buffer);
		}
		
		ControlLED(Len_mm);
		
		//Len_mm = 0; //Resets the data for displaying through USART3
		timer_flag = 0;
		send_flag = 0;
		countAS = 0;
	}
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	VREF_ADC0REF |= VREF_REFSEL_VDD_gc;
	
	/* Replace with your application code */
	while (1)
	{
		sleep_mode();
		
		GetDistance();
		
	}
}



#include <atmel_start.h>
#include <avr/sleep.h>

/*Amount of sensors on robot*/
#define NUM_SENSORS 8 
// 
// /*Maximum value of the ADC and the IR sensors*/
// #define MAX_SENS_VAL 4095 


char row[128];
char number[128];
volatile double minSensor[NUM_SENSORS] = {0};
volatile double maxSensor[NUM_SENSORS] = {0}; 
volatile double sensorArr[NUM_SENSORS] = {0};
volatile double normArr[NUM_SENSORS] = {0};
volatile double testArr[NUM_SENSORS] = {12, 204, 4095, 4095, 4095, 4095, 4095, 4095};

volatile double posRobot = 0;
volatile double adjustedPos = 0;
volatile double setPoint = (7.0*4095.0)/2;

typedef enum 

/*Variables for setting k values*/
volatile double k_p = (255.0-191.0)/((7.0*4095.0)/2);
volatile double k_i = 0;
volatile double k_d = 0;


volatile bool detLine = false;

uint8_t speedL = 0;
uint8_t speedR = 0;   
	
/*Flag for sending to computer with USART3*/
volatile uint8_t read_flag = 0;
volatile uint8_t send_flag = 0;
volatile uint8_t sensorPos = 0; 

ISR (ADC0_RESRDY_vect) 
{
// 	switch (ADC0.MUXPOS) 
//	{
// 		case 0x09:
// 			k_p = ADC0_RES;
// 			ADC0.MUXPOS = 0x0A;
// 			break; 
// 		case 0x0A:
// 			k_i = ADC0_RES;
// 			ADC0.MUXPOS = 0x0B; 
// 			break; 
// 		case 0x0B:
// 			k_d = ADC0_RES;
// 			ADC0.MUXPOS = 0x09;
// 			break;
// 	}

	read_flag = 1;
	
	ADC0.INTFLAGS = ADC_RESRDY_bm; 
}

/*Testing with USART3*/
void USART_3_sendChar(uint8_t data) 
{
	while (!(USART3.STATUS & USART_DREIF_bm));
	USART3.TXDATAL = data;
}

/*Testing with USART3*/
void USART_3_sendString(char * str)
{
	for(volatile size_t i = 0; i < strlen(str); i++)
	{
		USART_3_sendChar(str[i]);
	}
}

void stateMachine() {
	int lastEvent = currentEvent; 
}

void normalizeSensors(volatile double* normalizedArr ,volatile double* sensorData, volatile double* min, volatile double* max, size_t length) 
{	
	for (volatile size_t i = 0; i < length; i++)
	{	
		if (sensorData[i] < min[i]) 
		{
			min[i] = sensorData[i];
			volatile double minTest = min[i];
		}
		
		if (sensorData[i] > maxSensor[i]) 
		{
			max[i] = sensorData[i];
			volatile double maxTest = max[i];
		}
		
		normalizedArr[i] = ((sensorData[i] - min[i])/(max[i]-min[i]))*255.0;
	}		
}

void readSensors() {
	switch (ADC0.MUXPOS)
	{
		case 0x06:
			sensorArr[sensorPos] = ADC0_RES;		
			ADC0.MUXPOS = 0x03;
			break;
		case 0x03:
			sensorArr[++sensorPos] = ADC0_RES;
			ADC0.MUXPOS = 0x02;
			break;
		case 0x02:
			sensorArr[++sensorPos] = ADC0_RES;
			ADC0.MUXPOS = 0x01;
			break;
		case 0x01:
			sensorArr[++sensorPos] = ADC0_RES;
			ADC0.MUXPOS = 0x00;
			break;
		case 0x00:
			sensorArr[++sensorPos] = ADC0_RES;
			ADC0.MUXPOS = 0x07;
			break;
		case 0x07:
			sensorArr[++sensorPos] = ADC0_RES;
			ADC0.MUXPOS = 0x05;
			break;
		case 0x05:
			sensorArr[++sensorPos] = ADC0_RES;
			ADC0.MUXPOS = 0x04;
			break;
		case 0x04:
			sensorArr[++sensorPos] = ADC0_RES;
			ADC0.MUXPOS = 0x06;
			break;	
	}
}

void calcPos(volatile double* valueSensor, size_t length)
{
	double avgSum = 0; 
	double avg = 0;	
	detLine = false;
	
	for (size_t i = 0; i < 8; i++)
	{
		if (valueSensor[i] < 3276 )
		{
			detLine = true; 
		}
		
		if (valueSensor[i] < 3890) 
		{
			avgSum += (valueSensor[i])*(i*4095);
			avg += valueSensor[i]; 
		}
	}
	
	if (detLine)
	{
		posRobot = setPoint - (avgSum/avg); 
	}
	else if (posRobot < setPoint) 
	{
		posRobot = -setPoint; 
	}
	else 
	{
		posRobot = setPoint; 
	}
}

void motorCtrl()
{	
	adjustedPos = posRobot*k_p;
	
	speedL =  191 - adjustedPos;
	speedR = 191 + adjustedPos;
	
	TCA0.SPLIT.LCMP0 = speedL;
	TCA0.SPLIT.LCMP2 = speedR;
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
		
// 		double proportional = k_p / 4095.0;
// 		double integral = k_i / 4095.0;
// 		double derivative = k_d / 4095.0;
		
		//normalizeSensors(normArr, sensorArr, minSensor, maxSensor, NUM_SENSORS); 
		
		if (read_flag) {
			readSensors(); 
			read_flag = 0; 
		}
		
		if (sensorPos == 7)
		{
			calcPos(testArr, NUM_SENSORS);
			motorCtrl();
			sensorPos = 0;
			send_flag = 1;
		} 
		
		if (send_flag) 
		{
// 			sprintf(row, "%5s %5s %5s\n", "k_p", "k_i", "k_d");
// 			sprintf(number, "%5.3f %5.3f %5.3f\n", proportional, integral, derivative);
			sprintf(row, "%4s %4s %4s %4s %4s %4s %4s %4s %6s %3s %3s %3s \n", "IR0", "IR1", "IR2", "IR3", "IR4", "IR5", "IR6", "IR7", "POS", "Adj", "L", "R"); 
			sprintf(number, "%4.0lf %4.0lf %4.0lf %4.0lf %4.0lf %4.0lf %4.0lf %4.0lf %6.0lf %3.0lf %3d %3d \n\n", sensorArr[0], sensorArr[1], sensorArr[2], sensorArr[3], sensorArr[4], sensorArr[5], sensorArr[6], sensorArr[7], posRobot, adjustedPos, speedL, speedR);
			
			//char buffer[64];
			//sprintf(buffer, "%.0f\n", sensorArr[1]);
			
			USART_3_sendString(row);
			USART_3_sendString(number);
						
			send_flag = 0;
		}
	}
}

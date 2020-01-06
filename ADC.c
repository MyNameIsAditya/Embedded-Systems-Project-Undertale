// ADC.c
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0
// Last Modified: 4/10/2016
// Student names: Aditya Khanna and Hubert Ning
// Last modification date: 15 April 2018

#include <stdint.h>
#include "tm4c123gh6pm.h"

// ADC initialization function 
// Input: none
// Output: none
void ADC_Init(void)
{ 
	uint32_t NOP;
	SYSCTL_RCGCGPIO_R |= 0x08;		//Enable Clock For Port D
	for (NOP=0; NOP<4; NOP++)			//Wait For Clock Initialization
	{
	}
	GPIO_PORTD_DIR_R &= 0xF3;			//Port D Pin 2,3 Is Input
	GPIO_PORTD_AFSEL_R |= 0x0C;		//Enables Alternate Function
	GPIO_PORTD_DEN_R &= 0xF3;			//Disables Port D Pin 2,3 Digital I/O
	GPIO_PORTD_AMSEL_R |= 0x0C;		//Enables Port D Pin 2,3 Analog Function
	SYSCTL_RCGCADC_R |= 0x01;			//Activate ADC0
	for (NOP=0; NOP<4; NOP++)			//Wait For Clock Initialization
	{
	}
	ADC0_PC_R = 0x01;							//Configure For 125,000
	ADC0_SSPRI_R = 0x0023;				//Sequencer 3 Is Highest Priority And Sequencer 2 Is Next (0x0123)
	ADC0_ACTSS_R &= 0xFFF3;				//Disable Sample Seuencer 2,3
	ADC0_EMUX_R &= 0x00FF;				//Sequence 2,3 Is A Software Trigger
	ADC0_SSMUX3_R = (ADC0_SSMUX3_R & 0xFFFFFFF0) + 5;		//Clears SS3 Field And Sets Channel Ain5 (PD2)
	ADC0_SSMUX2_R = (ADC0_SSMUX2_R & 0xFFFFFFF0) + 4;		//Clears SS2 Field And Sets Channel Ain4 (PD3)
	ADC0_SSCTL3_R = 0x0006;		
	ADC0_SSCTL2_R = 0x0006;
	ADC0_IM_R &= 0xFFF3;					//Disables SS2,SS3 Interrupts
	ADC0_ACTSS_R |= 0x000C;				//Enables Sample Sequencer 3
}

//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
uint32_t ADC_In(void)
{  
	uint32_t result;
	ADC0_PSSI_R = 0x0008;							//Initiates SS3
	while ((ADC0_RIS_R & 0x08) == 0) 	//Waits For Conversion
	{
	}
	result = ADC0_SSFIFO3_R & 0x0FFF;	//Reads 12-Bit Result
	ADC0_ISC_R = 0x0008;							//Acknowledges Completion
	return result;
}

uint32_t ADC_In2(void)
{  
	uint32_t result;
	ADC0_PSSI_R = 0x0004;							//Initiates SS2
	while ((ADC0_RIS_R & 0x04) == 0) 	//Waits For Conversion
	{
	}
	result = ADC0_SSFIFO2_R & 0x0FFF;	//Reads 12-Bit Result
	ADC0_ISC_R = 0x0004;							//Acknowledges Completion
	return result;
}

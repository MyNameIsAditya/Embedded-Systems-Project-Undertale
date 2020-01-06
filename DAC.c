// dac.c
// This software configures DAC output
// Lab 6 requires a minimum of 4 bits for the DAC, but you could have 5 or 6 bits
// Runs on LM4F120 or TM4C123
// Program written by: Aditya Khanna and Hubert Ning
// Date Created: 3/6/17 
// Last Modified: 3/12/18 
// Lab number: 6
// Hardware connections
// Headphone Jack Connection For Soundplay and SysTick Handler As Well As Port B (Pins 0 - 5) DAC Output (6-Bit)
// Uses 6 Parallel Lines of Resistors In Combination Of 1, 2, 4, 8, 16, And 32 Ratio
// Port E Is Initialzized For Positive Logic Swithces (Pins 0 - 5)
// 3.3 Voltage Source With Everything Connected To A Common Ground

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data

// **************DAC_Init*********************
// Initialize 6-bit DAC, called once 
// Input: none
// Output: none
void DAC_Init(void) //Initializes Port B Pins 0 - 5 For Dac Output
{
	uint8_t NOP;
	SYSCTL_RCGCGPIO_R |= 0x02; //Enables Clock For Port B, E, and F
	for (NOP=0; NOP<4; NOP++)
	{
	}
	GPIO_PORTB_DIR_R |= 0x3F; //Makes Port B Pins 0-5 Outputs
	GPIO_PORTB_AFSEL_R &= 0xC0; //Disables Alternate Functions
	GPIO_PORTB_AMSEL_R &= 0xC0; //Disables Analog Functions
	GPIO_PORTB_DR8R_R |= 0xFF;
	//We Shall Use External Resistors For The Circuit As Well
	GPIO_PORTB_DEN_R |= 0x3F; //Enables Port B Pins
}

// **************DAC_Out*********************
// output to DAC
// Input: 6-bit data, 0 to 63 
// Output = n is converted to n*3.3V/63
// Output: none
void DAC_Out(uint32_t data) //Inputs 6-Bit DAC Data To Port B To Be Used For Audio Jack Circuitry
{
	//data = ((data*3.3)/63);
	GPIO_PORTB_DATA_R = data;
}

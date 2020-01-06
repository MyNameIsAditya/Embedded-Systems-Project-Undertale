// UART1.c
// Runs on LM4F120/TM4C123
// Use UART1 to implement bidirectional data transfer to and from 
// another microcontroller in Lab 9.  This time, interrupts and FIFOs
// are used.
// Daniel Valvano
// November 17, 2014
// Modified by EE345L students Charlie Gough && Matt Hawk
// Modified by EE345M students Agustinus Darmawan && Mingjie Qiu

/* Lab solution, Do not post
 http://users.ece.utexas.edu/~valvano/
*/

// U1Rx (VCP receive) connected to PC4
// U1Tx (VCP transmit) connected to PC5
#include <stdint.h>
#include "FiFo.h"
#include "UART.h"
#include "tm4c123gh6pm.h"
#define PF3       (*((volatile uint32_t *)0x40025020))

uint32_t DataLost; 
// Initialize UART1
// Baud rate is 115200 bits/sec
// Make sure to turn ON UART1 Receiver Interrupt (Interrupt 6 in NVIC)
// Write UART1_Handler
void UART_Init(void){
   // --UUU-- complete with your code
	SYSCTL_RCGCUART_R |= 0X0002;
	for(int nop = 0; nop <5; nop++){}
	SYSCTL_RCGCGPIO_R |= 0X0004; //PORTC
	for(int nop = 0; nop < 5; nop++){}
	FiFo_Init();
	UART1_CTL_R &= ~0x0001;
	UART1_IBRD_R = 43; // CLOCKRATE(80MHZ) / 16*BAUDRATE(115200) = 43.4027 
	UART1_FBRD_R = 26; //.4027*64 (2^-6) = 25.7728 WHICH ROUNDS TO 26
	UART1_LCRH_R = 0X0070; //ENABLE FEN
	UART1_IFLS_R &= ~0X38; //SET TRIGGERS BASED OFF OF RECIVERFIFO
	UART1_IFLS_R |= 0x10;
	UART1_IM_R |= 0X10;
	NVIC_PRI1_R = (NVIC_PRI1_R &0xFF0FFFFF) | 0X00400000; //set priority to 2
	NVIC_EN0_R |= 0X00000040; //enable interrupt 6
	UART1_CTL_R |= 0X0301;
	GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R & 0xFF00FFFF) + 0X00220000;
	GPIO_PORTC_AMSEL_R &= ~0X30; //PC5(TRANSMITTER) AND PC4(RECIEVER)
	GPIO_PORTC_AFSEL_R |= 0X30;
	GPIO_PORTC_DEN_R |= 0X30;
}

// input ASCII character from UART
// spin if RxFifo is empty
char UART_InChar(void){
	//reciever
	while((UART1_FR_R&0x0010) != 0){}
  return((char)(UART1_DR_R&0xFF)); // --UUU-- remove this, replace with real code
}
//------------UART1_OutChar------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
void UART_OutChar(char data){
  // transmitter
	while((UART1_FR_R&0X0020) != 0){}
		UART1_DR_R = data;
}

// hardware RX FIFO goes from 7 to 8 or more items
// UART receiver Interrupt is triggered; This is the ISR
void UART1_Handler(void){
  // --UUU-- complete with your code
	char msg;
	int RxCounter = 0;
	UART1_ICR_R = 0X10; //	ACK
	while((UART1_FR_R & 0X10) == 0)
	{
		RxCounter++;
		msg = UART_InChar();
		FiFo_Put(msg);					//if statement?
	}
}








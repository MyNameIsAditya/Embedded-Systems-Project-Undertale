// FiFo.c
// Runs on LM4F120/TM4C123
// Provide functions that implement the Software FiFo Buffer
// Last Modified: 4/10/2017 
// Student names: Aditya Khanna and Hubert Ning
// Last modification date: 15 April 2018
/*
#include <stdint.h>
// --UUU-- Declare state variables for FiFo
//        size, buffer, put and get indexes
#define SIZE	16
uint32_t static PutI;
uint32_t static GetI;
uint32_t static FIFO[SIZE];

// *********** FiFo_Init**********
// Initializes a software FIFO of a
// fixed size and sets up indexes for
// put and get operations
void FiFo_Init()
{
	PutI = 0;
	GetI = 0;
}

// *********** FiFo_Put**********
// Adds an element to the FIFO
// Input: Character to be inserted
// Output: 1 for success and 0 for failure
//         failure is when the buffer is full
uint32_t FiFo_Put(char data)
{
	if (((PutI + 1) % SIZE) == GetI)
	{
		return 0;																//Returns 0 If Buffer Is Full
	}
	FIFO[PutI] = data;												//Puts Data In Queue
	PutI = ((PutI + 1) % SIZE);								//Updates Tail Of Queue
	return 1;																	//Successfully Places Data In Queue
}

// *********** FiFo_Get**********
// Gets an element from the FIFO
// Input: Pointer to a character that will get the character read from the buffer
// Output: 1 for success and 0 for failure
//         failure is when the buffer is empty
uint32_t FiFo_Get(char *datapt)
{
	if (PutI == GetI)
	{
		return 0;																//Returns 0 For Empty Buffer
	}
	*datapt = FIFO[GetI];											//Gets Data
	GetI = ((GetI + 1) % SIZE);								//Moves The Head Queue
	return 1;																	//Successfully Retrieved Data
}
*/
// FiFo.c
// Runs on LM4F120/TM4C123
// Provide functions that implement the Software FiFo Buffer
// Last Modified: 4/10/2017 
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly

#include <stdint.h>
// --UUU-- Declare state variables for FiFo
//        size, buffer, put and get indexes
#define SIZE 16
uint32_t static PutI;
uint32_t static GetI;
int32_t static FIFO[SIZE];

// *********** FiFo_Init**********
// Initializes a software FIFO of a
// fixed size and sets up indexes for
// put and get operations
void FiFo_Init() {
// --UUU-- Complete this
	PutI = GetI =0;
}

// *********** FiFo_Put**********
// Adds an element to the FIFO
// Input: Character to be inserted
// Output: 1 for success and 0 for failure
//         failure is when the buffer is full
uint32_t FiFo_Put(char data) {
	// --UUU-- Complete this routine
	if(((PutI+1)%SIZE) == GetI)
		return 0;      //return 0 for failure if the buffer is full
	FIFO[PutI] = data; //put the data in the Queue
	PutI = (PutI+1)%SIZE; //update the 'tail' of the Queue
	return 1; //successfully placed data in queue *isnt it freaky that the word Queue is just the letter 'Q' followed by a silent u,e,u,e.
}

// *********** FiFo_Get**********
// Gets an element from the FIFO
// Input: Pointer to a character that will get the character read from the buffer
// Output: 1 for success and 0 for failure
//         failure is when the buffer is empty
uint32_t FiFo_Get(char *datapt)
{
	//--UUU-- Complete this routine
	if(PutI == GetI)
		return 0; //empty buffer
	*datapt = FIFO[GetI]; //get the data
	GetI = (GetI+1)%SIZE; //move the 'head' of the Queue
  return 1; // successfully retrieved
}











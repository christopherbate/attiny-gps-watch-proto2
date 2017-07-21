//---------------------------------------------------------------------------------------------
// Prototype2.cpp
//
// Created: 5/11/2017 9:09:43 AM
// Author : Christopher Bate
// 
// Includes the program point of entry for our prototype GPS-watch.
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
// MACROS
// System level defines.
//---------------------------------------------------------------------------------------------
#define F_CPU 8000000UL

//---------------------------------------------------------------------------------------------
// Dependencies
//---------------------------------------------------------------------------------------------
#include <stdio.h>
#include <avr/interrupt.h>
#include "SSD1306Driver.h"
#include "SoftUART.h"
#include "WatchClock.h"
#include "TWI.h"

//---------------------------------------------------------------------------------------------
// Dependencies
//---------------------------------------------------------------------------------------------
void DisableInterrupts();
void EnableInterrupts();
void Setup();
			
//---------------------------------------------------------------------------------------------
// Program point of entry
//---------------------------------------------------------------------------------------------
int main(void)
{
	// Setup our program
	Setup();

	// We need these variables to keep track of our update. Update once per second.
	unsigned int lastTime = GetSeconds();
	unsigned int currTime = GetSeconds();
	char longbuff[25]; // Buffer for holding our screen data.
    while (1) 
    {	
		currTime = GetSeconds();
		if(currTime > lastTime )
		{
			ScreenClear();
			WriteText(SecondsString(),0);
			WriteText(GetBufferPointer(),1);
			if( GetGPSStatus() )
			{
				sprintf(longbuff,"%ld",GetLat());
				WriteText(longbuff,2);
			}
			else
			{
				WriteText("NoFix",2);
			}
		
			lastTime = currTime; 
		}
    }
}

//---------------------------------------------------------------------------------------------
// Performs Setup for our Program
//---------------------------------------------------------------------------------------------
void Setup()
{
	// Initiate debug serial.
	// DebugSerialInit();
	
	DisableInterrupts();
	// Setup soft UART bus (external interrupt setup), pin 2
	InitSoftUART(); // Soft UART running on Timer 0
	
	// Setup the clock.
	WatchClockSetup(); // Clock is running on Timer 1
	
	EnableInterrupts();
	
	// Setup IIC bus (USI, pins 0 and 2)
    SetupIIC(); // Running on USI
	
	// Start the screen initialization sequence.
	InitSSD1306();
	
	// Clear the screen.
	ScreenClear();
	
	// Write text to screen.
	//WriteText("I LOVE MARGIE!");
}

void EnableInterrupts()
{
	sei();
}

void DisableInterrupts()
{
	cli();
}

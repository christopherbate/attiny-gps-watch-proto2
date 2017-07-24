//---------------------------------------------------------------------------------------------
// Prototype2.cpp
//
// Created: 5/11/2017 9:09:43 AM
// Author : Christopher Bate
// 
// Includes the program point of entry for our prototype GPS-watch.
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
// System Explanation
/*
Pin1 - Reset, unutilized during operation
Pin2 - XTAL1 - connected to external ceramic oscillator (8Mhz)
Pin3 - XTAL2 - connected to external ceramic oscillator 
Pin4 - Ground
Pin5 - Serial Data line for IIC - connected to open high drain via 10k resistor 
Pin6 - Initialized as a general output, used to bit-bang a UART input buffer
Pin7 - Serial Clock line for IIC - connected to open high drain via 10k resistor
Pin8 - VCC - connected to 3.3v LiPo Battery

Watch Clock - WatchClock.h, WatchClock.cpp
Runs a basic clock via Timer1. It has accuracy dictated by the ceramic oscillator. In future versions 
we could correct this with the GPS periodically. 

I2C (Master) Interface - IIC_Interface.h, IIC_Interface.cpp
Runs a basic I2C interface utilizing the USI hardware buffer present on the ATTiny85. This is used for
communicating with the SSD1306 display via the SSD1306 Driver. Because we only need to send commands to
the display, the interface only implements the master interface, no slave functionality. Unlike the soft
UART implemented for communication with the GPS, this interface utilizes sleep functionality to get the timing right. 
ATTiny85 only has two timers available for interrupt driven programming. Luckily, we are only sending data to the screen once per second.

SSD1306 Driver - SSD1306Driver.h, SSD1306Driver.cpp
Contains all the necessary code to drive the SSD1306 display using the I2C interface. All code in this interface was
kept as minimal as possible. Supports both 128x32 and 64x32 displays. 


*/
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
// MACROS
// System level defines.
//---------------------------------------------------------------------------------------------
#define F_CPU 8000000UL // We are running at 8 Mhz 

//---------------------------------------------------------------------------------------------
// Dependencies
//---------------------------------------------------------------------------------------------
#include <stdio.h>
#include <avr/interrupt.h>
#include "SSD1306Driver.h"
#include "SoftUART.h"
#include "WatchClock.h"
#include "IIC_Interface.h"

//---------------------------------------------------------------------------------------------
// Function Declarations
//---------------------------------------------------------------------------------------------
void Setup();
			
//---------------------------------------------------------------------------------------------
// Program point of entry
//---------------------------------------------------------------------------------------------
int main(void)
{
	// Setup our program
	Setup();

	// We need these variables to keep track of our update. Update once per second.
	unsigned int lastTime = WatchClock_GetSeconds();
	unsigned int currTime = WatchClock_GetSeconds();
	
	char longbuff[25]; // Buffer for holding our screen data.
    while (1) 
    {	
		currTime = WatchClock_GetSeconds();
		if(currTime > lastTime || ((currTime == 0) && (lastTime==59)) )
		{
			ScreenClear();
			WriteText(WatchClock_GetTimeString(),0);
			sprintf(longbuff,"Num msgs recv'd:%d",GPS_getNumMSG());
			WriteText(longbuff,1);
			if( GetGPSStatus() )
			{
				sprintf(longbuff,"Status: Fixed");
				WriteText(longbuff,2);
			}
			else
			{
				WriteText("Status: No Fix",2);
			}
			sprintf(longbuff,"Lat:");
			WriteText(longbuff,3);
			sprintf(longbuff,"Long:");
			WriteText(longbuff,4);
		
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
	
	// Disable interrupts for startup
	cli();
	
	// Setup soft UART bus (external interrupt setup), pin 2
	InitSoftUART(); // Soft UART utilizing Timer0
	
	// Setup the clock
	WatchClock_Init(13,0); // Clock function utilizing Timer1
	
	// Re-enable interrupts.
	sei();
	
	// Setup IIC bus (USI, pins 0 and 2)
    IIC_Init(); 
	
	// Start the screen initialization sequence.
	SSD1306_Init();
	
	// Clear the screen.
	ScreenClear();
}
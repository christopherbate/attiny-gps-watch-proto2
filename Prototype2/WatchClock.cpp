//---------------------------------------------------------------------------------------------
// WatchClock.h
//
// Created: 5/11/2017 9:09:43 AM
// Author : Christopher Bate
// Description: Provides the necessary interface to setup and run a clock.
// Requirements:
//---------------------------------------------------------------------------------------------
#include "WatchClock.h"
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

//---------------------------------------------------------------------------------------------
// Clock Settings
//---------------------------------------------------------------------------------------------
#define TIME_STR_MAX_LENGTH 20 // This should depend on your screen length and font pixel width

//---------------------------------------------------------------------------------------------
// Globals
//---------------------------------------------------------------------------------------------
volatile unsigned int msCount = 0;
volatile unsigned int secCount = 0;
volatile unsigned int minCount = 0;
volatile unsigned int hrsCount = 0;
char timeString[TIME_STR_MAX_LENGTH];

//---------------------------------------------------------------------------------------------
// Interrupt for the Timer1 CompA
// The purpose of this interrupt is to update time to the nearest millisecond.
//---------------------------------------------------------------------------------------------
ISR(TIMER1_COMPA_vect)
{
	TCNT1 = 0;
	msCount +=1;
	if(msCount == 1000)
	{
		msCount = 0;
		secCount += 1;
		if(secCount == 60)
		{
			secCount = 0;
			minCount += 1;
			if(minCount == 60)
			{
				minCount = 0;
				hrsCount += 1;
				if(hrsCount == 24)
				{
					hrsCount = 0;
				}
			}
		}
	}
}

//---------------------------------------------------------------------------------------------
// WatchClockSetup - Main API Function
// Initializes the timer and time-keeping variables
// Requirement: an initial hour and minute. If the hour is greater than 24, it will be set to 0. 
//				if the initMin is greater than 60, it will be set to zero. 
// Output: True if all succeeded 
//---------------------------------------------------------------------------------------------
bool WatchClock_Init( unsigned int initHour, unsigned int initMin)
{
	// Init time-keeping variables
	msCount = secCount = 0;
	hrsCount = initHour;
	minCount = initMin;
	
	TCCR1 |= (1<<CS12)|(1<<CS11)|(1<<CS10); // Setup the timer 
	
	TIMSK |= (1<<OCIE1A);
	
	TCNT1 = 0;
	
	OCR1A = 125; // Update every 125 ticks, which at this scale is 1 milisecond. 
	
	return true;
}

//---------------------------------------------------------------------------------------------
// GetTimeString - Main API Function
// Returns the string for the time formated in "HR:MIN:SEC"
// Requirement: 
// Output: As stated above.
//---------------------------------------------------------------------------------------------
char *WatchClock_GetTimeString()
{
	sprintf(timeString,"%d:%d:%d",hrsCount,minCount,secCount);
	return timeString;
}

//---------------------------------------------------------------------------------------------
// GetTimeStringLength - Main API Function
// Returns the length of the time string buffer
// Requirement: 
//---------------------------------------------------------------------------------------------
unsigned int WatchClock_GetTimeStringLength()
{
	return TIME_STR_MAX_LENGTH;
}

unsigned int WatchClock_GetSeconds()
{
	return secCount;
}
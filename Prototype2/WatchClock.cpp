/*
 * WatchClock.cpp
 *
 * Created: 5/20/2017 2:09:40 PM
 *  Author: Chris
 */ 
#include "WatchClock.h"
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

#define TIME_STR_MAX_LENGTH 20

unsigned int msCount = 0;
unsigned int secCount = 0;
unsigned int minCount = 0;
unsigned int hrsCount = 0;

char timeString[TIME_STR_MAX_LENGTH];

ISR(TIMER1_COMPA_vect)
{
	TCNT1 = 0;
	msCount +=1;
	if(msCount == 1000)
	{
		msCount = 0;
		secCount += 1;
	}
}

bool WatchClockSetup()
{
	msCount = 0;
	
	TCCR1 |= (1<<CS12)|(1<<CS11)|(1<<CS10);
	
	TIMSK |= (1<<OCIE1A);
	
	TCNT1 = 0;
	
	OCR1A = 125;
	
	return true;
}
char *SecondsString()
{
	return itoa(secCount,timeString,10);
}

unsigned int GetSeconds()
{
	return secCount;
}
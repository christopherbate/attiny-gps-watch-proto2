//---------------------------------------------------------------------------------------------
// WatchClock.h
//
// Created: 5/11/2017 9:09:43 AM
// Author : Christopher Bate
// Description: Provides the necessary interface to setup and run a clock.
// Requirements:
//---------------------------------------------------------------------------------------------


#ifndef WATCHCLOCK_H_
#define WATCHCLOCK_H_

//---------------------------------------------------------------------------------------------
// Main API Function Definitions for the Watch Clock
//---------------------------------------------------------------------------------------------
bool WatchClock_Init( unsigned int initHour, unsigned int initMin);
char* WatchClock_GetTimeString(); 
unsigned int WatchClock_GetTimeStringLength(); 
unsigned int WatchClock_GetSeconds();

#endif /* WATCHCLOCK_H_ */
//---------------------------------------------------------------------------------------------
// DebugSerial.h
//
// Created: 5/11/2017 9:09:43 AM
// Author : Christopher Bate
//
// Provides all necessary variables and functions for a software serial output. 
//
// Initialization Requirements: Defaults to pin 1. If another pin is used, define "SER_BIT" as the number of the pin. e.e "#define SER_BIT 0" for pin 0. 
//								Must call "DebugSerialInit()" otherwise all functions will do nothing.
//								
//---------------------------------------------------------------------------------------------


#ifndef DEBUGSERIAL_H_
#define DEBUGSERIAL_H_

#include <stdio.h>

//---------------------------------------------------------------------------------------------
// Function prototypes
//---------------------------------------------------------------------------------------------
void MDebug( const char msg[] );
void DebugSerialInit();
void DebugSerialWriterBangOneByte( uint8_t value );

#endif /* DEBUGSERIAL_H_ */
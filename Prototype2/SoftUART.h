//---------------------------------------------------------------------------------------------
// SoftUART.h
//
// Created: 5/11/2017 9:09:43 AM
// Author : Christopher Bate
//
// Includes the variables and functions needed to setup a receive-only software UART that interfaces
// with a GPS module.
//
// Initialization Requirements: #define UART_PIN to set the rx pin. Defaults to 1.
//								
//---------------------------------------------------------------------------------------------

#ifndef SOFTUART_H_
#define SOFTUART_H_

//---------------------------------------------------------------------------------------------
// Defines
//---------------------------------------------------------------------------------------------
#define UART_PIN 1

//---------------------------------------------------------------------------------------------
// Function Prototypes
//---------------------------------------------------------------------------------------------
bool InitSoftUART();
void ResetBufferState();
unsigned int GetBufferLength();
unsigned int GetBufferIndex();
char *GetBufferPointer();
bool BufferHasData();
bool HaveFix();

//---------------------------------------------------------------------------------------------
// Function Prototypes - GPS Access
//---------------------------------------------------------------------------------------------
long GetLat();
long GetLong();
unsigned long GetTime();
bool GetGPSStatus();
unsigned int GPS_getNumMSG();


#endif /* SOFTUART_H_ */
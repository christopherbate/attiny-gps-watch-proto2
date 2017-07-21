//---------------------------------------------------------------------------------------------
// DebugSerial.cpp
//
// Created: 5/11/2017 9:09:43 AM
// Author : Christopher Bate
//
// Provides all necessary variables and functions for a software serial output.
//
// Initialization Requirements: Defaults to pin 1. If another pin is used, define .....
//								Must call "DebugSerialInit()" otherwise all functions will do nothing.
//---------------------------------------------------------------------------------------------
#include "DebugSerial.h"
#include <string.h>
#include <stdio.h>

// Serial Debug output (pin 1) information
#define SER_REG 0x18 // Send serial out port b
#ifndef SER_BIT
	#define SER_BIT 2 // send serial debug output out pin 1
#endif 

//These are settings for a 8 MHz clock
const uint8_t lom =  0b00101;
const uint8_t him =  0b00100;
const uint8_t oloops = 3;
const uint8_t iloops = 89;
const uint8_t nops = 0;

/* These are settings for a 1 Mhz clock 
const uint8_t lom =  0b00100;
const uint8_t him =  0b00010;
const uint8_t oloops = 0;
const uint8_t iloops = 28;
const uint8_t nops = 2;*/

bool sd_init = false;

//---------------------------------------------------------------------------------------------
// MDebug 
// Description: Sends the specified char array out the output pin via software serial transmission.
// Requirements: msg must be null terminated 
//---------------------------------------------------------------------------------------------
void MDebug( const char msg[])
{
	if( sd_init )
	{
		for(unsigned int i = 0; i < strlen(msg); i++)
		{
			DebugSerialWriterBangOneByte(msg[i]);
		}
	}
}

//---------------------------------------------------------------------------------------------
// DebugSerialInit()
// Description: Initiates the debug serial by storing the defines into registers. If already initiated
//				does nothing.
// Requirements: none.
//---------------------------------------------------------------------------------------------
void DebugSerialInit()
{
	if( sd_init )
	{
		return;
	}
	
	asm volatile
	(
	"sbi   %[serreg]-1, %[serbit]"            "\n\t"
	"sbi   %[serreg], %[serbit]"              "\n\t"
	:
	:
	[serreg] "I" ( SER_REG ),
	[serbit] "I" ( SER_BIT )
	:
	);
	
	sd_init = true;
}

//---------------------------------------------------------------------------------------------
// DebugSerialInit()
// Description: "Bit-Bangs" the byte out the software serial port.
// Requirements: DebugSerialInit must have been called in order to set "sd_init" to true.
//				Otherwise, does nothing.
//---------------------------------------------------------------------------------------------
void DebugSerialWriterBangOneByte( uint8_t value )
{
	if(!sd_init)
	{
		return;
	}
	
	uint8_t i;
	uint8_t j;
	uint8_t ol;
	uint8_t il;
	uint8_t b;  // Initialized to the low bits
	uint8_t hib;
	uint8_t m;
	
	b   = ((value << 1) & 0x1F);
	hib = ((value >> 4) & 0x1F) | 0x10;
	
	asm volatile
	(
	"ldi   %[j], 2"                           "\n\t"
	"ldi   %[i], 5"                           "\n\t"
	"ldi   %[m], %[lom]"                      "\n\t"

	// Note: 8 MHz, 9600 baud ---> disabling interrupts does not appear to be necessary

	"cli"                                     "\n\t"

	"rjmp  L%=ntop"                           "\n\t"

	"L%=btop: "
	"nop"                                     "\n\t"      // ---> 7
	"nop"                                     "\n\t"      //
	"nop"                                     "\n\t"      //
	"nop"                                     "\n\t"      //
	"nop"                                     "\n\t"      //
	"nop"                                     "\n\t"      //
	"nop"                                     "\n\t"      //

	"L%=ntop: "
	"ror   %[b]"                              "\n\t"      // ---> 1
	
	"brcs  L%=bxh"                            "\n\t"      // 1  (not taken)
	"cbi   %[serreg], %[serbit]"              "\n\t"      // 2
	"rjmp  L%=bxz"                            "\n\t"      // 2
	
	"L%=bxh: "                                              // 2  (taken)
	"sbi   %[serreg], %[serbit]"              "\n\t"      // 2
	"nop"                                     "\n\t"      // 1

	// ---> 5
	"L%=bxz: "

	"ror   %[m]"                              "\n\t"      // ---> 3 or 4
	"brcc  L%=bnoe"                           "\n\t"      //
	"nop"                                     "\n\t"      //
	"nop"                                     "\n\t"      //
	"L%=bnoe: "

	// ---> 1
	".if %[oloops] >= 1"                        "\n\t"      // if oloops >= 1 then...
	"ldi   %[ol], %[oloops]"                  "\n\t"      // 4*oloops + oloops*(3*iloops) or oloops*((3*iloops)+4)
	"L%=odelay: "                               "\n\t"
	".endif"                                    "\n\t"
	"ldi   %[il], %[iloops]"                  "\n\t"      // if oloops == 0 then...
	"L%=idelay: "                               "\n\t"      // (3*iloops)
	"dec   %[il]"                             "\n\t"
	"brne  L%=idelay"                         "\n\t"
	"nop"                                     "\n\t"
	".if %[oloops] >= 1"                        "\n\t"
	"dec   %[ol]"                             "\n\t"
	"brne  L%=odelay"                         "\n\t"
	"nop"                                     "\n\t"
	".endif"                                    "\n\t"

	".if %[nops] >= 1"                          "\n\t"
	"nop"                                     "\n\t"      //
	".endif"                                    "\n\t"
	".if %[nops] >= 2"                          "\n\t"
	"nop"                                     "\n\t"      //
	".endif"                                    "\n\t"

	"dec   %[i]"                              "\n\t"      // ---> 3
	"brne  L%=btop"                           "\n\t"      //
	"nop"                                     "\n\t"      //

	"dec   %[j]"                              "\n\t"      // ---> 7
	"breq  L%=bfin"                           "\n\t"      //
	"ldi   %[i], 5"                           "\n\t"      //
	"mov   %[b], %[hib]"                      "\n\t"      //
	"ldi   %[m], %[him]"                      "\n\t"      //
	"rjmp  L%=ntop"                           "\n\t"      //

	"L%=bfin: "

	"sei"                                     "\n\t"
	:
	[i] "=&r" ( i ),
	[j] "=&r" ( j ),
	[ol] "=&r" ( ol ),
	[il] "=&r" ( il ),
	[m] "=&r" ( m )
	:
	[b] "r" ( b ),
	[hib] "r" ( hib ),
	[serreg] "I" ( SER_REG ),
	[serbit] "M" ( SER_BIT ),
	[lom] "M" ( lom ),
	[him] "M" ( him ),
	[oloops] "M" ( oloops ),
	[iloops] "M" ( iloops ),
	[nops] "M" ( nops )
	:
	"r31",
	"r30"
	);
	
}
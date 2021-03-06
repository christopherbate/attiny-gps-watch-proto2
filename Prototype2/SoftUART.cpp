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
#include "SoftUART.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "DebugSerial.h"

//---------------------------------------------------------------------------------------------
// Global definitions for the driver
//---------------------------------------------------------------------------------------------
#define MAX_UART_BUFFER_SIZE 25
char   softUARTBuffer[MAX_UART_BUFFER_SIZE+1];
volatile uint8_t	    workingChar = 0;
volatile uint8_t	bufferIndex = 0;
volatile uint8_t    currPassNum = 0;
volatile uint8_t	sampleNum = 4;
volatile uint8_t    workingCharMask = 0;
volatile bool halfDelay = false;
volatile bool fullDelay = false;
volatile bool haveData = false;
volatile bool bufferFull = false;
volatile bool waitForStart = false;
volatile bool waitForStop = false;


//---------------------------------------------------------------------------------------------
// Global definitions for the GPS driver
//---------------------------------------------------------------------------------------------
volatile bool hasFix = false; 
volatile uint8_t parsePos = 0; 
long lat, lon; 
unsigned long time1, date, fix_age; 
volatile unsigned int numMsgs = 0; 
volatile unsigned int numGPRMC_Msgs = 0; 
const char GPRMC_keyword[] = "GPRMC,,,,,,,,,,,";
enum GPS_State {SEARCHING, KEYWORD,TIME,VALIDITY,LAT,NS,LONG,EW,SPEED,COURSE,STAMP,VAR,EW2,CHECKSUM};
volatile GPS_State gpsState = SEARCHING;

//---------------------------------------------------------------------------------------------
// Global definitions for the driver
//---------------------------------------------------------------------------------------------
long GetLat() {return lat;}
long GetLong() {return lon; }
unsigned long GetTime() {return time1;}
bool GPS_getStatus() { return hasFix; }

//---------------------------------------------------------------------------------------------
// Takes the currently received character and sends it to the GPS Parser (TinyGPS)
//---------------------------------------------------------------------------------------------
void ParseGPS( char current )
{
	/*if( current == '$' )
	{
		parsePos = 0;
		return;
	}
	parsePos +=1;
	if(current == GPRMC[parsePos])
	{
			
	}*/
	
}

//---------------------------------------------------------------------------------------------
// Interrupt Routine for the Timer 0 Compare Match A
// Interrupts at 3x Baud rate of 9600. 
// Actually does the work of receiving the bits on the RX pin, putting hte bit in the currect position for the current byte.
// and then sending the completed byte to the GPS Parser
// Requirements: none 
//---------------------------------------------------------------------------------------------
ISR(TIMER0_COMPA_vect)
{		
	if( waitForStop )
	{		
		if( --sampleNum == 0 ) // If sampleNum is zero, that means we have read in a new byte (char)
		{
			
			// UART State variables 
			waitForStop = false;
			waitForStart = true;
			
			// GPS Parsing Section
			if(workingChar == '$')
			{
				numMsgs +=1;
				parsePos = 0;
				gpsState = KEYWORD;
			}
			else if (gpsState != SEARCHING)
			{
				switch( gpsState ) // This means we are in the "GPRMC part"
				{
					case KEYWORD:
						if(GPRMC_keyword[parsePos] == workingChar)
						{
							parsePos++;
							if(workingChar==',') // We have received a "GPRMC,"
							{
								parsePos = 0;
								gpsState = TIME;
								numGPRMC_Msgs++;
							}
						}
						else
						{
							gpsState = SEARCHING;
						}
						break;
						
					case TIME:
						if(workingChar==',')
						{
							gpsState = VALIDITY;
						}
						break;
						
					case VALIDITY:
						if(workingChar == ',')
						{
							gpsState = SEARCHING;
						}
						else if (workingChar =='V')
						{
							hasFix = false;
						}
						else if (workingChar == 'A')
						{
							hasFix = true;
						}
						break;			
						
					case LAT:
						break;
						
					case NS:
						break;
						
					case LONG:
						break;
						
					case EW:
						break;
						
					case SPEED:
						break;
						
					case COURSE:
						break;
							
						
					default:
						break;
				}
			}
			// enum GPS_State {SEARCHING, KEYWORD,TIME,VALIDITY,LAT,NS,LONG,EW,SPEED,COURSE,STAMP,VAR,EW2,CHECKSUM};

							
			// Internal software UART buffer section 
			softUARTBuffer[bufferIndex] = workingChar;
			bufferIndex += 1;
			if( bufferIndex == MAX_UART_BUFFER_SIZE )
			{
				bufferIndex = 0;
			}
		}
	}
	else
	{
		if(waitForStart)
		{
			// Start == low.
			if(!( PINB & (1<<UART_PIN)) )
			{
				waitForStart = false;
				workingChar = 0;
				sampleNum = 4; // We initially wait 4x ticks before sampling, for all other bits we wait 3x ticks
				currPassNum = 0;
				softUARTBuffer[bufferIndex] = 0;
				workingCharMask = 1;
			}
		}
		else
		{
			uint8_t temp = sampleNum-1;
			if( temp == 0)
			{
				// Reset pass number;
				temp = 3;
				
				// Set the ones...
				if( PINB & (1<<UART_PIN) ) //logical 1
				{
					workingChar |= workingCharMask;
				}
				
				// Change the mask.
				workingCharMask<<=1;
		
				// Increment bit position.
				currPassNum += 1;
				if(currPassNum == 8)
				{
					// We will be on the stop bit next 
					waitForStop = true;
				}
			}
			sampleNum = temp;
		}
	}
}


//---------------------------------------------------------------------------------------------
// Initializes the SOFT Uart and begins checking for a start signal.
// Requirements: Timer0 is not being used for anything else.
//				 "UART_PIN" must be defined 
//				"MAX_UART_BUFFER"SIZE" must be defined.
//---------------------------------------------------------------------------------------------
bool InitSoftUART()
{
	// Setup uart pin as input;
	DDRB &= ~(1<<UART_PIN);
	
	// Setup the buffer
	softUARTBuffer[MAX_UART_BUFFER_SIZE] = '\0'; // String termination
	bufferIndex = 0; // For debug purposes
	haveData = false;
	bufferFull = false;
	waitForStop = false;
	waitForStart = true;
	sampleNum = 4;
	currPassNum = 0;
	
	// Setup gps info
	hasFix = false;
	parsePos = 0;
	numMsgs = 0;
	numGPRMC_Msgs = 0;
	
	// Reset Timer0 to put us half a bit in.
	TCCR0A |= (1<<WGM01); // CTC mode
	TCCR0B |= (1<<CS01); // Over 8 pre-scale 
	OCR0A = 33;
	TCNT0 = 0; // Reset  timer
	TIFR |= 1<<OCF0A;
	TIMSK |= (1<<OCIE0A);
	
	return true;
}

char *GetBufferPointer()
{
	return softUARTBuffer;
}

bool HaveFix()
{
	return hasFix;
}

unsigned int GPS_getNumMSG()
{
	return numMsgs;
}

unsigned int GPS_getNumRMC_Msg()
{
	return numGPRMC_Msgs;
}
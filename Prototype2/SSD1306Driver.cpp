//---------------------------------------------------------------------------------------------
// SSD1306Driver.h
//
// Created: 5/11/2017 9:09:43 AM
// Author : Christopher Bate
//
// Provides all necessary variables and functions necessary to initialize, clear, and draw text
// to the SSD1306 driver.
//
// Initialization Requirements: Define "SSD1306_128X32_MODE" for 128x32. Otherwise 128x64 is assumed.
//								Must call "SSD1306_Init" or all of the other functions will just return.
//---------------------------------------------------------------------------------------------

#include <avr/io.h>
//#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>

#include "SSD1306Driver.h"
#include "TWI.h"
#include "DebugSerial.h"
#include "Font5x7.h"


const unsigned char screenAddr = (SCRADR<<SLAVEADRBITPOS) | MASTERSEND;
unsigned char initSeq[] = {SSD1306_SLEEP,
	SSD1306_SET_CLOCK, SSD1306_CLOCK_DEF,
	SSD1306_SET_MUX, SSD1306_MUX_DEF,
	SSD1306_SET_VERTSHIFT, 0x00,
	SSD1306_SET_VCOM_DESELECT, SSD1306_VCOM_DEF,
	SSD1306_SET_PRECHARGE, SSD1306_PRECHARGE_DEF,
	SSD1306_NO_SEGREMAP,
	SSD1306_NOINVERSE,
	SSD1306_SET_CHARGEPUMP,SSD1306_CP_DEF,
	SSD1306_SET_CONTRAST, 0x8F,
	SSD1306_SET_COMPINS, SSD1306_COMPINS_DEFAULT,
	SSD1306_SET_MEM_ADDR_MODE, SSD1306_PAGE_ADDR,
	SSD1306_DISPLAYON_FRAM,SSD1306_WAKE};
	
void SetPageAddress( uint8_t offset )
{
	offset = 0xB0 | offset;
	ScreenCMD(offset);
}

void SetColAddress( uint8_t offset )
{
	uint8_t lowerNibble = offset & 0x0F;
	uint8_t upperNibble = (((offset>>4) & (0b00011111)) | (1<<4)); 
	ScreenCMD(lowerNibble);
	ScreenCMD(upperNibble);
}

void WriteMultiLine( const char msg[] )
{
	uint8_t strSize = strlen(msg);
	uint8_t currPos = 0;
	if( strSize > 100 )
		strSize = 100;
	for( uint8_t i = 0; i < (strSize/25); i++)
	{
		SetPageAddress(i);
		SetColAddress(0);
		SendStartIIC(screenAddr);
		SendByteIIC(DCMD);
		for( uint8_t colPos = 0; colPos < 25; colPos++)
		{
			if(currPos == strSize)
			{
				break;
			}
			
			if( msg[currPos] == ' ' )
			{
				for( uint8_t charIndex = 0; charIndex < 5; charIndex++)
				{
					SendByteIIC(0x00);
				}
			}
			else
			{
				for( uint8_t charIndex = 0; charIndex < 5; charIndex++)
				{
					uint8_t charSlice = pgm_read_byte(&flash_font[((msg[currPos] - 0x21) * 5) + charIndex]);
					SendByteIIC(charSlice);
				}
			}
			currPos +=1;
		}
		
		if(currPos == strSize)
		{
			break;
		}
		SendStopIIC();
	}
}

void WriteText( const char msg[], uint8_t line)
{
	SetPageAddress(line);
	SetColAddress(0);
	SendStartIIC(screenAddr);
	SendByteIIC(DCMD);
	uint8_t msglen = strlen(msg);
	if(msglen > 25 )
		msglen = 25;
	for(uint8_t i = 0; i < msglen; i++ )
	{
		if( msg[i] == ' ' )
		{
			for( uint8_t charIndex = 0; charIndex < 5; charIndex++)
			{
				SendByteIIC(0x00);
			}
		}
		else
		{
			for( uint8_t charIndex = 0; charIndex < 5; charIndex++)
			{
				uint8_t charSlice = pgm_read_byte(&flash_font[((msg[i] - 0x21) * 5) + charIndex]);
				SendByteIIC(charSlice);
			}
		}
	}
	SendStopIIC();
}

bool InitSSD1306()
{
	for(unsigned int i = 0; i < 23; i++)
	{
		ScreenCMD(initSeq[i]);
	}
	
	return true;
}

void ScreenCMD( unsigned char msg )
{
	SendStartIIC(screenAddr);
	SendByteIIC(CCMD);
	SendByteIIC(msg);
	SendStopIIC();
}

void ScreenData( unsigned char msg )
{
	SendStartIIC(screenAddr);
	SendByteIIC(DCMD);
	SendByteIIC(msg);
	SendStopIIC();
}

void ScreenClear()
{
	SetPageAddress(0);
	SetColAddress(0);
	
	for (uint8_t i=0; i<=SSD1306_LAST_PAGE; i++)
	{
		SetPageAddress(i);
		SetColAddress(0);
		// send a bunch of data in one xmission
		SendStartIIC(screenAddr);
		SendByteIIC(DCMD);            // data mode
		for (uint8_t k=0;k<=SSD1306_LAST_COL;k++){
			SendByteIIC(0x00);
		}
		SendStopIIC();
	}
}



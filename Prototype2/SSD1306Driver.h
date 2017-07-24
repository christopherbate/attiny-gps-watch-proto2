//---------------------------------------------------------------------------------------------
// SSD1306Driver.h
//
// Created: 5/11/2017 9:09:43 AM
// Credit: This driver was informed by various open source drivers floating around the Internet, including
// the Tinysaur SSD1306xLED driver and the Adafruit SSD1306 library. 
// Working with the SSD1306 Taiwanese factor manuals floating around and the above libraries, I created this
// minimal driver for driving the display with text in this project.
//
// Note that this driver initializes the display in page-addressing mode (see SSD1306 manual). 
//
// Provides all necessary variables and functions necessary to initialize, clear, and draw text
// to the SSD1306 driver.
// 
// Initialization Requirements: Define "SSD1306_128X32_MODE" for 128x32. Otherwise 128x64 is assumed.
//								Must call "SSD1306_Init" or all of the other functions will just return.
//---------------------------------------------------------------------------------------------
#ifndef SSD1306DRIVER_H_
#define SSD1306DRIVER_H_

// Commands for display - general control
// DC Bit - 0
#define SSD1306_SET_CONTRAST 0x81    // Follows with command 0xA[7:0]
#define SSD1306_DISPLAYON_FRAM 0xA4 
#define SSD1306_DISPLAYON_IRAM 0xA5 
#define SSD1306_NOINVERSE 0xA6 
#define SSD1306_INVERSE 0xA7 
#define SSD1306_SLEEP 0xAE 
#define SSD1306_WAKE 0xAF 

// Commands for configuration
#define SSD1306_NO_SEGREMAP 0xA0
#define SSD1306_SET_MUX 0xA8
#ifdef SSD1306_128X32_MODE
	#define SSD1306_MUX_DEF 0x1F
#else
	#define SSD1306_MUX_DEF 0x3F
#endif
#define SSD1306_SET_COM_NORMAL 0xC0 
#define SSD1306_SET_VERTSHIFT 0xD3
#define SSD1306_SET_COMPINS 0xDA
#ifdef SSD1306_128X32_MODE
	#define SSD_1306_COMPINS_DEFAULT 0x02
#else
	#define SSD1306_COMPINS_DEFAULT 0x12
#endif 
#define SSD1306_SET_CHARGEPUMP 0x8D
#define SSD1306_CP_DEF 0x14

// Commands for timing and driving
#define SSD1306_SET_CLOCK 0xD5
#define SSD1306_CLOCK_DEF 0x80
#define SSD1306_SET_PRECHARGE 0xD9
#define SSD1306_PRECHARGE_DEF 0xF1
#define SSD1306_SET_VCOM_DESELECT 0xDB
#define SSD1306_VCOM_DEF 0x40

// Commands for display -- addressing
// DC Bit - 0
#define SSD1306_SET_MEM_ADDR_MODE 0x20 
#define SSD1306_HORIZONTAL_ADDR 0x00 
#define SSD1306_PAGE_ADDR 0x02
#define SSD1306_SETCOL_STARTEND 0x21 
#define SSD1306_SETPAGE_STARTEND 0x22 


// Display boundary constants
#define SSD1306_LAST_COL 0x7F // 127
#define SSD1306_FIRST_COL 0x00 
#ifdef SSD1306_128X32_MODE
	#define SSD1306_LAST_COL 0x03
#else
	#define SSD1306_LAST_PAGE 0x07 // 3
#endif 
#define SSD1306_FIRST_PAGE 0x00

// TWI Communication specific protocols.
#define SCRADR 0b0111100
#define DCMD 0x40
#define CCMD 0x00

//---------------------------------------------------------------------------------------------
// API - Initialization
//---------------------------------------------------------------------------------------------
bool SSD1306_Init();

//---------------------------------------------------------------------------------------------
// API - Command wrappers
// These should probably be "private"
//---------------------------------------------------------------------------------------------
void ScreenCMD( unsigned char msg );
void ScreenData( unsigned char msg );

//---------------------------------------------------------------------------------------------
// API - Screen Function
// These should be "public"
//---------------------------------------------------------------------------------------------
void ScreenClear();
void WriteText( const char msg[], uint8_t line );
//void WriteMultiLine( const char msg[] );
void SetPageAddress(  uint8_t offset );
void SetColAddress( uint8_t offset );

#endif /* SSD1306DRIVER_H_ */
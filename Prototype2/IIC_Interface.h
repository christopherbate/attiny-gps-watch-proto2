//---------------------------------------------------------------------------------------------
// IIC_Interface.h
//
// Created: 5/11/2017 9:09:43 AM
// Author : Christopher Bate
// BASED ON THE ATMEL AVR APPLICATION NOTE AVR310
//
// Provides all the necessary functions to initialize, and communicate as an IIC Master on pins 5 and 7 usin
// the USI
//
// Requirements: 
//---------------------------------------------------------------------------------------------
#ifndef TWI_H_
#define TWI_H_

//---------------------------------------------------------------------------------------------
// IIC Interface defines
//---------------------------------------------------------------------------------------------
#define TWI_LOW    2 		// >4,7us // low clock period minimum TWI standard
#define TWI_HIGH    1 		// >4,0us // high clock period minimum TWI standard
#define DDR_USI             DDRB
#define PORT_USI            PORTB
#define PIN_USI             PINB
#define PORT_USI_SDA        PORTB0
#define PORT_USI_SCL        PORTB2
#define PIN_USI_SDA         PINB0
#define PIN_USI_SCL         PINB2
#define DATAPIN 0
#define CLOCKPIN 2
#define READBITPOS 0 
#define SLAVEADRBITPOS 1
#define MASTERSEND 0
#define MASTERREC 1

//---------------------------------------------------------------------------------------------
// API Prototypes
//---------------------------------------------------------------------------------------------
void IIC_Init();
bool SendStartIIC( unsigned char addr);
bool SendByteIIC( unsigned char msg);
void SendStopIIC();

#endif /* TWI_H_ */
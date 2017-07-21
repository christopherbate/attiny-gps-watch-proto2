/*
 * TWI.h
 *
 * Created: 5/17/2017 9:31:53 AM
 *  Author: Chris
 */ 


#ifndef TWI_H_
#define TWI_H_

// TWI serial defines
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

// Function Prototypes
void SetupIIC();
bool SendStartIIC( unsigned char addr);
bool SendByteIIC( unsigned char msg);
void SendStopIIC();

#endif /* TWI_H_ */
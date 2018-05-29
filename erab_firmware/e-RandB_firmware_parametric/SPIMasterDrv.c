/*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author            	Date      Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Alvaro Gutierrez		09/02/2007	  First release of source file
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* ADDITIONAL NOTES: 
* Code For the Epuck Range and Bearing Board
**********************************************************************/
#include "p33FJ256GP710.h"
#include "SPIMasterDrv.h"

//Functions prototype
void init_SPI( void ){
	
	/* SDI1 INPUT */
	TRISFbits.TRISF7 = 1;
	/* SDO1 OUTPUT */
	TRISFbits.TRISF8 = 0;
	/* SS1 OUTPUT */
	TRISBbits.TRISB2 = 0;

	/* Disable SPI1 module */
	SPI2STAT = 0x0;  
	/* Internal SPI CLOCK enable */
	SPI1CON1bits.DISSCK = 0;
	/* SDOO controlled by the module */
	SPI1CON1bits.DISSDO = 0;
	/* Communication is a byte wide (8 bits) */
	SPI1CON1bits.MODE16 = 0;
	/* Input data sampled at middle of data output time */
	SPI1CON1bits.SMP = 0;
	/* Serial output data changes on transition from active clock state to Idle clock state*/
	SPI1CON1bits.CKE = 1;
	/* Chip select SS1 controlled by port functions */
	SPI1CON1bits.SSEN = 0;
	/* Idle state for clock is a low level; active state is a high level */
	SPI1CON1bits.CKP = 0;
	/* Master mode ON*/
	SPI1CON1bits.MSTEN = 1;
	/* secondary prescale */
	SPI1CON1bits.SPRE = 0b000;
	/* primary preescale */
	SPI1CON1bits.PPRE = 0b01;

	/* enable the SPI module*/
	SPI1STAT = 0x8000; 

	/* Init Emission to the maximun Distance */
	write_SPI( 0 );
}


void write_SPI( short command ){
	short temp;	

	PORTBbits.RB2 = 0;		// lower the slave select line
	temp = SPI1BUF;			// dummy read of the SPI1BUF register to clear the SPIRBF flag
	SPI1BUF = command;		// write the data out to the SPI peripheral
    while (!SPI1STATbits.SPIRBF);	// wait for the data to be sent out
	PORTBbits.RB2 = 1;		// raise the slave select line
}

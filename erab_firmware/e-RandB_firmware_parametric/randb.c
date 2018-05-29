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
#include "stdio.h"
#include "stdlib.h"

#include "ports.h"
#include "I2CSlaveDrv.h"
#include "PWMEmission.h"
#include "SPIMasterDrv.h"
#include "UARTDrv.h"

#include "IRReception.h"
#include "IREmission.h"
#include "DataLength.h"


char Txdata[50]; 

/* Configure CLOCK */
_FOSCSEL(FNOSC_FRC); 				
_FOSC(FCKSM_CSECMD &  // Clock Switching Enabled and Fail Safe Clock Monitor is disabled
	  OSCIOFNC_OFF &  // OSC2 Pin Function: OSC2 is Clock Output
      POSCMD_XT); 	  // Primary Oscillator Mode: XT Crystal
													
													
_FWDT(FWDTEN_OFF); 	  // Watchdog Timer Enabled/disabled by user software

void initRandomNumberGenerator ()
{
    unsigned int readings[NUM_RECEPTOR_SENSORS];
    unsigned int seed = 0;

    long int i;
	int j;
    for (i = 0; i < 15; i++)
    {	
		for (j = 0; j < 500; j++) asm ("nop");

		ReadAllIrPeaks(readings);
	    seed += readings[0] ^ readings[1];
	    seed += readings[2] ^ readings[3];
	    seed += readings[4] ^ readings[5];
	    seed += readings[6] ^ readings[7];
	    seed += readings[8] ^ readings[9];
	    seed += readings[10] ^ readings[11];
    }
    srand(seed);
	/*************** DEBUG RS232 - PC *****************/
	/*char SerialTX[50];
	sprintf(SerialTX,"%u \n\r",seed);
	WriteUart2((unsigned int*) SerialTX); */
	/*************** END DEBUG *****************/
}

int main ( void )
{
	init_ports();
	init_data_length();
	/* Init I2C*/
	init_I2C();
	/* Init SPI*/
	init_SPI();
	/* Init UART2 (PC)*/
	init_UART1();
	init_UART2();
	
	/* Data o be transmitted using UART communication module */	
	sprintf(Txdata,"E-PUCK RANGE AND BEARING STARTED\n\r");
   	WriteUart2( (unsigned int*) Txdata);

	/* Init Emmission task*/
	init_Emmision();
	/* Init Reception task */
	init_Reception();
	/* Init Random Number Generator */
	initRandomNumberGenerator();


	
	/* Medium Power Transmision */
	write_SPI( 100 );

	LED0 = 0;
	EmptyRxQueue ();
	while(1){
		ManageUart1Reception();
		DecodeData();
	}
	close_UART2();
}

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
#include "ports.h"

/* HOW THE FRAME IS COMPOSE */
#define HEADER_LENGTH 3
#define HEADER_LENGTH2 2*HEADER_LENGTH
#define FRAME_DATA_LENGTH_32 32
#define CRC_LENGTH 4
#define EOT_LENGTH 2


/* TOTAL NUMBER OF BITS */
#define FRAME_BITS_32 2*(HEADER_LENGTH + FRAME_DATA_LENGTH_32 + CRC_LENGTH)

//Functions prototype
void init_PWM( void );
void init_T1( void );
void init_Emmision( void );
char GetEmissionFrame ( unsigned int *frame );
char SetEmissionFrame ( unsigned int *frame);

int getFrameBits(int frameDataLength);

void __attribute__((__interrupt__)) _T2Interrupt( void );

void re_init_queue_emission();

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
#include "registers.h"

//Functions prototype
void init_UART2( void );
void close_UART2 ( void );
void WriteUart2 ( unsigned int *data );
void __attribute__((__interrupt__)) _U2TXInterrupt(void);
void __attribute__((__interrupt__)) _U2RXInterrupt(void);

void init_UART1( void );
void close_UART1 ( void );
void WriteUart1 ( void );
void __attribute__((__interrupt__)) _U1TXInterrupt(void);
void __attribute__((__interrupt__)) _U1RXInterrupt(void);
void ManageUart1Reception ( void );
void EmptyRxQueue ( void );

void SetUart1Communication ( char value ) ;
char GetUart1Communication ( void );

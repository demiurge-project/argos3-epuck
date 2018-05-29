#include "stdio.h"
#include "ports.h"
#include "queue.h"
#include "ADCDrv.h"
#include "UARTDrv.h"
#include "registers.h"




typedef struct{
	int received;			/* If data has been received */
	int alpha;				/* Angle to the source */
	unsigned int distance;	/* Distance to the source */
	//int data; 				/* Data received */
	unsigned long data; 				/* Data received */
} dataCompleated;

void init_Reception ( void );
void InitAllIrPeaks ( void );

void ReadAllIrPeaks ( unsigned int * readings);
void ReadIrPeak (unsigned int * reading, char channel );

void DisableAllIrPeaks ( void );
void DisableIrPeak ( char channel);

void EnableAllIrPeaks ( void );
void EnableIrPeak ( char channel);
void ResetIrPeak ( char channel);

void __attribute__((__interrupt__)) _T3Interrupt( void );

char GetReceptionFrame ( unsigned int *frame );
char SetReceptionFrame ( int frame );

void DecodeData ( void );

char GetReceptionData ( finalDataRegister *data );
char SetReceptionData ( finalDataRegister data );

char GetCalcOnBoard ( void );
void SetCalcOnBoard ( char value );

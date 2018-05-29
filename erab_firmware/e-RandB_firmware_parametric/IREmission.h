#include "PWMEmission.h"
#include "UARTDrv.h"
#include "stdio.h"

/* NUMBER OF IR EMITTING SENSORS */
#define NUM_RECEPTOR_SENSORS 12

/* Store and Write the IR data sent to all the sensors. The same for all*/
void WriteAllIrData ( unsigned long data);
//void WriteAllIrData ( unsigned int data);
/* Store IR data on each specific channel */
void StoreIrData ( int channel, unsigned long data);
//void StoreIrData ( int channel, unsigned int data);
/*Send the loaded values */
void WriteIrData ( void );


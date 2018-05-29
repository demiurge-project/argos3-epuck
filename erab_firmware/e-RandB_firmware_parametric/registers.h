/*	registers.h
*/
#ifndef _REGISTERSH_
#define _REGISTERSH_
#include "ports.h"
#include "stdio.h"
//#include "UARTDrv.h"
#include "IREmission.h"

extern int mediumStatus;

#ifndef _DATAREGISTERRECEIVED_
typedef struct {
	int received;
	int alpha;
	unsigned int distance;
	//unsigned int data;
	unsigned long data;
	char enable;
} dataRegisterReceived;

typedef struct {
	int received;
	unsigned int alpha;
	unsigned long data;
	//unsigned int data;
} modelledDataRegisterReceived;

typedef struct {
	//unsigned int data;
	unsigned long data;
	float angle;
	unsigned int range;
	unsigned int max_peak1;
	unsigned char max_sensor1;
	int bearing;
} finalDataRegister;

#define _DATAREGISTERRECEIVED_
#endif

void setDataReceived (dataRegisterReceived data);
//void setAllDataReceived (dataRegisterReceived *Alldata, unsigned int data);
void setAllDataReceived (dataRegisterReceived *Alldata, unsigned long data);
void setModelledDataReceived (modelledDataRegisterReceived data);

void getDataReceived (dataRegisterReceived *data);
//void getAllDataReceived (dataRegisterReceived *data);
//unsigned char getAllDataReceived ( dataRegisterReceived *Alldata, unsigned int *data);
unsigned char getAllDataReceived ( dataRegisterReceived *Alldata, unsigned long *data);
void getModelledDataReceived (modelledDataRegisterReceived *data);

void resetDataReceived ( void );
void resetAllDataReceived ( void );
void resetModelledDataReceived ( void );

void init_registers( void );
void setMediumStatus ( int status ); 
int getMediumStatus ( void );

#endif

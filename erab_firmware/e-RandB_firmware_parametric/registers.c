/*	registers.c
*/

#include "Registers.h"


dataRegisterReceived lastDataReceived;
dataRegisterReceived allDataReceived[NUM_RECEPTOR_SENSORS];
//unsigned int dataReceivedToSend;
unsigned long dataReceivedToSend;
unsigned char flagDataToSend;

modelledDataRegisterReceived modelDataReceived;

int mediumStatus=0;
//dataRegisterReceived *allDataReceived;

void init_registers ( void ){
	lastDataReceived.received = 0;
	lastDataReceived.data = 0;
	lastDataReceived.alpha = 0;
	lastDataReceived.distance = 0;

	modelDataReceived.received = 0;
	modelDataReceived.data = 0;
	modelDataReceived.alpha = 0;
	//modelDataReceived.distance = 0;

	int i;
	for (i= 0 ; i < NUM_RECEPTOR_SENSORS ; i++){
		allDataReceived[i].received = 0;
		allDataReceived[i].data = 0;
		allDataReceived[i].alpha = 0;
		allDataReceived[i].distance = 0;
	}
	flagDataToSend = FALSE;
}

void setDataReceived( dataRegisterReceived data){
	lastDataReceived = data;
	/*************** DEBUG RS232 - PC *****************/
	/*char SerialTX[50];
	sprintf(SerialTX,"%x %d %d \n\r",lastDataReceived.data,lastDataReceived.alpha,lastDataReceived.distance);
	WriteUart2((unsigned int*) SerialTX);*/
	/*************** END DEBUG *****************/
}

//void setAllDataReceived ( dataRegisterReceived *Alldata, unsigned int data ){
void setAllDataReceived ( dataRegisterReceived *Alldata, unsigned long data ){
	int i;
	for ( i = 0 ; i < NUM_RECEPTOR_SENSORS ; i++){
		allDataReceived[i] = Alldata[i];
	}
	dataReceivedToSend = data;
	flagDataToSend = TRUE;
	/*************** DEBUG RS232 - PC *****************/
	/*char SerialTX[50];
	sprintf(SerialTX,"%d %d %d %d %d %d %d %d %d %d %d %d\n\r",	
		allDataReceived[0].distance,allDataReceived[1].distance, 
		allDataReceived[2].distance,allDataReceived[3].distance, 
		allDataReceived[4].distance,allDataReceived[5].distance, 
		allDataReceived[6].distance,allDataReceived[7].distance, 
		allDataReceived[8].distance,allDataReceived[9].distance, 
		allDataReceived[10].distance,allDataReceived[11].distance);
	WriteUart2((unsigned int*) SerialTX);*/
	/*************** END DEBUG *****************/

}
void setModelledDataReceived( modelledDataRegisterReceived data){
	modelDataReceived = data;
	/*************** DEBUG RS232 - PC *****************/
	/*char SerialTX[50];
	sprintf(SerialTX,"%x %d %d \n\r",modelDataReceived.data,modelDataReceived.alpha,modelDataReceived.distance);
	WriteUart2((unsigned int*) SerialTX);*/
	/*************** END DEBUG *****************/
}

void getDataReceived (dataRegisterReceived *data){
	*data = lastDataReceived;
}

//unsigned char getAllDataReceived ( dataRegisterReceived *Alldata, unsigned int *data){
unsigned char getAllDataReceived ( dataRegisterReceived *Alldata, unsigned long *data){
	if ( flagDataToSend == TRUE ) 
	{
		int i;
		for ( i = 0 ; i < NUM_RECEPTOR_SENSORS ; i++){
			Alldata[i] = allDataReceived[i] ;
		}
		*data = dataReceivedToSend;
		return 1; 
	}
	else 
	{
		return 0;
	}
}
void getModelledDataReceived (modelledDataRegisterReceived *data){
	*data = modelDataReceived;
}

void resetDataReceived ( void ){
	lastDataReceived.received = 0;
}

void resetAllDataReceived ( void ){
	flagDataToSend = FALSE;
}
void resetModelledDataReceived ( void ){
	modelDataReceived.received = 0;
}

void setMediumStatus ( int status ) 
{
    mediumStatus = status;
}

int getMediumStatus ( void ) 
{
    return mediumStatus;
}

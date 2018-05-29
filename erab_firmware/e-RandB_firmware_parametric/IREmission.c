#include "IREmission.h"
#include "DataLength.h"
#include <math.h>

/* Where the Data is stored */
//unsigned int DataStorage[NUM_RECEPTOR_SENSORS];
unsigned long DataStorage[NUM_RECEPTOR_SENSORS];
/* Flag to check if sensor is going to transmit */
int DataReadyToSend[NUM_RECEPTOR_SENSORS];
/* The Sifts for all the sensors to comply with the buffer */
int SiftSensors[NUM_RECEPTOR_SENSORS] = {3, 4, 5, 8, 9, 10, 11, 12, 13, 14, 15, 0};

/* Store and Write the IR data sent to all the sensors. The same for all*/
//void WriteAllIrData ( unsigned int data){
void WriteAllIrData ( unsigned long data){
	int i;
	for (i = 0; i < NUM_RECEPTOR_SENSORS; i++){
		DataStorage[i] = data;
		DataReadyToSend[i] = 1;
	}
	WriteIrData();
}

/* Store IR data on each specific channel */
//void StoreIrData ( int channel, unsigned int data){
void StoreIrData ( int channel, unsigned long data){
	DataStorage[channel] = data;
	DataReadyToSend[channel] = 1;
	/*************** DEBUG RS232 - PC *****************/
	//char SerialTX[50];
	//sprintf(SerialTX,"%u ",data);
	//WriteUart2((unsigned int*) SerialTX); 
	/*************** END DEBUG *****************/

}

/*Send the loaded values */
void WriteIrData ( void ){
	int i,j;
	unsigned int aux=0;
	int frame_bits = getFrameBits(get_data_length());
	int frame_data_length = get_data_length();

	/*The frame transposed in the correct order to send the frame */
	unsigned int DataSend[(frame_bits/2) + EOT_LENGTH];
	unsigned int DataSendManch[(frame_bits/2) + EOT_LENGTH];
	unsigned int ManchesterFrame[frame_bits + ( 2 * EOT_LENGTH)];
//	unsigned int DataSend[(FRAME_BITS/2) + EOT_LENGTH];
//	unsigned int DataSendManch[(FRAME_BITS/2) + EOT_LENGTH];
//	unsigned int ManchesterFrame[FRAME_BITS + ( 2 * EOT_LENGTH)];

	unsigned int CRC[NUM_RECEPTOR_SENSORS];
	unsigned int CRCManch[NUM_RECEPTOR_SENSORS];

	/* CLEAR DATA TO SEND */
	for(i = 0; i < frame_bits/2; i++){
	//for(i = 0; i < FRAME_BITS/2; i++){
		DataSend[i] = 0;
		DataSendManch[i] = 0;
	}
	/* CLEAR CRC VAR */
	for(i=0; i < NUM_RECEPTOR_SENSORS; i++){
		CRC[i] = 0;
		CRCManch[i] = 0;
	}

	/* WRITE HEADER*/
	for(j = 0; j < NUM_RECEPTOR_SENSORS; j++){
		if ( DataReadyToSend[j] == 1){
			aux +=  (1 << SiftSensors[j]);
		}
	}

	for (i = 0; i < HEADER_LENGTH; i++){
		/* Header is repeated for the length of the header */
		/* Header is not in Manchester Code*/
		DataSend[i] = aux;
		DataSendManch[i] = aux;
	}

	/* WRITE DATA */
	for(i=0; i < frame_data_length; i++){
	//for(i=0; i < FRAME_DATA_LENGTH; i++){		
		/* Transpose sensor data into emission frame */
		for(j = 0; j < NUM_RECEPTOR_SENSORS; j++){			
			aux = (DataStorage[j] >> i) & 0x01;
			DataSend[i+HEADER_LENGTH] += (aux << SiftSensors[j]);
			if(DataReadyToSend[j] == 1){
				DataSendManch[i+HEADER_LENGTH] += (((~aux) & 0x01) << SiftSensors[j]);
			}			
			CRC[j]+=aux;
		}
	}

	/* WRITE CRC */
	for(i=0; i < NUM_RECEPTOR_SENSORS; i++){
		CRC[i] %= 16;
		CRC[i] = 16 - CRC[i];

		DataSend[HEADER_LENGTH+frame_data_length] += ((CRC[i] >> 3) & 0x01) << SiftSensors[i];
		DataSend[HEADER_LENGTH+frame_data_length+1] += ((CRC[i] >> 2) & 0x01) << SiftSensors[i];
		DataSend[HEADER_LENGTH+frame_data_length+2] += ((CRC[i] >> 1) & 0x01) << SiftSensors[i];
		DataSend[HEADER_LENGTH+frame_data_length+3] += (CRC[i] & 0x01) << SiftSensors[i];
		if(DataReadyToSend[i] == 1){
			DataSendManch[HEADER_LENGTH+frame_data_length] += ((~CRC[i] >> 3) & 0x01) << SiftSensors[i];
			DataSendManch[HEADER_LENGTH+frame_data_length+1] += ((~CRC[i] >> 2) & 0x01) << SiftSensors[i];
			DataSendManch[HEADER_LENGTH+frame_data_length+2] += ((~CRC[i] >> 1) & 0x01) << SiftSensors[i];
			DataSendManch[HEADER_LENGTH+frame_data_length+3] += (~CRC[i] & 0x01) << SiftSensors[i];

		}
	}
	/* INSERT EOF*/
	DataSend[HEADER_LENGTH + frame_data_length + CRC_LENGTH + 0] = 0;
	DataSend[HEADER_LENGTH + frame_data_length + CRC_LENGTH + 1] = 0;
	//DataSend[HEADER_LENGTH + FRAME_DATA_LENGTH + CRC_LENGTH + 2] = 0;
	//DataSend[HEADER_LENGTH + FRAME_DATA_LENGTH + CRC_LENGTH + 3] = 0;
	//DataSend[HEADER_LENGTH + FRAME_DATA_LENGTH + CRC_LENGTH + 4] = 0;
	//DataSend[HEADER_LENGTH + FRAME_DATA_LENGTH + CRC_LENGTH + 5] = 0;
	//DataSend[HEADER_LENGTH + FRAME_DATA_LENGTH + CRC_LENGTH + 6] = 0;
	//DataSend[HEADER_LENGTH + FRAME_DATA_LENGTH + CRC_LENGTH + 7] = 0;
	DataSendManch[HEADER_LENGTH + frame_data_length + CRC_LENGTH + 0] = 0;
	DataSendManch[HEADER_LENGTH + frame_data_length + CRC_LENGTH + 1] = 0;
	//DataSendManch[HEADER_LENGTH + FRAME_DATA_LENGTH + CRC_LENGTH + 2] = 0;
	//DataSendManch[HEADER_LENGTH + FRAME_DATA_LENGTH + CRC_LENGTH + 3] = 0;
	//DataSendManch[HEADER_LENGTH + FRAME_DATA_LENGTH + CRC_LENGTH + 4] = 0;
	//DataSendManch[HEADER_LENGTH + FRAME_DATA_LENGTH + CRC_LENGTH + 5] = 0;
	//DataSendManch[HEADER_LENGTH + FRAME_DATA_LENGTH + CRC_LENGTH + 6] = 0;
	//DataSendManch[HEADER_LENGTH + FRAME_DATA_LENGTH + CRC_LENGTH + 7] = 0;
	//DataSendManch[HEADER_LENGTH + FRAME_DATA_LENGTH + CRC_LENGTH + 8] = 0;
	
	for(i = 0; i< frame_bits/2; i++){
		ManchesterFrame[2*i] = DataSend[i];		
		ManchesterFrame[(2*i)+1] = DataSendManch[i];
	}

	SetEmissionFrame(ManchesterFrame);

	for(i=0; i< NUM_RECEPTOR_SENSORS; i++){
		DataStorage[i] = 0;
		DataReadyToSend[i] = 0;
	}
}

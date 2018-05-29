#include "p33FJ256GP710.h"
#include <math.h>

#include "IRReception.h"
#include <math.h>
#define STATE_HEADER 0
#define STATE_DATA 1
#define STATE_CRC 2
#define WAIT 100


queueSingle queueReception;
queueSingle queuePeakReception[NUM_RECEPTOR_SENSORS];
queueFinal queueFinalReception;
int calcOnBoard;

/* Variables for DATA INPUT */
int dataInput;
int dataInputOld[NUM_RECEPTOR_SENSORS];
int countBits[NUM_RECEPTOR_SENSORS];
int countFrame[NUM_RECEPTOR_SENSORS];
//unsigned int receivedData[NUM_RECEPTOR_SENSORS];
unsigned long receivedData[NUM_RECEPTOR_SENSORS];
int crcCalc[NUM_RECEPTOR_SENSORS];
int crcReceived[NUM_RECEPTOR_SENSORS];
unsigned int readFrame;

//int state[NUM_RECEPTOR_SENSORS];
int state;
char SerialTX[50];

int noSignal;
int preambleReceived;
unsigned int oldDataInput;
int counterPreamble;
int counterPreambleStart;

int counterDisable;
int NEXT[NUM_RECEPTOR_SENSORS];
int received;
//unsigned int dataReceived;
unsigned long dataReceived;

int SensorAngle[] = {15,50,75,105,133,159,-165,-135,-105,-77,-50,-15};double CosSensorAngle[] = {0.965926,0.642788,0.258819,-0.258819,-0.681998,-0.93358,-0.965926,-0.707107,-0.258819,0.224951,0.642788,0.965926};double SinSensorAngle[] = {0.258819,0.766044,0.965926,0.965926,0.731354,0.358368,-0.258819,-0.707107,-0.965926,-0.97437,-0.766044,-0.258819};double SensorAngleRads[] = {0.261799,0.872665,1.308997,1.832596,2.321288,2.775073,-2.879793,-2.356194,-1.832596,-1.343903,-0.872665,-0.261799};

dataRegisterReceived dataCompleatedIteration[NUM_RECEPTOR_SENSORS];
//dataRegisterReceived dataCompleatedIterationToSend[NUM_RECEPTOR_SENSORS];

/* Variables for PEAK RECEPTOR */
unsigned int IrPeakReadings[NUM_RECEPTOR_SENSORS];
unsigned int IrPeakReadingsInit[NUM_RECEPTOR_SENSORS];

long int NumberFrames = 0;

void init_Reception ( void ){

	int i;
	/* Reset IR peaks */
	for (i = 0 ; i < 12 ; i++){
		ResetIrPeak(i);
	}
	/* Init ADC for Peak reception */
	init_ADC();

	/* Timer 2 reset */
	T3CONbits.TON = 0;
	/* Select internal clock */
	T3CONbits.TCS = 0;
	/* Disable Gated Timer mode*/
	T3CONbits.TGATE = 0; 
	/* Set Timer3 interrupt priority to 5 */
	IPC2bits.T3IP = 1;
	/* Reset Timer3 interrupt flag */
	IFS0bits.T3IF = 0;
	/* Enable Timer1 interrupt */
	IEC0bits.T3IE = 1;
	
	T3CONbits.TCKPS = 0; //Preescaler 1:1
	/* Tcy = 50ns --> 
	 * Treceptiondata(20Khz) = 50us
	 * PR3 = Treceptiondata / Tcy = 1000 */
	TMR3=  0x0000;  	
	//PR3 = 1000;           
	PR3 = 4000;           
	/* Enable Timer3 and start the counter */
	T3CONbits.TON = 1; 

	init_queue_single(&queueReception);
	init_queue_final(&queueFinalReception);

	for (i = 0 ; i < NUM_RECEPTOR_SENSORS ; i++){
		ResetIrPeak(i);
		dataInputOld[i] = 0;
		countBits[i] = 0;
		countFrame[i] = 0;
		receivedData[i] = 0;
		NEXT[i] = 0;
		init_queue_single(&queuePeakReception[i]);
		dataCompleatedIteration[i].enable = TRUE;
	}
	state = STATE_HEADER;
	counterDisable = 0;
	noSignal = 0;
	preambleReceived = FALSE;
	oldDataInput = 0xFFFF;
	counterPreamble = 0;
	counterPreambleStart = 0;
	received = 0;
	calcOnBoard = ON_BOARD;
	
	/* Read Actual Status of IR signal on the environment */
	//ReadAllIrPeaks(IrPeakReadingsInit);
	InitAllIrPeaks();

	for (i = 0 ; i < NUM_RECEPTOR_SENSORS ; i++){
		sprintf(SerialTX,"%u ",IrPeakReadingsInit[i]);
		WriteUart2((unsigned int*) SerialTX); 	
	}
	sprintf(SerialTX,"\n\r");
	WriteUart2((unsigned int*) SerialTX); 	
}

void InitAllIrPeaks ( void ){
	ReadAllIrPeaks(IrPeakReadingsInit);
}
/* FUNCTION FOR READING ALL THE PEAK DETECTOR VALUES */
void ReadAllIrPeaks ( unsigned int * readings){
	/* Take all the readings form the ADC channels */
	int i;
	for(i = 0; i < 12; i++){
		readings[i] = ReadADC ( 20 + i);
	}
	
	/* Convert all the readings related to the actual light conditions */
	//for(i = 0; i < 12; i++){
	//	/* Acutal value = Actual value - Init Value */
	//	if(IrPeakReadings[i] > IrPeakReadingsInit[i])
	//		IrPeakReadings[i] = IrPeakReadings[i] - IrPeakReadingsInit[i];			
	//	else
	//		IrPeakReadings[i] = 0;
	//}
}

/* FUNCTION FOR READING A SPECIFIC PEAK DETECTOR CHANNEL */
void ReadIrPeak (unsigned int * reading, char channel )
{
	/* Read the value */
	int aux = ReadADC(20 + channel);

	aux -= IrPeakReadingsInit[channel];
	/* Convert the value related to the light conditions */
	if (aux>0)
		*reading = (unsigned int) aux;
	else
		*reading = 0;
}

/* FUNCTION FOR DISABLE ALL THE IR PEAKS */
void DisableAllIrPeaks ( void ) {
	/* Enable reset */
	RESET_PEAK1 = HIGH;
	RESET_PEAK2 = HIGH;
	RESET_PEAK3 = HIGH;
	RESET_PEAK4 = HIGH;	
	RESET_PEAK5 = HIGH;
	RESET_PEAK6 = HIGH;
	RESET_PEAK7 = HIGH;
	RESET_PEAK8 = HIGH;
	RESET_PEAK9 = HIGH;
	RESET_PEAK10 = HIGH;
	RESET_PEAK11 = HIGH;
	RESET_PEAK12 = HIGH;
}
/* FUNCTION FOR RESETING AN SPECIFIC IR PEAK */
void ResetIrPeak ( char channel){

	int i;
	switch(channel){
		case 0:
			/* Reset the peak detecto */
			RESET_PEAK1 = HIGH;
			/* Wait a period of time */
			for(i = 0 ; i < WAIT; i++)
				__asm__("nop");
			/* Enable the peak receptor */
			RESET_PEAK1 = LOW;
			break;
		case 1:
			RESET_PEAK2 = HIGH;
			for(i=0; i < WAIT; i++)
				__asm__("nop");
			RESET_PEAK2 = LOW;
			break;
		case 2:
			RESET_PEAK3 = HIGH;
			for(i=0; i < WAIT; i++)
				__asm__("nop");
			RESET_PEAK3 = LOW;
			break;
		case 3:
			RESET_PEAK4 = HIGH;
			for(i=0; i < WAIT; i++)
				__asm__("nop");
			RESET_PEAK4 = LOW;
			break;
		case 4:
			RESET_PEAK5 = HIGH;
			for(i=0; i < WAIT; i++)
				__asm__("nop");
			RESET_PEAK5 = LOW;
			break;
		case 5:
			RESET_PEAK6 = HIGH;
			for(i=0; i < WAIT; i++)
				__asm__("nop");
			RESET_PEAK6 = LOW;
			break;
		case 6:
			RESET_PEAK7 = HIGH;
			for(i=0; i < WAIT; i++)
				__asm__("nop");
			RESET_PEAK7 = LOW;
			break;
		case 7:
			RESET_PEAK8 = HIGH;
			for(i=0; i < WAIT; i++)
				__asm__("nop");
			RESET_PEAK8 = LOW;
			break;
		case 8:
			RESET_PEAK9 = HIGH;
			for(i=0; i < WAIT; i++)
				__asm__("nop");
			RESET_PEAK9 = LOW;
			break;
		case 9:
			RESET_PEAK10 = HIGH;
			for(i=0; i < WAIT; i++)
				__asm__("nop");
			RESET_PEAK10 = LOW;
			break;
		case 10:
			RESET_PEAK11 = HIGH;
			for(i=0; i < WAIT; i++)
				__asm__("nop");
			RESET_PEAK11 = LOW;
			break;
		case 11:
			RESET_PEAK12 = HIGH;
			for(i=0; i < WAIT; i++)
				__asm__("nop");
			RESET_PEAK12 = LOW;
			break;
	}
}

/* FUNCTION FOR ENABLING ALL PEAK READINGS */
void EnableAllIrPeaks ( void ){
	//int i;

	/* Wait a small time to capacitor to discharge */
	//for (i = 0; i < 100; i++){__asm__("nop");}
	
	/* Disable reset */
	RESET_PEAK1 = LOW;
	RESET_PEAK2 = LOW;
	RESET_PEAK3 = LOW;
	RESET_PEAK4 = LOW;	
	RESET_PEAK5 = LOW;
	RESET_PEAK6 = LOW;
	RESET_PEAK7 = LOW;
	RESET_PEAK8 = LOW;
	RESET_PEAK9 = LOW;
	RESET_PEAK10 = LOW;
	RESET_PEAK11 = LOW;
	RESET_PEAK12 = LOW;	
}

/* FUNCTION FOR ENABLING A SPECIFIC PEAK READING */
void EnableIrPeak ( char channel){
	switch(channel){
		case 0:
			RESET_PEAK1 = LOW;
			break;
		case 1:
			RESET_PEAK2 = LOW;
			break;
		case 2:
			RESET_PEAK3 = LOW;
			break;
		case 3:
			RESET_PEAK4 = LOW;
			break;
		case 4:
			RESET_PEAK5 = LOW;
			break;
		case 5:
			RESET_PEAK6 = LOW;
			break;
		case 6:
			RESET_PEAK7 = LOW;
			break;
		case 7:
			RESET_PEAK8 = LOW;
			break;
		case 8:
			RESET_PEAK9 = LOW;
			break;
		case 9:
			RESET_PEAK10 = LOW;
			break;
		case 10:
			RESET_PEAK11 = LOW;
			break;
		case 11:
			RESET_PEAK12 = LOW;
			break;
	}
}

/* FUNCTION FOR ENABLING A SPECIFIC PEAK READING */
void DisableIrPeak ( char channel){
	switch(channel){
		case 0:
			RESET_PEAK1 = HIGH;
			break;
		case 1:
			RESET_PEAK2 = HIGH;
			break;
		case 2:
			RESET_PEAK3 = HIGH;
			break;
		case 3:
			RESET_PEAK4 = HIGH;
			break;
		case 4:
			RESET_PEAK5 = HIGH;
			break;
		case 5:
			RESET_PEAK6 = HIGH;
			break;
		case 6:
			RESET_PEAK7 = HIGH;
			break;
		case 7:
			RESET_PEAK8 = HIGH;
			break;
		case 8:
			RESET_PEAK9 = HIGH;
			break;
		case 9:
			RESET_PEAK10 = HIGH;
			break;
		case 10:
			RESET_PEAK11 = HIGH;
			break;
		case 11:
			RESET_PEAK12 = HIGH;
			break;
	}
}

/* RECEIVING INTERRUPT */
void __attribute__((__interrupt__, no_auto_psv)) _T3Interrupt(void)
{
	/* Reset Comparator */
	TMR3=0;
	 
	/* Get all the actual data values */
	unsigned int dataInput = REPort & 0xFFF0;


	/* If Working with CSMA */
	#ifdef _CSMA_
		/* If no data is received */
		if((dataInput) == 0xFFF0){
			/* If there is being less than 3 times without receiving a frame */
			if(noSignal < 2){
				/* Store it */
				QueueInSingle(&queueReception,dataInput);
				noSignal++;
			    /*It's possible that a signal is on the medium*/
				mediumStatus = NOT_IDLE;
			}
			else{
   				/*It's possible that a signal is on the medium*/
				mediumStatus = IDLE;
			}
		}
		/* If something received */
		else{	
			/* Reset the noSignal tag */
			noSignal=0;
			/* Store the data */
			QueueInSingle(&queueReception,dataInput);
			/* The medium is not free*/ 
			mediumStatus = NOT_IDLE;
		}	
	/* If not Working with CSMA */
	#else
		int i;
		/* If no data is received */
		if((dataInput) == 0xFFF0){
			/* If there is being less than 2 times without receiving a frame */
			if(noSignal < 2){
//				if ( preambleReceived == TRUE ){
//					counterPreamble++;
//					if ( counterPreamble == 1 ) {
//						DisableAllIrPeaks();
//					}
//					if ( counterPreamble == 2) {
//						EnableAllIrPeaks();
//					}
//				}
				/* Store it */
				QueueInSingle(&queueReception,dataInput);
				noSignal++;

			}
			else{
				if ( preambleReceived == TRUE ){
					unsigned int readingsPeak[NUM_RECEPTOR_SENSORS];
					//ReadAllIrPeaks(readingsPeak);
					for ( i = 0 ; i < NUM_RECEPTOR_SENSORS; i++)
					{
						ReadIrPeak(&readingsPeak[i],i);
						QueueInSingle(&queuePeakReception[i],readingsPeak[i]);
					}
					preambleReceived = FALSE;
				}
			}
			counterPreambleStart = 0;
		}
		/* If something received */
		else{	
			if ( preambleReceived == FALSE )
			{
				counterPreambleStart++;
				if ( counterPreambleStart == 3 ){
					DisableAllIrPeaks();
 				}
				if ( counterPreambleStart == (HEADER_LENGTH2 -1) ) 
				{
					EnableAllIrPeaks();
					preambleReceived = TRUE;
//					counterPreamble = 0;
				}
			}
//			else 
//			{
//				counterPreamble++;
//				if ( counterPreamble == 1) {
//					DisableAllIrPeaks();
//				}
//				if ( counterPreamble == 2) {
//					EnableAllIrPeaks();
//				}
//			}
			/* Reset the noSignal tag */
			noSignal=0;
			/* Store the data */
			QueueInSingle(&queueReception,dataInput);
//			oldDataInput = dataInput;
		}
	#endif

	/* Clear Timer3 interrupt */
	IFS0bits.T3IF = 0;
}

/* FUNCTION FOR GETTING A FRAME FROM THE QUEUE */
char GetReceptionFrame ( unsigned int *frame ){
	return QueueOutSingle(&queueReception,frame);
}
/* FUNCTION FOR SETTING A FRAME FROM THE QUEUE */
char SetReceptionFrame ( int frame){
	return QueueInSingle(&queueReception,frame);
}

/* FUNCTION FOR DECODING DATA - MAIN FUNCTION OF ALL THE BOARD */
void DecodeData ( void ) {
	/*************** DEBUG RS232 - PC *****************/
	//while(GetReceptionFrame(&readFrame)){
	//	sprintf(SerialTX,"%x\n\r",readFrame);
	//	WriteUart2((unsigned int*) SerialTX); 
	//}
	/*************** END DEBUG *****************/

	int i;
//	unsigned int PeakReading;
	/* If there is a frame */
	if(GetReceptionFrame(&readFrame))
	{
		/*************** DEBUG RS232 - PC *****************/
		//sprintf(SerialTX,"%d \n\r",counterDisable);
		//WriteUart2((unsigned int*) SerialTX); 
		/*************** END DEBUG *****************/
		received = 0;
		int frame_data_length = get_data_length();

		/* Read the data related to the sensor */
		/* Switch the state in which the specific sensor is */
		switch(state)
		{
			/* If waiting for a HEADER */
			case STATE_HEADER:
				/* For each sensor */
				for( i = 0 ; i < NUM_RECEPTOR_SENSORS ; i++ )
				{
					if ( dataCompleatedIteration[i].enable == TRUE) 
					{
						dataInput = ( readFrame >> ( i + 4) ) & 0x1;
						/* If the data received is the expected. */
						/* Header frame is not in Manchester Code */				
						if(dataInput==NEXT[i])
						{
							/* Increase heading counts */
							countBits[i]++;
						}
						/* If data is not what expected */
						else
						{
							dataCompleatedIteration[i].enable = FALSE;
							counterDisable++;
							countBits[i] = 0;							
							//QueueOutSingle(&queuePeakReception[i],&PeakReading);
						}
					
						/* If a preamble has been received (double in manchester */
						if(countBits[i] == HEADER_LENGTH2)
						{
							/* Change state to receive data */
							state = STATE_DATA;
							/* Reset the countBits index*/
							countBits[i] = 0;
							/* Set the length of the data field*/
							countFrame[i] = frame_data_length;
							/* Reset the receivedData value*/
							receivedData[i] = 0;
							/* Reset the crcReceived value*/
							crcReceived[i] = 0;
						}
					}
				}
				break;

			/* If waiting for DATA */
			case STATE_DATA:
				/* For each sensor */
				for( i = 0 ; i < NUM_RECEPTOR_SENSORS ; i++ )
				{
					if ( dataCompleatedIteration[i].enable == TRUE) 
					{
						dataInput = ( readFrame >> ( i + 4) ) & 0x1;
						/* If first half bit of a Manchester code*/
						if(countBits[i] == 0)
						{
							/* Increase counter */
							countBits[i]++;
							/* Store the old data */
							dataInputOld[i] = dataInput;
						}
						/* If it is the second bit of the manchester code*/
						else
						{	
							/* Reset counter */
							countBits[i] = 0;
							/* If the invers of the first bit received */
							if ( dataInput != dataInputOld[i])
							{
								/* add to the data value */
								//receivedData[i]+=( dataInput << ( FRAME_DATA_LENGTH - countFrame[i]) );	
								// Now that the FRAME_DATA_LENGTH is 32 we need a long to contain the bits
								receivedData[i]+=( (long)dataInput << ( frame_data_length - countFrame[i]) );	
								//receivedData[i]+=( (long)dataInput << ( FRAME_DATA_LENGTH - countFrame[i]) );	
									
								/* Decrease the number of frames till the end of data*/
								countFrame[i]--;
								/* Store the value for the crc check */
								if(dataInputOld[i] == 0) crcReceived[i]++;
							}
							/* If Manchester is not arrived. Reset and return to heading state */
							else
							{
								//state = STATE_HEADER;
								dataCompleatedIteration[i].enable = FALSE;
								counterDisable++;
								//while(QueueEmptySingle(&queuePeakReception[i]));
								//QueueOutSingle(&queuePeakReception[i],&PeakReading);
							}
						}	
						/* If all the data have been received */
						if(countFrame[i] == 0)
						{
							/* Change state for CRC reception */
							state = STATE_CRC;
							/* Stores number of CRC bits */
							countFrame[i] = CRC_LENGTH;
							countBits[i] = 0;
							crcCalc[i] = 0;
						}
					}
				}
				break;
	
			/* If waiting for CRC */
			case STATE_CRC:
				/* For each sensor */
				for( i = 0 ; i < NUM_RECEPTOR_SENSORS ; i++ )
				{
					if ( dataCompleatedIteration[i].enable == TRUE) 
					{
						dataInput = ( readFrame >> ( i + 4) ) & 0x1;
						/* If is the first half bit of a Manchester code */
						if(countBits[i]==0)
						{
							/*Increase counter */
							countBits[i]++;
							/* Store the old data */
							dataInputOld[i] = dataInput;
						}	
						/* If it is the second bit of the manchester code */
						else
						{
							/* Reset counter */
							countBits[i]=0;
							/* If the invers of the first bit received */
							if(dataInput != dataInputOld[i])
							{
								/* add to the crc value */
								crcCalc[i] += (dataInput << (countFrame[i]-1));	
								/* Decrease the number of frames till the end of CRC */
								countFrame[i]--; 	
							}
							/* If Manchester is not arrived. Reset and return to heading state */
							else
							{	
								//state = STATE_HEADER;
								dataCompleatedIteration[i].enable = FALSE;
								counterDisable++;
								//while(QueueEmptySingle(&queuePeakReception[i]));
								//QueueOutSingle(&queuePeakReception[i],&PeakReading);
							}	
						}
						/* If all the data have been received */
						if(countFrame[i] == 0)
						{
							/* Check CRC of 2 bits */
							int crc = (crcCalc[i] + crcReceived[i]) & 0x0F;
							/* If CRC OK */
							if( crc == 0x0)
							{
								/* Read related peak */
								//ReadIrPeak(&PeakReading,i);
								//if(PeakReading != 0){
								//while(QueueEmptySingle(&queuePeakReception[i]));
								//if ( QueueOutSingle(&queuePeakReception[i],&PeakReading) ){
								//QueueOutSingle(&queuePeakReception[i],&PeakReading);
								//PeakReading=1;
								/*************** DEBUG RS232 - PC *****************/
								//int queueStatusValue = QueueStatusSingle(&queueReception); 
								//sprintf(SerialTX,"%d %d %d %d\n\r",receivedData[i],i,PeakReading,queueStatusValue);
								//sprintf(SerialTX,"%d ",receivedData[i]);
								//WriteUart2((unsigned int*) SerialTX); 
								/*************** END DEBUG *****************/
								/* Flag the correct reception */
								dataCompleatedIteration[i].received = 1;
								/* Store the sensor */
								//dataCompleatedIteration[i].alpha = i;
								/* Store peak reading */
								//dataCompleatedIteration[i].distance = PeakReading;
								//dataCompleatedIteration[i].distance = PeakReading;
								/* Store data reception */
								//dataCompleatedIteration[i].data = receivedData[i];	
								dataReceived = receivedData[i];
								received = 1;
							}
							/* Reset Count bits index*/
							countBits[i] = 0;
							/* Prepare data frame for data reception */
							countFrame[i] = frame_data_length;
							//countFrame[i] = FRAME_DATA_LENGTH;
							/* Return to heading state */
							//state = STATE_HEADER;
							//DisableIrPeak(i);
						}
					}
				}
				break;
		}

		if ( counterDisable == 12 ) 
		{
			received = 0;
			counterDisable = 0;
			unsigned int trash;
			for( i = 0 ; i < NUM_RECEPTOR_SENSORS ; i++ )
			{
				dataCompleatedIteration[i].enable = TRUE;
				state = STATE_HEADER;
				QueueOutSingle(&queuePeakReception[i],&trash);
			}
		}

		/* If something received*/ 
		if ( received == 1 ) 
		{
			if ( calcOnBoard == ON_BOARD) 
			{
				finalDataRegister finalDataReceived;
				double x=0;
				double y=0;
				unsigned int max_peak1 = 0;
				unsigned int max_peak2 = 0;
				char max_sensor1 = 0;
				char max_sensor2 = 0;
				int num_sensors;
				for( num_sensors = 0 ; num_sensors < NUM_RECEPTOR_SENSORS ; num_sensors++ )
				{
					//int distance = dataCompleatedIteration[num_sensors].distance;
					unsigned int distance = 0;
					while(QueueEmptySingle(&queuePeakReception[num_sensors]));
					QueueOutSingle(&queuePeakReception[num_sensors],&distance);
					/*************** DEBUG RS232 - PC *****************/
					//sprintf(SerialTX,"%u %u ",distance,dataCompleatedIteration[num_sensors].received);
					//sprintf(SerialTX,"%u ",distance);
					//WriteUart2((unsigned int*) SerialTX); 
					/*************** END DEBUG *****************/
					//if ( dataCompleatedIteration[num_sensors].received == 1 ){
					if ( distance != 0 && dataCompleatedIteration[num_sensors].received == 1)
					{
						x+= distance * CosSensorAngle[num_sensors];
						y+= distance * SinSensorAngle[num_sensors];
			
						if ( distance > max_peak1 ) 
						{
							max_peak2 = max_peak1;
							max_sensor2 = max_sensor1;
							max_peak1 = distance;
							max_sensor1 = num_sensors;
						}
						else if ( distance > max_peak2) 
						{
							max_peak2 = distance;
							max_sensor2 = num_sensors;
						}	
						/*************** DEBUG RS232 - PC *****************/
						//sprintf(SerialTX,"'%u' ",max_peak1);
						//WriteUart2((unsigned int*) SerialTX); 
						/*************** END DEBUG *****************/
					}
				}

			
				/* Calc Angle*/ 
				double angle;
				if ( x!= 0)
					angle = atan2(y,x);	
				else 
					angle = 0;
				/*************** DEBUG RS232 - PC *****************/
				//sprintf(SerialTX," --- %u %f\n\r ",dataReceived, angle*180/3.1415);
				//WriteUart2((unsigned int*) SerialTX); 
				/*************** DEBUG RS232 - PC *****************/

				/* Calc Distance*/
				double v1 = (double) max_peak1 * (double) max_peak1;
				v1 *= v1;
				double v2 = (double) max_peak2 * (double) max_peak2;
				v2 *= v2;
				double cos1 = cos(angle - SensorAngleRads[max_sensor1]);
				cos1 *= cos1;
				double cos2 = cos(angle - SensorAngleRads[max_sensor2]);
				cos2 *= cos2;
				v1 /= cos1;
				v2 /= cos2;
				double distance = sqrt(sqrt(v1 + v2));
			
				/* Store Data*/
				//finalDataReceived.data = dataCompleatedIteration[max_sensor1].data;
				finalDataReceived.data = dataReceived;
				finalDataReceived.bearing = (int) (angle * 10000);
				finalDataReceived.range = distance;
				finalDataReceived.max_peak1 = max_peak1;
				finalDataReceived.max_sensor1 = max_sensor1;
				finalDataReceived.angle = angle;
				/*************** DEBUG RS232 - PC *****************/
				//sprintf(SerialTX,"%04X %2f %u \n\r ",finalDataReceived.data, (angle * 180 / 3.1415), finalDataReceived.range);
				//WriteUart2((unsigned int*) SerialTX); 
				/*************** END DEBUG *****************/

				QueueInFinal(&queueFinalReception,finalDataReceived);
				/* Send data to the robot if UART enable*/
				if (GetUart1Communication())
				{
					WriteUart1();
				}


				received = 0;
				counterDisable = 0;
				/*************** DEBUG RS232 - PC *****************/
				//NumberFrames++;
				//sprintf(SerialTX,"Frame: %ld\n\r ",NumberFrames);
				//WriteUart2((unsigned int*) SerialTX); 	
				/*************** END DEBUG *****************/
				for( num_sensors = 0 ; num_sensors < NUM_RECEPTOR_SENSORS ; num_sensors++ )
				{
					//sprintf(SerialTX,"%d ",QueueStatusSingle(&queuePeakReception[i]));
					//WriteUart2((unsigned int*) SerialTX); 
					
					dataCompleatedIteration[num_sensors].enable = TRUE;
					state = STATE_HEADER;
					dataCompleatedIteration[num_sensors].received = 0;
				}
				/*************** DEBUG RS232 - PC *****************/
				//sprintf(SerialTX,"-- %d\n\r ",dataReceived);
				//WriteUart2((unsigned int*) SerialTX); 
				/*************** END DEBUG *****************/
			}
			/* All calcs on robot. He receives all the peaks for a data*/
			else 
			{
				for( i = 0 ; i < NUM_RECEPTOR_SENSORS ; i++ )
				{
					/*************** DEBUG RS232 - PC *****************/
					while(QueueEmptySingle(&queuePeakReception[i]));
					QueueOutSingle(&queuePeakReception[i],&dataCompleatedIteration[i].distance);
					sprintf(SerialTX,"%u  ",dataCompleatedIteration[i].distance);
					WriteUart2((unsigned int*) SerialTX); 
					/*************** END DEBUG *****************/
					//QueueOutSingle(&queuePeakReception[i],&dataCompleatedIteration[i].distance);
				}
				sprintf(SerialTX,"\n\r");
				WriteUart2((unsigned int*) SerialTX); 
				setAllDataReceived (dataCompleatedIteration,dataReceived);
			}			
		}
	}
}

char GetReceptionData ( finalDataRegister *data ){
	return QueueOutFinal(&queueFinalReception,data);
}

char SetReceptionData ( finalDataRegister data ){
	return QueueOutFinal(&queueFinalReception,&data);
}

char GetCalcOnBoard ( void ){
	return calcOnBoard;
}

void SetCalcOnBoard ( char value ){
	calcOnBoard = value;
}


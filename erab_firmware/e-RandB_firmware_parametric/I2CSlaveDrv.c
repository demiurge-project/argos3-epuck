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
#include "registers.h"
#include "I2CSlaveDrv.h"
#include "IREmission.h"
#include "IRReception.h"
#include "SPIMasterDrv.h"
#include "DataLength.h"


#define WAITING 		0
#define WRITE_ADD 		1
#define WRITE_DATA		2
#define READ_STRING		3

dataRegisterReceived dataAll[12];
//unsigned int maxData;
unsigned long maxData;
finalDataRegister finalData;

//unsigned char auxValue=0;
unsigned long auxValue=0;
unsigned char counter;
//Functions prototype
void init_I2C(){
	
	counter=0;
	/* Enable I2C Module */
	I2C1CONbits.I2CEN = 1;
	/* 7 bit Address */
	I2C1CONbits.A10M=0;
	/* Set I2C1 interrupt priority */
	IPC4bits.SI2C1IP = 7;
	/* Reset I2C1 interrupt flag */
	IFS1bits.SI2C1IF = 0;
	/* Enable I2C interrupt */
	IEC1bits.SI2C1IE = 1;
	/* Enable I2C, enable Clock Stretching */
	I2C1CON = 0x9040;
	
	/* Define Addres 0x20 */
	/* From the robot:
		- 0x40 for writing 
		- 0x41 for reading */
		
	I2C1ADD = 0x20;
	init_registers();
}


//unsigned char address, mode, register_add, new_command, indexer;
unsigned char address, mode, register_add, register_add2, indexer;
unsigned char kk[200];

void __attribute__((interrupt,no_auto_psv)) _SI2C1Interrupt(void)
{
	/* Address Matched */
/*	if (counter < 199 ) {
	//	if(I2C1STATbits.D_A == 0)	
	//	{
			kk[counter] = I2C1RCV;
			// Release SCL1 line 
			I2C1CONbits.SCLREL = 1;	
			IFS1bits.SI2C1IF = 0;
			counter++;
	//	}
	}
	else
	{
		counter++;
		counter = 0;
	}
*/

	/* Address Matched */
	if(I2C1STATbits.D_A == 0)	
	{
		/* Dummy Read */
		address = I2C1RCV;
		/* Master requesting read */
		if (I2C1STATbits.R_W == 1)	
		{
			/* Read Register "register_add" and put it to send */
			I2C1TRN = read_register(register_add);	
			/* Release SCL1 line */
			I2C1CONbits.SCLREL = 1;	
			/* Wait till sending compleat */
			while(I2C1STATbits.TBF);
			/* Next Mode could be a reading if it's continuous*/
			mode = READ_STRING;	
		}
		else 
		{
			/* Set Mode in Writing for next interrupt */
			mode = WRITE_ADD;	// set mode for next interrupt
		}
	}
	else{

		/* Current byte is register address to be written */
		if (mode == WRITE_ADD)		
		{	
			/* Save Register Address */
			register_add = I2C1RCV; 		
			/* Release SCL1 line, because stretching is enabled */
			I2C1CONbits.SCLREL = 1;	
			/* Next Mode in writing data for next interrupt */
			mode = WRITE_DATA;		
			/* Increase indexr for string write */
			indexer = 0;				
		}
		/* Current byte is data to be written to register "register_add"*/
		else if (mode == WRITE_DATA)	
		{
			/* Write data into register */
			//write_register(register_add+indexer, I2C1RCV); 
			write_register(register_add, I2C1RCV); 
			/* Release SCL1 line, because stretching is enabled */
			I2C1CONbits.SCLREL = 1;	
			/* Increase indexer for string write */
			//indexer++; 
		}
		/* Current status continuous reading */
		else if (mode == READ_STRING) 
		{
			/* Read Register "register_add" and put it to send */
			I2C1TRN = read_register(register_add+indexer);	
			/* Release SCL1 line */
			I2C1CONbits.SCLREL = 1;	
			/* Wait till sending compleate */
			while(I2C1STATbits.TBF);
			/* Increase indexer for string read */
			indexer++;
		}
	}
	IFS1bits.SI2C1IF = 0;
}

char read_register(char reg_add)
{
	//dataRegisterReceived data;
	//modelledDataRegisterReceived modelData;
	//dataRegisterReceived dataAll[12];
	unsigned int aux;
	switch (reg_add)
	{
		case 0:
			return (GetReceptionData( &finalData ));
			break;
		case 1:
			return ( ( finalData.data >> 8 ) & 0xFF );
			break;
		case 2:
			return ( finalData.data & 0xFF );
			break;
		// Added for 32 bit payload
		case 10:
			return ( ( finalData.data >> 24 ) & 0xFF );
			break;
		case 11:
			return ( ( finalData.data >> 16 ) & 0xFF );
			break;

		case 3:
			return ( ( finalData.bearing >> 8 ) & 0xFF );
			break;
		case 4:
			return ( finalData.bearing & 0xFF );
			break;
		case 5:
			return ( ( finalData.range >> 8 ) & 0xFF );
			break;
		case 6:
			return ( finalData.range & 0xFF );
			break;
		case 7:
			return ( ( finalData.max_peak1 >> 8 ) & 0xFF );
			break;
		case 8:
			return ( finalData.max_peak1 & 0xFF );
			break;
		case 9:
			return ( finalData.max_sensor1 & 0xFF );
			break;

		case 20:
			return getAllDataReceived (dataAll,&maxData);
			break;
		case 21:
			return ((dataAll[0].distance >> 8 ) & 0xFF);
			break;
		case 22:
			 return (dataAll[0].distance & 0xFF);
			break;
		case 23:
			return ((dataAll[1].distance >> 8 ) & 0xFF);
			break;
		case 24:
			 return (dataAll[1].distance & 0xFF);
			break;
		case 25:
			return ((dataAll[2].distance >> 8 ) & 0xFF);
			break;
		case 26:
			 return (dataAll[2].distance & 0xFF);
			break;
		case 27:
			return ((dataAll[3].distance >> 8 ) & 0xFF);
			break;
		case 28:
			 return (dataAll[3].distance & 0xFF);
			break;
		case 29:
			return ((dataAll[4].distance >> 8 ) & 0xFF);
			break;
		case 30:
			 return (dataAll[4].distance & 0xFF);
			break;
		case 31:
			return ((dataAll[5].distance >> 8 ) & 0xFF);
			break;
		case 32:
			 return (dataAll[5].distance & 0xFF);
			break;
		case 33:
			return ((dataAll[6].distance >> 8 ) & 0xFF);
			break;
		case 34:
			 return (dataAll[6].distance & 0xFF);
			break;
		case 35:
			return ((dataAll[7].distance >> 8 ) & 0xFF);
			break;
		case 36:
			 return (dataAll[7].distance & 0xFF);
			break;
		case 37:
			return ((dataAll[8].distance >> 8 ) & 0xFF);
			break;
		case 38:
			 return (dataAll[8].distance & 0xFF);
			break;
		case 39:
			return ((dataAll[9].distance >> 8 ) & 0xFF);
			break;
		case 40:
			 return (dataAll[9].distance & 0xFF);
			break;
		case 41:
			return ((dataAll[10].distance >> 8 ) & 0xFF);
			break;
		case 42:
			 return (dataAll[10].distance & 0xFF);
			break;
		case 43:
			return ((dataAll[11].distance >> 8 ) & 0xFF);
			break;
		case 44:
			return (dataAll[11].distance & 0xFF);
			break;
		case 45:
			return  ((maxData >> 8 ) & 0xFF);  
			break;
		case 46:
			aux = maxData & 0xFF;
			resetAllDataReceived();
			return ( aux & 0xFF);
			break;
		default:
			return 0;
			break;
	}
}

void write_register(char reg_add, unsigned char value)
{
	switch(reg_add){
		/* Change Power Transmission */
		case 12:
			write_SPI( value );
			break;
		
		/* Write All values and send values */
		case 13:
			auxValue = value;
			break;
		case 14:
			WriteAllIrData( (auxValue << 8) + value );
			break;
		// Added for 32 bit payload
		case 19:
			auxValue = ((auxValue << 8) + value );
			break;
		case 20:
			auxValue = ((auxValue << 8) + value );
			break;

		// Added to set the payload size from the epuck
		case 21:
			set_data_length(value);
			break;

		//
		/* Send Value */
		case 15:
			WriteIrData();
			break;
	
		/* Init All Ir Peaks */
		case 16:
			InitAllIrPeaks();
			break;
	
		/* Write one register value if valid, nothing if reg_add > 12*/
		case 17:
			SetCalcOnBoard(value);
			break;
		/* For 0 to 11 for the channel*/
		
		/* Set/Unset UART Communication */
		case 18:
			SetUart1Communication(value);
			break;
		default:
			if(reg_add < 12){
				StoreIrData(reg_add,((auxValue << 8) + value));
			}
			break;			
	}
	return;
}

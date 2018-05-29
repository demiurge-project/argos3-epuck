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


#include <uart.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "UARTDrv.h"
#include "IREmission.h"
#include "registers.h"
#include "I2CSlaveDrv.h"
#include "IRReception.h"
#include "SPIMasterDrv.h"

#define FCY      40000000
#define BAUDRATE 57600                
#define BRGVAL   ((FCY/BAUDRATE)/16)-1 

#define UART_TX_PIN_NORMAL      0xF7FF

#define UART1_RX_WAIT 0
#define UART1_RX_VALUE1 1
#define UART1_RX_VALUE2 2

/* Tx by interruptions */
finalDataRegister sendData;
char* p;
char uart1TXCounter;

/* Rx by Interruptions*/
char uart1RXCounter;
queueSingle UartRxReception;

/* Define UART1 communication */
char UartCommunication = FALSE;

/*Variables for the PC-ROBOT Comunication */
char numBytesReceived = 0;
int bytesReceived[4] ={0,0,0,0};

//Functions prototype
void init_UART2( void ){

	/* Enable UART2 */
	U2MODEbits.UARTEN = 1;
	/* Stop on IDLE*/ 
	U2MODEbits.USIDL = 1;
	/* NO IRDA*/ 
	U2MODEbits.IREN = 0;	
	/* JUST TX AND RX*/ 
	U2MODEbits.UEN = 0;
	/* WAKE_UP disables*/ 
	U2MODEbits.WAKE = 0;
	/* No LoopBack*/ 
	U2MODEbits.LPBACK = 0;
	/* No auto detect baudrate*/ 
	U2MODEbits.ABAUD = 0;
	/* No RX INV. IDLE STATE 1*/ 
	U2MODEbits.URXINV = 0;
	/* HIGH BAUD RATE*/ 
	U2MODEbits.BRGH = 0;
	/* 8 bits no parity*/ 
	U2MODEbits.PDSEL = 0;
	/* 1 Stop Bit*/ 
	U2MODEbits.STSEL = 0;
	
	/* Interrupt with 1 char*/	 
	U2STAbits.UTXISEL1 = 0;
	U2STAbits.UTXISEL0 = 0;
	/* IDLE STATE 0*/ 
	U2STAbits.UTXINV = 0;
	/* Sync Breack Disabled*/ 
	U2STAbits.UTXBRK = 0;
	/* Enable Transimission*/ 
	U2STAbits.UTXEN = 1;
	/* transmit without full buffer*/ 
	U2STAbits.UTXBF = 0;
	/* Interrupt when 1 char received*/ 
	U2STAbits.URXISEL = 0;
	/* No Address detected*/ 
	U2STAbits.ADDEN = 0;

	/* Configure 57600 bauds*/ 
	U2BRG = BRGVAL;
	
	/* Clear TX Interrupt*/ 	
	IFS1bits.U2TXIF = 0;
	/* Clear RX Interrupt*/ 
	IFS1bits.U2RXIF = 0;
	
	/* Enable TX Interrupt*/ 
	IEC1bits.U2TXIE = 1;
	/* Enable RX Interrupt*/ 
	IEC1bits.U2RXIE = 1;

	
}

void close_UART2 ( void )
{
	/* Turn off UART2 module */
    CloseUART2();
}

void WriteUart2 ( unsigned int *data ){
	putsUART2 (data);
	/* Wait for  transmission to complete */
    while(BusyUART2());
}

void __attribute__((__interrupt__, no_auto_psv)) _U2TXInterrupt(void)
{
	IFS1bits.U2TXIF = 0;
}
void __attribute__((__interrupt__,no_auto_psv)) _U2RXInterrupt(void)
{
  	IFS1bits.U2RXIF = 0;
	char input=0;
	/* Read the receive buffer till atleast one or more character can be read */ 
	input = U2RXREG;
	
	char TxData[50];

	/*switch(input){
		case 'a':
			WriteAllIrData(0xAAAA);
			sprintf(TxData,"RECIBIDO a\n\r");
			WriteUart2((unsigned int*) TxData);
			break;
		case 'b':
			WriteAllIrData(0xBBBB);
			sprintf(TxData,"RECIBIDO b\n\r");
			WriteUart2((unsigned int*) TxData);
			break;
		case 'c':
			WriteAllIrData(0xCCCC);
			sprintf(TxData,"RECIBIDO c\n\r");
			WriteUart2((unsigned int*) TxData);
			break;
		case 'd':
			WriteAllIrData(0xDDDD);
			sprintf(TxData,"RECIBIDO d\n\r");
			WriteUart2((unsigned int*) TxData);
			break;
		case 'e':
			WriteAllIrData(0xEEEE);
			sprintf(TxData,"RECIBIDO e\n\r");
			WriteUart2((unsigned int*) TxData);
			break;
		case 'f':
			WriteAllIrData(0xFFFF);	
			sprintf(TxData,"RECIBIDO f\n\r");
			WriteUart2((unsigned int*) TxData);
			break;
		default:
			sprintf(TxData,"ERROR\n\r" );
			WriteUart2((unsigned int*) TxData);
			break;
	}*/
	
	int input2 = 0xFFFF;
	switch(input){	
		case '1':
			input2=0x01;
			break;
		case '2':
			input2=0x02;
			break;
		case '3':
			input2=0x03;
			break;
		case '4':
			input2=0x04;
			break;
		case '5':
			input2=0x05;
			break;
		case '6':
			input2=0x06;
			break;
		case '7':
			input2=0x07;
			break;
		case '8':
			input2=0x08;
			break;
		case '9':
			input2=0x09;
			break;
		case 'a':
			input2=0x0A;
			break;
		case 'b':
			input2=0x0B;
			break;
		case 'c':
			input2=0x0C;
			break;
		case 'd':
			input2=0x0D;
			break;
		case 'e':
			input2=0x0E;
			break;
		case 'f':
			input2=0x0F;
			break;
		case 'A':
			input2=0x0A;
			break;
		case 'B':
			input2=0x0B;
			break;
		case 'C':
			input2=0x0C;
			break;
		case 'D':
			input2=0x0D;
			break;
		case 'E':
			input2=0x0E;
			break;
		case 'F':
			input2=0x0F;
			break;
		default:
			input2 = 0xFFFF;
			break;
	}
	if (input2 <= 0x0F)
	{
		bytesReceived[numBytesReceived] = input2;
		numBytesReceived++;
		if (numBytesReceived >= 4)
		{
			numBytesReceived = 0;
			int frame = (bytesReceived[0]  << 12 ) + ( bytesReceived[1] << 8 ) +
						(bytesReceived[2] << 4 ) + (int) bytesReceived[3];
			WriteAllIrData(frame);
			sprintf(TxData,"Received: %x\n\r",  frame);
			WriteUart2((unsigned int*) TxData);
		}
	}
	else 
	{
		numBytesReceived = 0;
		bytesReceived[0] = 0;
		bytesReceived[1] = 0;
		bytesReceived[2] = 0;
		bytesReceived[3] = 0;

	}
}


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

//Functions prototype
void init_UART1( void ){
	/* Init Rx vars */
	init_queue_single(&UartRxReception);

	/* Enable UART2 */
	U1MODEbits.UARTEN = 1;
	/* Stop on IDLE*/ 
	U1MODEbits.USIDL = 1;
	/* NO IRDA*/ 
	U1MODEbits.IREN = 0;	
	/* JUST TX AND RX*/ 
	U1MODEbits.UEN = 0;
	/* WAKE_UP disables*/ 
	U1MODEbits.WAKE = 0;
	/* No LoopBack*/ 
	U1MODEbits.LPBACK = 0;
	/* No auto detect baudrate*/ 
	U1MODEbits.ABAUD = 0;
	/* No RX INV. IDLE STATE 1*/ 
	U1MODEbits.URXINV = 0;
	/* HIGH BAUD RATE*/ 
	U1MODEbits.BRGH = 0;
	/* 8 bits no parity*/ 
	U1MODEbits.PDSEL = 0;
	/* 1 Stop Bit*/ 
	U1MODEbits.STSEL = 0;
	
	/* Interrupt with 1 char*/	 
	U1STAbits.UTXISEL1 = 0;
	U1STAbits.UTXISEL0 = 0;
	/* IDLE STATE 0*/ 
	U1STAbits.UTXINV = 0;
	/* Sync Breack Disabled*/ 
	U1STAbits.UTXBRK = 0;
	/* Enable Transimission*/ 
	U1STAbits.UTXEN = 1;
	/* transmit without full buffer*/ 
	U1STAbits.UTXBF = 0;
	/* Interrupt when 1 char received*/ 
	U1STAbits.URXISEL = 0;
	/* No Address detected*/ 
	U1STAbits.ADDEN = 0;

	/* Configure 57600 bauds*/ 
	U1BRG = BRGVAL;
	
	/* Clear TX Interrupt*/ 	
	IFS0bits.U1TXIF = 0;
	/* Clear RX Interrupt*/ 
	IFS0bits.U1RXIF = 0;

	/* RX Priority */
	IPC2bits.U1RXIP = 6;	
	/* Enable TX Interrupt*/ 
	IEC0bits.U1TXIE = 1;
	/* Enable RX Interrupt*/ 
	IEC0bits.U1RXIE = 1;


	/* Init Tx vars */
	uart1TXCounter = 0;

	QueueClearSingle(&UartRxReception);
}

void close_UART1 ( void )
{
	/* Turn off UART2 module */
    CloseUART1();
}

void WriteUart1 ( void ){
	/* If something to send */
	if (GetReceptionData( &sendData ))
	{
		/* Point to the beginning of the struct */
		p = (&sendData);
		/* Start the counter */
		uart1TXCounter = 1;
		/*Write first character */
		putcUART1 ( (unsigned char) (*p));
	}
}

void __attribute__((__interrupt__, no_auto_psv)) _U1TXInterrupt(void)
{
	/* Clear Interrupt */
	IFS0bits.U1TXIF = 0;
	/* If not all the struct send, send next character */
	if(uart1TXCounter < 12)
	{
		/* Increase the pointer */
		p++;
		/* Put the value into the tx reg */
		U1TXREG = *p;
		/*Increase the pointer */
		uart1TXCounter++;
	}
}

void __attribute__((__interrupt__,no_auto_psv)) _U1RXInterrupt(void)
{
  	IFS0bits.U1RXIF = 0;
	char input=0;
	/* Read the receive buffer till atleast one or more character can be read */ 
	input = U1RXREG;
	//char TxData[10];
	//sprintf(TxData,"%d\n\r",input);
	//WriteUart2((unsigned int*) TxData);
	/* Store it */
	QueueInSingle(&UartRxReception,(unsigned int) input);
}

void ManageUart1Reception ( void )
{
	char TxData[50];
	if(QueueStatusSingle(&UartRxReception) >= 3)
	{


		unsigned int action;
		QueueOutSingle(&UartRxReception,&action);

		if ( action <= 18 )
		{
			//unsigned int value1,value2,auxValue;
			unsigned int value1,value2,value3,value4 =0;
			unsigned long auxValue;
			if(get_data_length() >= 32){
				QueueOutSingle(&UartRxReception,&value1);
			}
			if(get_data_length() >= 24){
				QueueOutSingle(&UartRxReception,&value2);
			}
			if(get_data_length() >= 16){
				QueueOutSingle(&UartRxReception,&value3);
			}			
			QueueOutSingle(&UartRxReception,&value4);

			switch (action)
			{
				case 15:
					WriteIrData();
					/* DEBUG */
					//sprintf(TxData,"WriteIrData \n\r");
					//WriteUart2((unsigned int*) TxData);
					/* DEBUG */
					break;
				case 16:
					InitAllIrPeaks();
					/* DEBUG */
					sprintf(TxData,"InitAllIrPeaks \n\r");
					WriteUart2((unsigned int*) TxData);
					/* DEBUG */
					break;
				case 12:
					write_SPI(value1&0xFF);
					/* DEBUG */
					sprintf(TxData,"WriteSPI %d \n\r",(value1 & 0xFF));
					WriteUart2((unsigned int*) TxData);
					/* DEBUG */
					break;
				case 17:
					SetCalcOnBoard(value1&0xFF);
					/* DEBUG */
					sprintf(TxData,"SetCalcOnBoard %d \n\r",(value1 & 0xFF));
					WriteUart2((unsigned int*) TxData);
					/* DEBUG */
					break;
				case 14:
					//auxValue = (value1 << 8 ) + (value2 & 0xFF);
					auxValue = ((unsigned long)value1 << 24 ) + ((unsigned long)value2 << 16 ) + (value3 << 8 ) + (value4 & 0xFF);
					WriteAllIrData( auxValue );
					/* DEBUG */
					//sprintf(TxData,"AllIrData %u \n\r",auxValue);
					//WriteUart2((unsigned int*) TxData);
					/* DEBUG */
					break;
				case 18:
					SetUart1Communication(value1&0xFF);
					break;
				default:
					if(action < 12)	
					{
						//auxValue = (value1 << 8 ) + (value2 & 0xFF);
						auxValue = ((unsigned long)value1 << 24 ) + ((unsigned long)value2 << 16 ) + (value3 << 8 ) + (value4 & 0xFF);
						StoreIrData(action,auxValue);
						/* DEBUG */
						//sprintf(TxData,"StoreIrData %u %d \n\r ",auxValue, action);
						//WriteUart2((unsigned int*) TxData);
						/* DEBUG */
					}	
					break;
			}
		}
	}
}

void EmptyRxQueue ( void )
{
	QueueClearSingle(&UartRxReception);
}

void SetUart1Communication ( char value ) 
{
	UartCommunication = value;
}

char GetUart1Communication ( void )
{
	return UartCommunication;
}

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
#include "PWMEmission.h"
#include "ports.h"
#include "queue.h"
#include "stdlib.h"
#include "DataLength.h"

char FrameToSend = FALSE;
int bufferIndex = 0;
unsigned int bufferEmission[FRAME_BITS_32];

queue queueEmission;

long int FramesCounter=0;

/* Counter for the CSMA*/ 
int CSMAcounter = 0;

void init_PWM( void ){
	// Initialize and enable Timer2
	T2CONbits.TON = 0; // Disable Timer
	T2CONbits.TCS = 0; // Select internal instruction cycle clock
	T2CONbits.TGATE = 0; // Disable Gated Timer mode
	TMR2 = 0x00; // Clear timer register
	
	/* Fosc = 40MHz --> Tosc = 25ns
	 * Fcy = 20 MHz --> Tcy = 50ns
	 * Fpwm = 455KHz --> Tpwm = 2.1978us
	 * Tpwm = (PR2 + 1) * Tcy * Preescaler 
	 * 2.1978us = (PR2 + 1) * 50ns * Preescaler 
	 * 
	 * PR2 = 42.95
	 * Preescaler = 1
     * 
	 * PWMresolution(bits) = log10(Fcy/Fpwm)/log10(2)
	 * PWMresolution = 5.46 bits
     * 
     * DutyCycle 50%:
	 * PR2 = 42 --> OC1R and OC1RS = 21
     * 
     * */
	T2CONbits.TCKPS = 0; // Select 1:1 Prescaler
	PR2 = 84; // Load the period value
	
	OC1CONbits.OCM = 0; // Disable Output Compare Module
	OC1R = 42; // Write the duty cycle for the first PWM pulse
	OC1RS = 42; // Write the duty cycle for the second PWM pul
	OC1CONbits.OCTSEL = 0; // Select Timer 2 as output compare time base
	OC1CONbits.OCM = 0b110; // Select the Output Compare mode	
	
	IPC1bits.T2IP = 5; // Set Timer 2 Interrupt Priority Level
	IFS0bits.T2IF = 0; // Clear Timer 2 Interrupt Flag
	IEC0bits.T2IE = 1; // Enable Timer 2 interrupt
	T2CONbits.TON = 1; // Start Timer

	init_queue(&queueEmission);
}

void init_T1( void ){
	/* Timer 1 reset */
	T1CONbits.TON = 0;
	/* Select internal clock */
	T1CONbits.TCS = 0;
	/* Disable Gated Timer mode*/
	T1CONbits.TGATE = 0; 
	/* Set Timer1 interrupt priority to 4 */
	IPC0bits.T1IP = 4;
	/* Reset Timer1 interrupt flag */
	IFS0bits.T1IF = 0;
	/* Enable Timer1 interrupt */
	IEC0bits.T1IE = 1;
	
	T1CONbits.TCKPS = 0; //Preescaler 1:1
	/* Tcy = 50ns --> 
	 * Tirdata(20Khz) = 50us
	 * PR1 = Tirdata / Tcy = 1000 */
	TMR1=  0x0000;  	
	PR1 = 4000;           // Timer1 period register = ?????
	/* Enable Timer1 and start the counter */
	T1CONbits.TON = 1;      
	
}
void init_Emmision( void ){
	init_PWM();
	init_T1();
}

/* Example code for Timer 2 ISR*/
void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt( void )
{
	/* Interrupt Service Routine code goes here */
	OC1RS = 42; // Write Duty Cycle value for next PWM cycle
	IFS0bits.T2IF = 0; // Clear Timer 2 interrupt flag
}

void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void)
{
	/* Reset comparator */
   	TMR1 = 0;

	int aux;
	/* Read Emission Port Actual Value*/
	aux = EMPort;
	/* Mask Port with no emission used bits */
	aux &= 0x00C6;	
	int frame_bits = getFrameBits(get_data_length());

	if(FrameToSend == TRUE){
		/* Load Value to Send */
		aux |= bufferEmission[bufferIndex];
		/* Increase Pointer */
		bufferIndex++;
		/* If Frame finished Clear FrameToSend Flag*/
		if(bufferIndex == frame_bits){
			bufferIndex = 0;
			FrameToSend = FALSE;
		}
	}
	else{
		#ifdef _CSMA_
			if ( CSMAcounter <= 0)
			{
				if(mediumStatus == IDLE)
				{
					FrameToSend = GetEmissionFrame(bufferEmission);
				}
				else {
					double r = ((double)rand()/((double)(RAND_MAX)+(double)(1)) ); 
		    		CSMAcounter=(int)(r*(16));
				}
			}
			else{
				CSMAcounter--;
			}
		#else

			FrameToSend = GetEmissionFrame(bufferEmission);
			/*************** DEBUG RS232 - PC *****************/
			//if(FrameToSend==TRUE)
			//{
			//	FramesCounter++;
			//	char SerialTX[50];
			//	sprintf(SerialTX,"Frame: %ld\n\r ",FramesCounter);
			//	WriteUart2((unsigned int*) SerialTX); 		
			//}
			/*************** END DEBUG *****************/

		#endif
	}	
	EMPort = aux;

	/* Clear Timer1 interrupt */
	IFS0bits.T1IF = 0;
}

int getFrameBits(int frameDataLength){
	return 2*(HEADER_LENGTH + frameDataLength + CRC_LENGTH);
}

char GetEmissionFrame ( unsigned int *frame ){
	return QueueOut(&queueEmission,frame);
}

char SetEmissionFrame ( unsigned int *frame){
	return QueueIn(&queueEmission,frame);
}

void re_init_queue_emission(){
	init_queue(&queueEmission);
}

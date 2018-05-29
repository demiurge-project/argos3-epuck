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
#include "ADCDrv.h"
#include <adc.h>
#include <delay.h>

//Functions prototype
void init_ADC( void ){
	/* Using AN20 - AN31 */
	AD1PCFGH = 0x000F;

	/* Internal Vdd */
	AD1CON2bits.VCFG = 0;
	/* Data Output Integer */
	AD1CON1bits.FORM   = 0;
	/*Internal counter ends sampling and starts conversion (auto-convert)*/
	AD1CON1bits.SSRC   = 7;
	/*Sampling begins when SAMP bit is set*/
	AD1CON1bits.ASAM   = 0;
	/* 12-bit ADC operation */
	AD1CON1bits.AD12B  = 1;		

	/* Internal Voltage Reference*/
	AD1CON2bits.VCFG = 0;
	/* */
	AD1CON2bits.CSCNA = 0;

	/* Using standard clock*/
	AD1CON3bits.ADRC = 0;
	/* REVIEW --------!!!!!!!!!!
	   ADC Conversion Clock Tad=Tcy*(ADCS+1)= (1/20M)*1 = 50ns (20Mhz)
	   ADC Conversion Time for 12-bit Tc=14*Tab = 0.7us */
	AD1CON3bits.ADCS = 10;
	 
    IEC0bits.AD1IE = 0;			// Enable A/D interrupt 
    AD1CON1bits.ADON = 1;		// Turn on the A/D converter	
	
}
void close_ADC ( void )
{
}
int ReadADC ( int channel )
{
	unsigned int result=0;
	
	AD1CHS0bits.CH0SA = channel;
	AD1CON1bits.SAMP = 1; // start sampling ...
	//Delay( 100 * Delay_1mS_Cnt );
	int i;
	for(i=0; i< 1000; i++){__asm__("nop");}
	AD1CON1bits.SAMP = 0; // start Converting
	while (!AD1CON1bits.DONE);// conversion done?
	result = ADC1BUF0; // yes then get ADC value

	return result;
}

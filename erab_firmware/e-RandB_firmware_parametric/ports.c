#include "ports.h"

void init_ports( void ){

	/* IR EMISSION */
	EM1_CONF = OUTPUT_PIN;
	EM2_CONF = OUTPUT_PIN;
	EM3_CONF = OUTPUT_PIN;
	EM4_CONF = OUTPUT_PIN;
	EM5_CONF = OUTPUT_PIN;
	EM6_CONF = OUTPUT_PIN;
	EM7_CONF = OUTPUT_PIN;
	EM8_CONF = OUTPUT_PIN;
	EM9_CONF = OUTPUT_PIN;
	EM10_CONF = OUTPUT_PIN;
	EM11_CONF = OUTPUT_PIN;
	EM12_CONF = OUTPUT_PIN;

	EM1 = LOW;
	EM2 = LOW;
	EM3 = LOW;
	EM4 = LOW;
	EM5 = LOW;
	EM6 = LOW;
	EM7 = LOW;
	EM8 = LOW;
	EM9 = LOW;
	EM10 = LOW;
	EM11 = LOW;
	EM12 = LOW;

	/* IR RECEPTION DATA */
	RE1_CONF = INPUT_PIN;
	RE2_CONF = INPUT_PIN;
	RE3_CONF = INPUT_PIN;
	RE4_CONF = INPUT_PIN;
	RE5_CONF = INPUT_PIN;
	RE6_CONF = INPUT_PIN;
	RE7_CONF = INPUT_PIN;
	RE8_CONF = INPUT_PIN;
	RE9_CONF = INPUT_PIN;
	RE10_CONF = INPUT_PIN;
	RE11_CONF = INPUT_PIN;
	RE12_CONF = INPUT_PIN;

	/*IR RECEPTION RESET PEAK */
	RESET_PEAK1_CONF = OUTPUT_PIN;
	RESET_PEAK2_CONF = OUTPUT_PIN;
	RESET_PEAK3_CONF = OUTPUT_PIN;
	RESET_PEAK4_CONF = OUTPUT_PIN;
	RESET_PEAK5_CONF = OUTPUT_PIN;
	RESET_PEAK6_CONF = OUTPUT_PIN;
	RESET_PEAK7_CONF = OUTPUT_PIN;
	RESET_PEAK8_CONF = OUTPUT_PIN;
	RESET_PEAK9_CONF = OUTPUT_PIN;
	RESET_PEAK10_CONF = OUTPUT_PIN;
	RESET_PEAK11_CONF = OUTPUT_PIN;
	RESET_PEAK12_CONF = OUTPUT_PIN;

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



	/* Config test led as output */
	LED0_CONF = OUTPUT_PIN;
	/* Config ir emission power enable as output */
	IRPOWER_CONF = OUTPUT_PIN;


	/* Init test led as OFF */
	LED0 = LOW;
	/* Init ir emission power enable as ON */
	IRPOWER = ON;

	TRISGbits.TRISG2 = INPUT_PIN;
	TRISGbits.TRISG3 = INPUT_PIN;
	
	RCONbits.SWDTEN=0;            /* Disable Watch Dog Timer*/
	// Configure Oscillator to operate the device at 40Mhz
	// Fosc= Fin*M/(N1*N2), Fcy=Fosc/2
	// Fosc= 4M*80/(4*2)=40Mhz for 4M input clock
	//PLLFBD=80;					// M=80
	PLLFBD=80;					// M=160
	CLKDIVbits.PLLPOST=0b00;	// N1=4
	CLKDIVbits.PLLPRE=0;		// N2=2

	// Clock switching to incorporate PLL
	__builtin_write_OSCCONH(0x03);		// Initiate Clock Switch to Primary
									    // Oscillator with PLL (NOSC=0b011)
	__builtin_write_OSCCONL(0x01);		// Start clock switching
	while (OSCCONbits.COSC != 0b011);	// Wait for Clock switch to occur
}

#include "p33FJ256GP710.h"

#define OUTPUT_PIN 	0
#define INPUT_PIN 	1

#define HIGH  		1
#define LOW  		0

#define ON			1
#define OFF			0

#define TRUE 		1
#define FALSE		0

#define NOT_IDLE 	1
#define IDLE		0

#define ON_BOARD	0
#define ON_ROBOT	1

/* TEST LED*/
#define LED0_CONF 	TRISAbits.TRISA3
#define LED0 		LATAbits.LATA3

/*IR EMISSION POWER SUPPLY ENABLE */
#define IRPOWER_CONF	TRISFbits.TRISF12
#define IRPOWER			LATFbits.LATF12

/* IR Emmision */
#define EM1_CONF		TRISBbits.TRISB3
#define EM2_CONF		TRISBbits.TRISB4
#define EM3_CONF		TRISBbits.TRISB5
#define EM4_CONF		TRISBbits.TRISB8
#define EM5_CONF		TRISBbits.TRISB9
#define EM6_CONF		TRISBbits.TRISB10
#define EM7_CONF		TRISBbits.TRISB11
#define EM8_CONF		TRISBbits.TRISB12
#define EM9_CONF		TRISBbits.TRISB13
#define EM10_CONF		TRISBbits.TRISB14
#define EM11_CONF		TRISBbits.TRISB15
#define EM12_CONF		TRISBbits.TRISB0

#define EM1			LATBbits.LATB3
#define EM2			LATBbits.LATB4
#define EM3			LATBbits.LATB5
#define EM4			LATBbits.LATB8
#define EM5			LATBbits.LATB9
#define EM6			LATBbits.LATB10
#define EM7			LATBbits.LATB11
#define EM8			LATBbits.LATB12
#define EM9			LATBbits.LATB13
#define EM10		LATBbits.LATB14
#define EM11		LATBbits.LATB15
#define EM12		LATBbits.LATB0

#define EMPort		LATB

/* IR RECEPTION DATA */

#define RE1_CONF		TRISDbits.TRISD4
#define RE2_CONF		TRISDbits.TRISD5
#define RE3_CONF		TRISDbits.TRISD6
#define RE4_CONF		TRISDbits.TRISD7
#define RE5_CONF		TRISDbits.TRISD8
#define RE6_CONF		TRISDbits.TRISD9
#define RE7_CONF		TRISDbits.TRISD10
#define RE8_CONF		TRISDbits.TRISD11
#define RE9_CONF		TRISDbits.TRISD12
#define RE10_CONF		TRISDbits.TRISD13
#define RE11_CONF		TRISDbits.TRISD14
#define RE12_CONF		TRISDbits.TRISD15

#define RE1				PORTDbits.RD4
#define RE2				PORTDbits.RD5
#define RE3				PORTDbits.RD6
#define RE4				PORTDbits.RD7
#define RE5				PORTDbits.RD8
#define RE6				PORTDbits.RD9
#define RE7				PORTDbits.RD10
#define RE8				PORTDbits.RD11
#define RE9				PORTDbits.RD12
#define RE10			PORTDbits.RD13
#define RE11			PORTDbits.RD14
#define RE12			PORTDbits.RD15

#define REPort 			PORTD

/*IR RECEPTION RESET PEAK */
#define RESET_PEAK1_CONF		TRISFbits.TRISF0
#define RESET_PEAK2_CONF		TRISFbits.TRISF1
#define RESET_PEAK3_CONF		TRISGbits.TRISG0
#define RESET_PEAK4_CONF		TRISGbits.TRISG1
#define RESET_PEAK5_CONF		TRISGbits.TRISG12
#define RESET_PEAK6_CONF		TRISGbits.TRISG13
#define RESET_PEAK7_CONF		TRISGbits.TRISG14
#define RESET_PEAK8_CONF		TRISGbits.TRISG15
#define RESET_PEAK9_CONF		TRISAbits.TRISA5
#define RESET_PEAK10_CONF		TRISAbits.TRISA4
#define RESET_PEAK11_CONF		TRISAbits.TRISA1
#define RESET_PEAK12_CONF		TRISAbits.TRISA0

#define RESET_PEAK1				LATFbits.LATF0
#define RESET_PEAK2				LATFbits.LATF1
#define RESET_PEAK3				LATGbits.LATG0
#define RESET_PEAK4				LATGbits.LATG1
#define RESET_PEAK5				LATGbits.LATG12
#define RESET_PEAK6				LATGbits.LATG13
#define RESET_PEAK7				LATGbits.LATG14
#define RESET_PEAK8				LATGbits.LATG15
#define RESET_PEAK9				LATAbits.LATA5
#define RESET_PEAK10			LATAbits.LATA4
#define RESET_PEAK11			LATAbits.LATA1
#define RESET_PEAK12			LATAbits.LATA0


void init_ports(void);

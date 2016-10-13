/********************************************************************************

			fonctions for simple LED manipulation
			Version 1.0 november 2005
			Michael Bonani, Francesco Mondada, Davis Dadie


This file is part of the e-puck library license.
See http://www.e-puck.org/index.php?option=com_content&task=view&id=18&Itemid=45

(c) 2005-2007 Michael Bonani, Francesco Mondada, Davis Dadie

Robotics system laboratory http://lsro.epfl.ch
Laboratory of intelligent systems http://lis.epfl.ch
Swarm intelligent systems group http://swis.epfl.ch
EPFL Ecole polytechnique federale de Lausanne http://www.epfl.ch

**********************************************************************************/

/*! \file
 * \ingroup motor_LED
 * \brief Manage the LEDs.
 * \n A little exemple for the LEDs (all the LEDs are blinking)
 * \code
 * #include <p30f6014A.h>
 * #include <motor_led/e_epuck_ports.h>
 * #include <motor_led/e_init_port.h>
 * #include <motor_led/e_led.h>
 * 
 * int main(void)
 * {
 * 	e_init_port();
 * 	while(1)
 * 	{
 * 		long i;
 * 		for(i=0; i<500000; i++)
 * 			asm("NOP");
 * 		e_set_led(8, 2); //switch the state of all leds
 * 	}
 * }
 * \endcode
 * \author Code: Michael Bonani, Francesco Mondada, Davis Dadie \n Doc: Jonathan Besuchet
 */

#include "e_epuck_ports.h"
#include "e_led.h"

/*! \brief turn on/off the specified LED
 *
 * The e-puck has 8 green LEDs. With this function, you can
 * change the state of these LEDs.
 * \param led_number between 0 and 7
 * \param value 0 (off), 1 (on) otherwise change the state
 * \warning if led_number is other than 0-7, all leds are set
 * to the indicated value.
 */
void e_set_led(unsigned int led_number, unsigned int value)
{
	switch(led_number)
	{
		case 0: 
			{
			if(value>1)
				LED0 = LED0^1; // "^" exclusif OR bit to bit
			else
				LED0 = value;
			break;
			}
		case 1: 
			{
			if(value>1)
				LED1 = LED1^1;
			else
				LED1 = value;
			break;
			}
		case 2: 
			{
			if(value>1)
				LED2 = LED2^1;
			else
				LED2 = value;
			break;
			}
		case 3: 
			{
			if(value>1)
				LED3 = LED3^1;
			else
				LED3 = value;
			break;
			}
		case 4: 
			{
			if(value>1)
				LED4 = LED4^1;
			else
				LED4 = value;
			break;
			}
		case 5: 
			{
			if(value>1)
				LED5 = LED5^1;
			else
				LED5 = value;
			break;
			}
		case 6: 
			{
			if(value>1)
				LED6 = LED6^1;
			else
				LED6 = value;
			break;
			}
		case 7: 
			{
			if(value>1)
				LED7 = LED7^1;
			else
				LED7 = value;
			break;
			}
		default:
			if(value > 1)
			{
				LED0 = LED0^1; LED1 = LED1^1; LED2 = LED2^1; LED3 = LED3^1;
				LED4 = LED4^1; LED5 = LED5^1; LED6 = LED6^1; LED7 = LED7^1;
			}	
			else
				LED0 = LED1 = LED2 = LED3 = LED4 = LED5 = LED6 = LED7 = value;
	}
}

/*! \brief turn on/off the body LED
 *
 * The e-puck has a green LED that illuminate his body. With this function,
 * you can change the state of this LED.
 * \param value 0 (off), 1 (on) otherwise change the state
 */
void e_set_body_led(unsigned int value)
{
	if(value>1)
		BODY_LED = BODY_LED^1;
	else
		BODY_LED = value;
}

/*! \brief turn on/off the front LED
 *
 * The e-puck has a red LED in the front. With this function, you can
 * change the state of this LED.
 * \param value 0 (off), 1 (on) otherwise change the state
 */
void e_set_front_led(unsigned int value)
{
	if(value>1)
		FRONT_LED = FRONT_LED^1;
	else
	FRONT_LED = value;
}

/*! \brief turn off the 8 LEDs
 *
 * The e-puck has 8 green LEDs. This function turn all off.
 * \warning this function doesn't turn off "body LED" and "front LED".
 */
void e_led_clear(void)
{
	LED0 = 0;
	LED1 = 0;
	LED2 = 0;
	LED3 = 0;
	LED4 = 0;
	LED5 = 0;
	LED6 = 0;
	LED7 = 0;
}

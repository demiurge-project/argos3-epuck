/********************************************************************************

			fonctions for simple LED manipulation					
			december 2004: first example for microinfo				
			by Francesco Mondadaa 


This file is part of the e-puck library license.
See http://www.e-puck.org/index.php?option=com_content&task=view&id=18&Itemid=45

(c) 2004-2007 Francesco Mondada

Robotics system laboratory http://lsro.epfl.ch
Laboratory of intelligent systems http://lis.epfl.ch
Swarm intelligent systems group http://swis.epfl.ch
EPFL Ecole polytechnique federale de Lausanne http://www.epfl.ch

**********************************************************************************/

/*! \file
 * \ingroup motor_LED
 * \brief Manage the LEDs with blinking possibility (timer1, 2, 3).
 *
 * Here we use the fast agenda solution to make the LED blinking.
 * \sa e_agenda.h
 * \author Code: Francesco Mondada \n Doc: Jonathan Besuchet
 */

#include "../../e_epuck_ports.h"
#include "e_agenda_fast.h"


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
// led_number between 0 and 7, value 0 (off) or 1 (on)
// if led_number other than 0-7, all leds are set to value
{
	switch(led_number)
	{
		case 0: 
			{
			if(value>1)
				LED0 = LED0^1;
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
			LED0 = LED1 = LED2 = LED3 = LED4 = LED5 = LED6 = LED7 = value;
	}
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

/*! \brief turn on/off the body LED
 *
 * The e-puck has a green LED that illuminate his body. With this function,
 * you can change the state of these LED.
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
 * change the state of these LED.
 * \param value 0 (off), 1 (on) otherwise change the state
 */
void e_set_front_led(unsigned int value)
{
	if(value>1)
		FRONT_LED = FRONT_LED^1;
	else
	FRONT_LED = value;
}

/*! \brief Change the state of all LED
 *
 * Callback function for an agenda.
 * \sa AgendaType
 */
void e_blink_led(void)
{
	LED0 = ~LED0;
	LED1 = ~LED1;
	LED2 = ~LED2;
	LED3 = ~LED3;
	LED4 = ~LED4;
	LED5 = ~LED5;
	LED6 = ~LED6;
	LED7 = ~LED7;
}

/*! \brief Change the state of LED0
 *
 * Callback function for an agenda.
 * \sa AgendaType
 */
void e_blink_led0(void)
{
	LED0 = ~LED0;
}

/*! \brief Change the state of LED1
 *
 * Callback function for an agenda.
 * \sa AgendaType
 */
void e_blink_led1(void)
{
	LED1 = ~LED1;
}

/*! \brief Change the state of LED2
 *
 * Callback function for an agenda.
 * \sa AgendaType
 */
void e_blink_led2(void)
{
	LED2 = ~LED2;
}

/*! \brief Change the state of LED3
 *
 * Callback function for an agenda.
 * \sa AgendaType
 */
void e_blink_led3(void)
{
	LED3 = ~LED3;
}

/*! \brief Change the state of LED4
 *
 * Callback function for an agenda.
 * \sa AgendaType
 */
void e_blink_led4(void)
{
	LED4 = ~LED4;
}

/*! \brief Change the state of LED5
 *
 * Callback function for an agenda.
 * \sa AgendaType
 */
void e_blink_led5(void)
{
	LED5 = ~LED5;
}

/*! \brief Change the state of LED6
 *
 * Callback function for an agenda.
 * \sa AgendaType
 */
void e_blink_led6(void)
{
	LED6 = ~LED6;
}

/*! \brief Change the state of LED7
 *
 * Callback function for an agenda.
 * \sa AgendaType
 */
void e_blink_led7(void)
{
	LED7 = ~LED7;
}

/*! \brief Start blinking all LED
 *
 * \param cycle	   the number of cycle we wait before launching "e_blink_led()"
 * \sa e_blink_led, e_activate_agenda
 */
void e_start_led_blinking(int cycle)
{
  e_activate_agenda(e_blink_led, cycle);
}

/*! \brief Stop blinking all LED
 *
 * This function use e_destroy_agenda(void (*func)(void))
 * \sa e_destroy_agenda
 */
void e_stop_led_blinking(void)
{
  e_destroy_agenda(e_blink_led);
}

/*! \brief Change the blinking speed
 *
 * This function use e_set_agenda_cycle(void (*func)(void), int cycle)
 * \param cycle	   the number of cycle we wait before launching "e_blink_led()"
 * \sa e_blink_led, e_set_agenda_cycle
 */
void e_set_blinking_cycle(int cycle)
{
	if (cycle>=0)
		e_set_agenda_cycle(e_blink_led,cycle);
}

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

#ifndef _LED
#define _LED

/* functions */
void e_set_led(unsigned int led_number, unsigned int value); // set led_number (0-7) to value (0-1)
void e_led_clear(void);
void e_blink_led(void);
void e_blink_led0(void);
void e_blink_led1(void);
void e_blink_led2(void);
void e_blink_led3(void);
void e_blink_led4(void);
void e_blink_led5(void);
void e_blink_led6(void);
void e_blink_led7(void);

void e_set_body_led(unsigned int value); // value (0=off 1=on higher=inverse) 
void e_set_front_led(unsigned int value); //value (0=off 1=on higher=inverse) 

void e_start_led_blinking(int cycle);
void e_stop_led_blinking(void);

#endif

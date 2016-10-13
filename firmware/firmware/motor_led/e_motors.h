/********************************************************************************

			Control motor of e-puck	with timer 4 and 5
			December 2004: first version							
			Lucas Meier & Francesco Mondada  						
			Version 1.0 november 2005								
			Michael Bonani


This file is part of the e-puck library license.
See http://www.e-puck.org/index.php?option=com_content&task=view&id=18&Itemid=45

(c) 2005-2007 Michael Bonani, Francesco Mondada, Lucas Meier

Robotics system laboratory http://lsro.epfl.ch
Laboratory of intelligent systems http://lis.epfl.ch
Swarm intelligent systems group http://swis.epfl.ch
EPFL Ecole polytechnique federale de Lausanne http://www.epfl.ch

**********************************************************************************/

/*! \file
 * \ingroup motor_LED
 * \brief Manage the motors (with timer 4 and 5).
 *
 * This module manage the motors with two timers: timer4 (motor left)
 * and timer5 (motor right).
 * \warning You can't use this module to control the motors if you are
 * using the camera, because the camera's module also use timer4 and
 * timer5.
 *
 * A little exemple for the motors (e-puck turn on himself)
 * \code
 * #include <p30f6014A.h>
 * #include <motor_led/e_epuck_ports.h>
 * #include <motor_led/e_init_port.h>
 * #include <motor_led/e_motors.h>
 * 
 * int main(void)
 * {
 * 	e_init_motors();
 * 	e_set_speed_left(500); //go forward on half speed
 * 	e_set_speed_right(-500); //go backward on half speed
 * 	while(1) {}
 * }
 * \endcode
 * \author Code: Michael Bonani, Francesco Mondada, Lucas Meier \n Doc: Jonathan Besuchet
 */

#ifndef _MOTORS
#define _MOTORS

/* functions */

void e_init_motors(void); // init to be done before using the other calls

void e_set_speed_left(int motor_speed); 	// motor speed in steps/s
void e_set_speed_right(int motor_speed);  	// motor speed in steps/s
int e_get_steps_left(void);					// motors steps done left
int e_get_steps_right(void);				// motors steps done right
void e_set_steps_left(int set_steps);		// set motor steps counter
void e_set_steps_right(int set_steps);		// set motor steps counter
#endif

/********************************************************************************

			Advance control motor of e-puck							
			December 2004: first version							
			Basic examples from Lucas Meier & Francesco Mondada		
			Adaptation, formatting and test by Francesco Mondadaa 


This file is part of the e-puck library license.
See http://www.e-puck.org/index.php?option=com_content&task=view&id=18&Itemid=45

(c) 2004-2007 Francesco Mondada, Lucas Meier

Robotics system laboratory http://lsro.epfl.ch
Laboratory of intelligent systems http://lis.epfl.ch
Swarm intelligent systems group http://swis.epfl.ch
EPFL Ecole polytechnique federale de Lausanne http://www.epfl.ch

**********************************************************************************/

/*! \file
 * \ingroup motor_LED
 * \brief Manage the motors (with timer1, 2, 3)
 *
 * This module manage the motors with the fast agenda solution (timer1, 2, 3).
 * \sa e_agenda.h
 * \author Code: Francesco Mondada, Lucas Meier \n Doc: Jonathan Besuchet
 */

#ifndef _MOTORS
#define _MOTORS

/* internal functions */
//void run_left_motor(void);
//void run_right_motor(void);

/* user called function */
void e_init_motors(void); 				// init to be done before using the other calls

void e_set_speed_left(int motor_speed);  	// motor speed in percent
void e_set_speed_right(int motor_speed);	// motor speed in percent
void e_set_speed(int linear_speed, int angular_speed);

void e_set_steps_left(int steps_left);
void e_set_steps_right(int steps_right);

int e_get_steps_left();
int e_get_steps_right();

#endif

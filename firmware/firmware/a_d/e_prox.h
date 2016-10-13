/********************************************************************************

			Accessing proximity sensor of e-puck with timer 1
			December 2004: first version Lucas Meier & Francesco Mondada
			Version 1.0 november 2005 Michael Bonani


This file is part of the e-puck library license.
See http://www.e-puck.org/index.php?option=com_content&task=view&id=18&Itemid=45

(c) 2005-2006 Lucas Meier & Francesco Mondada
(c) 2005-2007 Michael Bonani

Robotics system laboratory http://lsro.epfl.ch
Laboratory of intelligent systems http://lis.epfl.ch
Swarm intelligent systems group http://swis.epfl.ch
EPFL Ecole polytechnique federale de Lausanne http://www.epfl.ch

**********************************************************************************/

/*! \file
 * \ingroup a_d
 * \brief Accessing proximity sensor of e-puck with timer 1.
 *
 * The functions of this file are made to deal with the proximity
 * sensor data. You can know the value of the ambient light detected
 * by the sensor. You can estimate the distance between the e-puck
 * and an obstacle by using \ref e_get_prox(unsigned int sensor_number) function.
 *
 * A little exemple which turn the LED0 when an obstacle is detected
 * by the proximity sensor number 0.
 * \code
 * #include <p30f6014A.h>
 * #include <motor_led/e_epuck_ports.h>
 * #include <motor_led/e_init_port.h>
 * #include <a_d/e_prox.h>
 * 
 * int main(void)
 * {
 * 	int value;
 * 	e_init_port();
 * 	e_init_prox();
 * 	while(1)
 * 	{
 * 		long i;
 * 		value = e_get_prox(0);
 * 		if(value > 1000)	//LED0 on if an obstacle is detected by proxy0
 * 			LED0 = 1;
 * 		else
 * 			LED0 = 0;
 * 		for(i=0; i<100000; i++) { asm("nop"); }
 * 	}
 * }
 * \endcode
 * \warning This module uses the timer1
 * \author Code: Lucas Meier & Francesco Mondada, Michael Bonani \n Doc: Jonathan Besuchet
 */

#ifndef _PROX
#define _PROX


/* functions */

void e_init_prox(void);   // to be called before starting using prox
void e_stop_prox(void); //Stop the timer and put pusle to 0
int e_get_prox(unsigned int sensor_number); // to get a prox value
int e_get_ambient_light(unsigned int sensor_number); // to get ambient light value


#endif

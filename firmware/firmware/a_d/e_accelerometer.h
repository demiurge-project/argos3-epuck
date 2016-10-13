/********************************************************************************

			Accelerometer sensor of e-puck
			Version 1.0 november 2005


This file is part of the e-puck library license.
See http://www.e-puck.org/index.php?option=com_content&task=view&id=18&Itemid=45

(c) 2005-2007 Michael Bonani

Robotics system laboratory http://lsro.epfl.ch
Laboratory of intelligent systems http://lis.epfl.ch
Swarm intelligent systems group http://swis.epfl.ch
EPFL Ecole polytechnique federale de Lausanne http://www.epfl.ch

**********************************************************************************/

/*! \file
 * \ingroup a_d
 * \brief Accessing the accelerometer sensor data.
 *
 * A little exemple to read the accelerator.
 * \code
 * #include <p30f6014A.h>
 * #include <motor_led/e_epuck_ports.h>
 * #include <motor_led/e_init_port.h>
 * #include <a_d/e_accelerometer.h>
 * 
 * int main(void)
 * {
 * 	int x, y, z;
 * 	e_init_port();
 * 	e_init_acc();
 * 	while(1)
 * 	{
 * 		long i;
 * 		e_get_acc(&x, &y, &z);
 * 		if(z < 2100)	//LED4 on if e-puck is on the back
 * 		{
 * 			LED0 = 0;
 * 			LED4 = 1;
 * 		}
 * 		else		//LED0 on if e-puck is on his wells
 * 		{
 * 			LED0 = 1;
 * 			LED4 = 0;
 * 		}
 * 		for(i=0; i<100000; i++) { asm("nop"); }
 * 	}
 * }
 * \endcode
 * \author Code: Michael Bonani \n Doc: Jonathan Besuchet
 */

#ifndef _ACCELEROMETER
#define _ACCELEROMETER


/* functions */

void e_init_acc(void);   // to be called before starting using accelerometer
void e_get_acc(int *x, int *y, int *z); // to get analog value of accelerometer
#endif

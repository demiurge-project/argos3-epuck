/********************************************************************************

			Microphone sensor of e-puck	
			Version 1.0 January 2006


This file is part of the e-puck library license.
See http://www.e-puck.org/index.php?option=com_content&task=view&id=18&Itemid=45

(c) 2006-2007 Michael Bonani

Robotics system laboratory http://lsro.epfl.ch
Laboratory of intelligent systems http://lis.epfl.ch
Swarm intelligent systems group http://swis.epfl.ch
EPFL Ecole polytechnique federale de Lausanne http://www.epfl.ch

**********************************************************************************/

/*! \file
 * \ingroup a_d
 * \brief Accessing the microphone data.
 *
 * A little exemple which takes the volume of micro1 and if the
 * sound level is more than 2000. The LED1 is turned on.
 * \code
 * #include <p30f6014A.h>
 * #include <motor_led/e_epuck_ports.h>
 * #include <motor_led/e_init_port.h>
 * #include <a_d/e_micro.h>
 * 
 * int main(void)
 * {
 * 	int m1, m2, m3;
 * 	e_init_port();
 * 	e_init_micro();
 * 	while(1)
 * 	{
 * 		long i;
 * 		e_get_micro(&m1, &m2, &m3);
 * 		if(m1 < 2000)
 * 			LED0 = 0;
 * 		else
 * 			LED0 = 1;
 * 		for(i=0; i<100000; i++) { asm("nop"); }
 * 	}
 * }
 * \endcode
 * \author Code: Michael Bonani \n Doc: Jonathan Besuchet
 */

#include "e_ad_conv.h"
#include "./../motor_led/e_epuck_ports.h"
#include "e_micro.h"

/*! \brief Init the microphone A/D converter
 * \warning Must be called before starting using microphone
 */
void e_init_micro(void)
{
	e_init_ad();	// init AD converter module
}

/*! \brief To get the m0, m1, m2 microphones's values
 * \param m0 A pointer to store the m0 analogic value
 * \param m1 A pointer to store the m1 analogic value
 * \param m2 A pointer to store the m2 analogic value
 */
void e_get_micro(int *m0, int *m1,int *m2)
{
	T1CONbits.TON = 0;//cut of proximity timer
	*m0=e_read_ad(MIC1);
	*m1=e_read_ad(MIC2);
	*m2=e_read_ad(MIC3);
	T1CONbits.TON = 1;
}
				

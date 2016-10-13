/********************************************************************************

			Analogic/Digital conversion	
			December 2004: first version Lucas Meier & Francesco Mondada
			Version 1.0 november 2005 Michael Bonani


This file is part of the e-puck library license.
See http://www.e-puck.org/index.php?option=com_content&task=view&id=18&Itemid=45

(c) 2004-2006 Lucas Meier & Francesco Mondada
(c) 2006-2007 Michael Bonani

Robotics system laboratory http://lsro.epfl.ch
Laboratory of intelligent systems http://lis.epfl.ch
Swarm intelligent systems group http://swis.epfl.ch
EPFL Ecole polytechnique federale de Lausanne http://www.epfl.ch

**********************************************************************************/

/*! \file
 * \ingroup a_d
 * \brief Module for the Analogic/Digital conversion.
 * \author Code: Lucas Meier & Francesco Mondada, Michael Bonami \n Doc: Jonathan Besuchet
 */

/*! \defgroup a_d Analogic/Digital conversion (ADC) 
 * 
 * \section intro_sec Introduction
 * The microcontroller p30f6014A has a 12-bit Analog-to-Digital
 * Converter (ADC). This package is built to manage this ADC.
 * \n The e-puck has three peripherals which take advantage from it.
 * - 1 3D accelerometer
 * - 8 proximity sensors
 * - 3 microphones
 *
 * \section organisation_sec Package organization
 * This package is divided by two sub packages:
 * - The standard approach (files located in the "a_d" folder). This approach uses the
 * timer1 (or timer2) to coordinate the ADC register acquisition.
 * - The more advanced approach (files located in the "a_d/advance_ad_scan" folder) uses
 * the ADC interrupt to update the four \b arrays containing all the data
 * of all the peripherals which use the ADC (\ref e_mic_scan for the microphones, \ref e_acc_scan
 * for the accelerometer, \ref e_ambient_ir and \ref e_ambient_and_reflected_ir for the proximity 
 * sensors). In this approach, the acquisition is made automatically and always with the
 * same delay. The functions specific to each module (advance_ad_scan/e_acc.c,
 * advance_ad_scan/e_prox.c, advance_ad_scan/e_micro.c) are also more elaborates than
 * the same one in the standard package.
 * 
 * \author Doc: Jonathan Besuchet
 */

#ifndef _AD_CONV
#define _AD_CONV

/* functions */
void e_init_ad(void); // to be used at the beginning to initialize AD
int e_read_ad(unsigned int channel); // simple function to sample one channel

#endif

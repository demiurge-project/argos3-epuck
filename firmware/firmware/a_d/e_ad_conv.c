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


//#include "p30f6014a.h"
#include "../motor_led/e_epuck_ports.h"

/*! \brief Initialize all the A/D register needed */
void e_init_ad(void)
{
  ADCON1 = ADCON2 = ADCON3 = 0;
  // ADPCFGbits.PCFGx 
  // = 0 for Analog input mode, 
  // = 1 for digital input mode (default)
  ADPCFGbits.PCFG0 = 1; // Debugger 
  ADPCFGbits.PCFG1 = 1; // Debugger 
  ADPCFGbits.PCFG2 = 0; // mic1
  ADPCFGbits.PCFG3 = 0; // mic2
  ADPCFGbits.PCFG4 = 0; // mic3
  ADPCFGbits.PCFG5 = 0;	// axe x acc.
  ADPCFGbits.PCFG6 = 0; // axe y acc.
  ADPCFGbits.PCFG7 = 0; // axe z acc.
  ADPCFGbits.PCFG8 = 0; // ir0
  ADPCFGbits.PCFG9 = 0; // ir1
  ADPCFGbits.PCFG10 = 0;  // ir2
  ADPCFGbits.PCFG11 = 0;  // ir3
  ADPCFGbits.PCFG12 = 0;  // ir4
  ADPCFGbits.PCFG13 = 0;  // ir5
  ADPCFGbits.PCFG14 = 0;  // ir6
  ADPCFGbits.PCFG15 = 0;  // ir7
  ADCON3 = (2*667/TCY_PIC)-1; //ADCS sampling time Tad minimum 667ns    
  ADCHS =0x0007;
  ADCON1bits.ADON = 1;
}

/*! \brief Function to sample an AD channel
 * \param channel The A/D channel you want to sample
 *                Must be between 0 to 15
 * \return The sampled value on the specified channel
 */
int e_read_ad(unsigned int channel)
{
  int delay;
  if(channel > 0x000F) return(0);
  ADCHS = channel;
  ADCON1bits.SAMP = 1;
  for(delay = 0; delay < 40; delay++);
  IFS0bits.ADIF = 0;
  ADCON1bits.SAMP = 0;
  while(!IFS0bits.ADIF);
  return(ADCBUF0);
}

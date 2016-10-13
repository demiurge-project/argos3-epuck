/********************************************************************************

			Control proximity sensor of e-puck	with timer 2
			December 2004: first version Lucas Meier & Francesco Mondada				
			Version 1.0 november 2005 Michael Bonani
			Version 1.1 January 2006 Xavier Raemy

This file is part of the e-puck library license.
See http://www.e-puck.org/index.php?option=com_content&task=view&id=18&Itemid=45

(c) 2004-2006 Lucas Meier & Francesco Mondada
(c) 2006-2007 Michael Bonani & Xavier Raemy

Robotics system laboratory http://lsro.epfl.ch
Laboratory of intelligent systems http://lis.epfl.ch
Swarm intelligent systems group http://swis.epfl.ch
EPFL Ecole polytechnique federale de Lausanne http://www.epfl.ch

**********************************************************************************/

/*! \file
 * \ingroup a_d
 * \brief Control proximity sensor of e-puck with timer2.
 *
 * The functions of this file are made to deal with the proximitiy
 * data. You can know the value of the ambient light detected by the
 * sensor. You can estimate the distance between the e-puck and an
 * obstacle by using \ref e_get_prox(unsigned int sensor_number) function.
 * \warning The module uses the timer2
 * \sa e_prox.c, e_prox.h to use the timer1
 * \author Code: Lucas Meier & Francesco Mondada, Michael Bonani, Xavier Raemy \n Doc: Jonathan Besuchet
 */

#include "e_ad_conv.h"
#include "./../motor_led/e_epuck_ports.h"
#include "e_prox.h"

/* internal variables for prox */
static int ambient_ir[8];				 // ambient light measurement
static int ambient_and_reflected_ir[8]; // light when led is on
static int reflected_ir[8];			 // variation of light


/* internal calls for prox */
void init_tmr2(void)
{
  T2CON = 0;                    // 
  T2CONbits.TCKPS = 1;          // prescsaler = 8
  TMR2 = 0;                     // clear timer 2
  PR2 = (350.0*MICROSEC)/8.0;   // first interrupt after 350us with 8 prescaler
  IFS0bits.T2IF = 0;            // clear interrupt flag
  IEC0bits.T2IE = 1;            // set interrupt enable bit
  T2CONbits.TON = 1;            // start Timer2
}

void __attribute__((interrupt, auto_psv, shadow))
_T2Interrupt(void)
{

// read ambient light and switch on leds in a first phase
// wait 350 us to let the phototransistor react
// read reflected light and switch off the leds in a second phase
// wait 3 ms before stating again
// repeat these two steps for the four couples of prox sensors

  static int ir_phase=0;	// phase can be 0 (ambient) or 1 (reflected)	
  static int ir_number=0;	// number goes from 0 to 3 (4 couples of sensors)	

  IFS0bits.T2IF = 0;            // clear interrupt flag

  switch (ir_number)
  {
    case 0:		// ir sensors 0 and 4
    {
      if (ir_phase == 0)
      {
        PR2 = (350.0*MICROSEC)/8.0;		// next interrupt in 350 us
		ambient_ir[0] = e_read_ad(IR0);
        ambient_ir[4] = e_read_ad(IR4);
        PULSE_IR0 = 1;			// led on for next measurement
        ir_phase = 1;			// next phase
      }
      else
      {
        PR2 = (2100.0*MICROSEC)/8.0;		// next interrupt in 3 ms
        ambient_and_reflected_ir[0] = e_read_ad(IR0);
        ambient_and_reflected_ir[4] = e_read_ad(IR4);
		reflected_ir[0] = ambient_ir[0] - ambient_and_reflected_ir[0];
		reflected_ir[4] = ambient_ir[4] - ambient_and_reflected_ir[4];
        PULSE_IR0 = 0;			// led off
        ir_phase = 0;			// reset phase
        ir_number = 1;			// next two sensors
      }
      break;
    }
    case 1:		// ir sensors 1 and 5
    {
      if (ir_phase == 0)
      {
        PR2 = (350.0*MICROSEC)/8.0;		// next interrupt in 350 us
        ambient_ir[1] = e_read_ad(IR1);
        ambient_ir[5] = e_read_ad(IR5);
        PULSE_IR1 = 1;			// led on for next measurement
        ir_phase = 1;			// next phase
      }
      else
      {
        PR2 = (2100.0*MICROSEC)/8.0;		// next interrupt in 3 ms
        ambient_and_reflected_ir[1] = e_read_ad(IR1);
        ambient_and_reflected_ir[5] = e_read_ad(IR5);
		reflected_ir[1] = ambient_ir[1] - ambient_and_reflected_ir[1];
		reflected_ir[5] = ambient_ir[5] - ambient_and_reflected_ir[5];
        PULSE_IR1 = 0;			// led off
        ir_phase = 0;			// reset phase
        ir_number = 2;			// next two sensors
      }
      break;
    }
    case 2:		// ir sensors 2 and 6
    {
      if (ir_phase == 0)
      {
        PR2 = (350.0*MICROSEC)/8.0;		// next interrupt in 350 us
        ambient_ir[2] = e_read_ad(IR2);
        ambient_ir[6] = e_read_ad(IR6);
        PULSE_IR2 = 1;			// led on for next measurement
        ir_phase = 1;			// next phase
      }
      else
      {
        PR2 = (2100.0*MICROSEC)/8.0;		// next interrupt in 3 ms
        ambient_and_reflected_ir[2] = e_read_ad(IR2);
        ambient_and_reflected_ir[6] = e_read_ad(IR6);
		reflected_ir[2] = ambient_ir[2] - ambient_and_reflected_ir[2];
		reflected_ir[6] = ambient_ir[6] - ambient_and_reflected_ir[6];
        PULSE_IR2 = 0;			// led off
        ir_phase = 0;			// reset phase
        ir_number = 3;			// next sensor
      }
      break;
    }
    case 3:		// ir sensors 3 and 7
    {
      if (ir_phase == 0)
      {
        PR2 = (350.0*MICROSEC)/8.0;		// next interrupt in 350 us
        ambient_ir[3] = e_read_ad(IR3);
        ambient_ir[7] = e_read_ad(IR7);
        PULSE_IR3 = 1;			// led on for next measurement
        ir_phase = 1;			// next phase
      }
      else
      {
        PR2 = (2100.0*MICROSEC)/8.0;		// next interrupt in 3 ms
        ambient_and_reflected_ir[3] = e_read_ad(IR3);
        ambient_and_reflected_ir[7] = e_read_ad(IR7);
		reflected_ir[3] = ambient_ir[3] - ambient_and_reflected_ir[3];
		reflected_ir[7] = ambient_ir[7] - ambient_and_reflected_ir[7];
        PULSE_IR3 = 0;			// led off
        ir_phase = 0;			// reset phase
        ir_number = 0;			// next sensor (back to beginning)
      }
      break;
    }
  }
  
}

/* ----- user calls ----- */

/*! \brief Init the proxymity sensor A/D converter and the timer2
 * \warning Must be called before starting using proximity sensor
 */
void e_init_prox(void)
{
	e_init_ad();				// init AD converter module
	init_tmr2();				// init timer 1 for ir processing
}

/*! \brief Stop the acquisition (stop timer2) */
void e_stop_prox(void)
{
	 T2CONbits.TON = 0;
	 PULSE_IR0=PULSE_IR1=PULSE_IR2=PULSE_IR3=0;
}

/*! \brief To get the analogic proxy sensor value of a specific sensor
 *
 * To estimate the proxymity of an obstacle, we do the following things:
 * - measure the ambient light
 * - turn on the IR led of the sensor
 * - measure the reflected light + ambient light
 * - calculate: reflected light = (reflected light + ambient light) - ambient light
 * - turn off the IR led of the sensor
 *
 * The result value of this function is: reflected light. More this value is great,
 * more the obsacle is near.
 * \param sensor_number The proxy sensor's number that you want the value.
 *                      Must be between 0 to 7.
 * \return The analogic value of the specified proxy sensor
 */
int e_get_prox(unsigned int sensor_number)
{
	if (sensor_number > 7)
		return 0;
	else
		return reflected_ir[sensor_number];
}

/*! \brief To get the analogic ambient light value of a specific sensor
 *
 * This function retur the analogic value of the ambient light measurement.
 * \param sensor_number The proxy sensor's number that you want the value.
 *                      Must be between 0 to 7.
 * \return The analogic value of the specified proxy sensor
 */
int e_get_ambient_light(unsigned int sensor_number)
{
	if (sensor_number > 7)
		return 0;
	else
		return ambient_ir[sensor_number];
}

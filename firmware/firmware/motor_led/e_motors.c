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

#include <stdlib.h>
#include "e_epuck_ports.h"
#include "e_init_port.h"
#include "e_motors.h"

/* internal variables */

static int left_speed = 0;
static int right_speed = 0;
static int nbr_pas_left = 0;
static int nbr_pas_right = 0;

/* internal calls */
 
void __attribute__((interrupt, auto_psv, shadow))
 _T5Interrupt(void) // interrupt for motor 1 (of two) = left motor
{
  static int motor_phase=0;		 // phase can be 0 to 3

  IFS1bits.T5IF = 0;             // clear interrupt flag

  // increment or decrement phase depending on direction

  if (left_speed > 0) // inverted for the two motors
    {
    nbr_pas_left++;
	motor_phase--;
    if (motor_phase < 0) motor_phase = 3;
    }
  else 
    {
    nbr_pas_left--;
    motor_phase++;
    if (motor_phase > 3) motor_phase = 0;
    }
  
  // set the phase on the port pins

  switch (motor_phase)
  {
    case 0:
    {
      MOTOR1_PHA = 0;
      MOTOR1_PHB = 1;
      MOTOR1_PHC = 0;
      MOTOR1_PHD = 1;
      break;
    }
    case 1:
    {
      MOTOR1_PHA = 0;
      MOTOR1_PHB = 1;
      MOTOR1_PHC = 1;
      MOTOR1_PHD = 0;
      break;
    }
    case 2:
    {
      MOTOR1_PHA = 1;
      MOTOR1_PHB = 0;
      MOTOR1_PHC = 1;
      MOTOR1_PHD = 0;
      break;
    }
    case 3:
    {
      MOTOR1_PHA = 1;
      MOTOR1_PHB = 0;
      MOTOR1_PHC = 0;
      MOTOR1_PHD = 1;
      break;
    }
  }
}

void __attribute__((interrupt, auto_psv, shadow))
 _T4Interrupt(void)  // interrupt for motor 2 (of two) = right motor
{
  static int motor_phase=0;	// phase can be 0 to 3

  IFS1bits.T4IF = 0;             // clear interrupt flag

  // increment or decrement phase depending on direction

  if (right_speed < 0)
    {
    nbr_pas_right--;
    motor_phase--;
    if (motor_phase < 0) motor_phase = 3;
    }
  else 
    {
    nbr_pas_right++;
    motor_phase++;
    if (motor_phase > 3) motor_phase = 0;
    }
  
  // set the phase on the port pins

  switch (motor_phase)
  {
    case 0:
    {
      MOTOR2_PHA = 0;
      MOTOR2_PHB = 1;
      MOTOR2_PHC = 0;
      MOTOR2_PHD = 1;
      break;
    }
    case 1:
    {
      MOTOR2_PHA = 0;
      MOTOR2_PHB = 1;
      MOTOR2_PHC = 1;
      MOTOR2_PHD = 0;
      break;
    }
    case 2:
    {
      MOTOR2_PHA = 1;
      MOTOR2_PHB = 0;
      MOTOR2_PHC = 1;
      MOTOR2_PHD = 0;
      break;
    }
    case 3:
    {
      MOTOR2_PHA = 1;
      MOTOR2_PHB = 0;
      MOTOR2_PHC = 0;
      MOTOR2_PHD = 1;
      break;
    }
  }
}


/* ---- user calls ---- */

/*! \brief Initialize the motors's ports
 *
 * This function initialize the ports used by the motors.
 * In fact it call "the e_init_port()" function.
 * \sa e_init_port
 */
void e_init_motors(void)
{
  e_init_port();				// init general ports
}

/*! \brief Manage the left speed
 *
 * This function manage the left motor speed by changing the MOTOR1
 * phases. The changing phases frequency (=> speed) is controled by
 * the timer5.
 * \param motor_speed from -1000 to 1000 give the motor speed in steps/s,
 * positive value to go forward and negative to go backward.
 */
void e_set_speed_left(int motor_speed)  // motor speed in steps/s
{
  if (motor_speed == 0)
  {
    T5CONbits.TON = 0;            // stop Timer5
    MOTOR1_PHA = 0;
    MOTOR1_PHB = 0;
    MOTOR1_PHC = 0;
    MOTOR1_PHD = 0;
  }
  else
  {
    T5CONbits.TON = 0;            // stop Timer5

    left_speed = motor_speed;

    T5CON = 0;                    // 
    T5CONbits.TCKPS=3;            // prescsaler = 256
    TMR5 = 0;                     // clear timer 5
    PR5 = (FCY/256)/abs(motor_speed);  // interrupt every Xms with 256 prescaler
    IFS1bits.T5IF = 0;            // clear interrupt flag
    IEC1bits.T5IE = 1;            // set interrupt enable bit
    T5CONbits.TON = 1;            // start Timer5
  }
}

/*! \brief Manage the right speed
 *
 * This function manage the right motor speed by changing the MOTOR2
 * phases. The changing phases frequency (=> speed) is controled by
 * the timer4.
 * \param motor_speed from -1000 to 1000 give the motor speed in steps/s,
 * positive value to go forward and negative to go backward.
 */
void e_set_speed_right(int motor_speed)
{
  if (motor_speed == 0)
  {
    T4CONbits.TON = 0;            // stop Timer4
    MOTOR2_PHA = 0;
    MOTOR2_PHB = 0;
    MOTOR2_PHC = 0;
    MOTOR2_PHD = 0;
  }
  else
  {
    T4CONbits.TON = 0;            // stop Timer4

    right_speed = motor_speed;

    T4CON = 0;                    // 
    T4CONbits.TCKPS=3;            // prescsaler = 256
    TMR4 = 0;                     // clear timer 4
    PR4 = (FCY/256)/abs(motor_speed);  // interrupt every Xms with 256 prescaler
    IFS1bits.T4IF = 0;            // clear interrupt flag
    IEC1bits.T4IE = 1;            // set interrupt enable bit
    T4CONbits.TON = 1;            // start Timer4
  }
}

/*! \brief Give the number of left motor steps
 * \return The number of phases steps made since the left motor
 * is running.
 */
int e_get_steps_left(void)
{
  return nbr_pas_left;
}

/*! \brief Set the number of left motor steps
 * \param set_steps The number of changed phases that you want set.
 */
void e_set_steps_left(int set_steps)
{
  INTERRUPT_OFF();
  nbr_pas_left = set_steps;
  INTERRUPT_ON();
}

/*! \brief Give the number of right motor steps
 * \return The number of phases steps made since the right motor
 * is running.
 */
int e_get_steps_right(void)
{
  return nbr_pas_right;
}

/*! \brief Set the number of right motor steps
 * \param set_steps The number of changed phases that you want set.
 */
void e_set_steps_right(int set_steps)
{
  INTERRUPT_OFF();
  nbr_pas_right = set_steps;
  INTERRUPT_ON();
}

/********************************************************************************

			Advance fast agenda events of e-puck							
			2005: first version
			Julien Hubert 


This file is part of the e-puck library license.
See http://www.e-puck.org/index.php?option=com_content&task=view&id=18&Itemid=45

(c) 2004-2007 Julien Hubert

Robotics system laboratory http://lsro.epfl.ch
Laboratory of intelligent systems http://lis.epfl.ch
Swarm intelligent systems group http://swis.epfl.ch
EPFL Ecole polytechnique federale de Lausanne http://www.epfl.ch

**********************************************************************************/

/*! \file
 * \ingroup motor_LED
 * \brief Manage the fast agendas (timer1, 2, 3).
 *
 * This module manage the fast agendas with the timer1, 2, 3.
 * \n \n An agenda is a structure made to work as chained list. It containts:
 * the function you want to launch, the time setup between two launching events,
 * a counter to measure the current time, a pointer to the next element of the list.
 * \n \n Each times the timer1, 2, 3 has an interrupt, the corresponding agenda
 * chained list is scanned to look if an agenda has to be
 * treated according to the cycle value and current counter value.
 * \n \n If one (or more) agenda has to be treated, his callback function is launch.
 * \author Code: Julien Hubert \n Doc: Jonathan Besuchet
 */

#ifndef __AGENDA_FAST_H__
#define __AGENDA_FAST_H__

#define AG_ALREADY_CREATED	1
#define AG_NOT_FOUND		2

/**********************************************************************
 * ------------------------ Type definition ---------------------------
 **********************************************************************/

typedef struct AgendaType Agenda;

struct AgendaType
{
  unsigned int cycle;		// length in 10e of ms of a cycle between two alarms
  int  counter;				// counter 
  char activate;			// can be on=1 or off=0
  void (*function) (void);	// function called when counter > cycle, 
							// WARNING: This function must have the following prototype
							//			void func(void)
  Agenda *next;				// pointer on the next agenda
};

/*! \struct AgendaList
 * \brief Manage the differents agendas lists
 *
 * We use the 3 first timers, then we need 3 pointers
 * of the beginning of each Agenda chained list. We also
 * have a pointer of the waiting Agenda chained list.
 */
struct AgendaList
{
	char motors;
	Agenda* waiting;                  /*!< If an agenda's speed goes down to zero, we remove it from the list and add it to the waiting list */
	Agenda* agendas[3];               /*!< We use the 3 first timers */
	unsigned char timers_in_use[3];   /*!< Determine which one we use currently */
	unsigned speed[3];                /*!< Base speed 0.1 ms but use of multiplication*/
};

/***********************************************************************
 * ------------------------ From agenda.c file --------------------------
 **********************************************************************/
void e_start_agendas_processing(void);
void e_start_timer_processing(int timer);

//void e_end_agendas_processing();
void e_end_agendas_processing(int timer);

void e_configure_timer(int timer);

void e_activate_agenda(void (*func)(void), unsigned cycle);
void e_activate_motors(void (*func1)(void),void (*func2)(void));
int e_set_motor_speed(void (*func)(void), unsigned cycle);
int e_destroy_agenda(void (*func)(void));

int e_set_agenda_cycle(void (*func)(void), unsigned cycle);
int e_reset_agenda(void (*func)(void));

int e_pause_agenda(void (*func)(void));
int e_restart_agenda(void (*func)(void));

//void SendAgendaStatus();


#endif /* __AGENDA_FAST_H__ */


/* End of File : e_agenda_fast.h */

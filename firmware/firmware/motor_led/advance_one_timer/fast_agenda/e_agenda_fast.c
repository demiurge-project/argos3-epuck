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

#include "e_agenda_fast.h"

#include "../../e_epuck_ports.h"
#include <stdlib.h>

#define EXIT_OK 1

//static char buffer[100];

static struct AgendaList agenda_list;

// Use Timer 1, 2 and 3
// With prescaler == 0 we can do only 4.4 msec timers at maximum.
// So we need to use the prescaler
// prescaler == 1 -> 1:8 prescaler -> up to 35.8 msec
// prescaler == 2 -> 1:64 prescaler -> up to 286.4 msec
// prescaler == 3 -> 1:256 prescaler -> up to 1145.9 msec

/*------ internal calls ------*/

// Copyright Wikipedia A faster one in assembly language and binary operators can be found
unsigned compute_gcd(unsigned u,unsigned v)
{
     unsigned int k = 0;
     if (u == 0)
         return v;
     if (v == 0)
         return u;
     while ((u & 1) == 0  &&  (v & 1) == 0) { /* while both u and v are even */
         u >>= 1;   /* shift u right, dividing it by 2 */
         v >>= 1;   /* shift v right, dividing it by 2 */
         k++;       /* add a power of 2 to the final result */
     }
     /* At this point either u or v (or both) is odd */
     do {
         if ((u & 1) == 0)      /* if u is even */
             u >>= 1;           /* divide u by 2 */
         else if ((v & 1) == 0) /* else if v is even */
             v >>= 1;           /* divide v by 2 */
         else if (u >= v)       /* u and v are both odd */
             u = (u-v) >> 1;
         else                   /* u and v both odd, v > u */
             v = (v-u) >> 1;
     } while (u > 0);
     return v << k;  /* returns v * 2^k */

}

unsigned my_ceil(float a)
{
	if (a>(unsigned)a)
		return (unsigned)a+1;
	return (unsigned)a;
}

void e_set_timer_speed(int timer, unsigned speed)
{
	// Compute adequate prescaler
	unsigned prescaler=0;
	unsigned factor=0;

	if (speed>11459)
		speed=speed/my_ceil(speed/11459.0);
	
	agenda_list.speed[timer-1]=speed;

	if (speed>2864) {
		prescaler=3;
		factor=8;   //2^8
	}
	else {
 		if (speed>358) {
			prescaler=2;
			factor=6;   //2^6
		}
		else
			if (speed>=44) {  // 44 = (2^16-1)/1464
				prescaler=1;
				factor=3;   //2^3
			}
	}
	
	switch(timer) {
	case 0:
		e_set_timer_speed(1,speed);
		e_set_timer_speed(2,speed);
		e_set_timer_speed(3,speed);
		break;
	case 1:
		PR1 = (unsigned)((long unsigned)(0.1*MILLISEC*speed) >> factor);
		T1CONbits.TCKPS=prescaler;
		break;
	case 2:
		PR2 = (unsigned)((long unsigned)(0.1*MILLISEC*speed) >> factor);
		T2CONbits.TCKPS=prescaler;
		break;
	case 3:
		PR3 = (unsigned)((long unsigned)(0.1*MILLISEC*speed) >> factor);
		T3CONbits.TCKPS=prescaler;
		break;
	default:
		break;
	}
}

// Look for the best timer to receive a task with cycle
int search_best_fit(unsigned cycle)
{
	int i=0;
	char ctimer=-1;
	unsigned gcd=1;
	unsigned tgcd;
	for (i=0;i<3;++i)
		if (agenda_list.timers_in_use[i] > 0 && ((tgcd=compute_gcd(agenda_list.speed[i],cycle))>gcd) ) {
			gcd=tgcd;
			ctimer=i;
		}
	if (ctimer<0) {
		ctimer=0;
		gcd=cycle;
	}
	return ctimer;
}

// Recompute the speeds of all the timers to optimize their cycles
void recompute_speeds()
{
	e_end_agendas_processing(0);   // Stops the agenda
	unsigned gcd;
	int i=1;
	Agenda* current;
	
	for (;i<3;++i) {
		current=agenda_list.agendas[i];
		if (current) {
			gcd=current->cycle;
			current=current->next;
		}
		while (current) {
			gcd=compute_gcd(gcd,current->cycle);
			current=current->next;
		}
		if (gcd!=agenda_list.speed[i]) {
			e_set_timer_speed(i+1,gcd);
		}
	}
	e_start_timer_processing(0);  // Resume the timers
}

// Find a particular function
Agenda* find_function(void (*func)(void))
{
	Agenda *current=agenda_list.waiting;
	int i;
	
	while(current && current->function != func)
		current = current->next;
	if(current)
		return current;
	else {
	
		for (i=0;i<3;++i) {
			current = agenda_list.agendas[i];
			while(current && current->function != func)
				current = current->next;
			if (current)
				return current;
		}
	}
	return (Agenda*)0;
}

// Assign a task to the best timer
unsigned assign_agenda(Agenda* agenda)
{
	unsigned gcd=1,tgcd;
	int i;
	char ctimer=-1;

	// Look for one free timer...

	for (i=0;i<3 && agenda_list.timers_in_use[i] > 0;++i);

	if (i<3 && agenda_list.motors!=i) {   // We have a free timer...
		e_configure_timer(i+1);   // Initialise the timer
		ctimer=i;
		gcd=agenda->cycle;
	}
	else {	
		for (i=0;i<3;++i)
			if (agenda_list.timers_in_use[i] > 0 && agenda_list.motors!=i && ((tgcd=compute_gcd(agenda_list.speed[i],agenda->cycle))>gcd) ) {
				gcd=tgcd;
				ctimer=i;
			}
	}

	if (ctimer<0) {
		ctimer=0;

		if (ctimer==agenda_list.motors)
			ctimer=1;
		else if (agenda_list.motors!=1 && agenda_list.speed[1]<agenda_list.speed[(unsigned)ctimer])
			ctimer=1;			

		if (agenda_list.motors!=2 && agenda_list.speed[2]<agenda_list.speed[(unsigned)ctimer])
			ctimer=2;	
	}

	e_end_agendas_processing(ctimer+1);
	e_set_timer_speed(ctimer+1,gcd);

	agenda->activate=1;
	agenda->next=agenda_list.agendas[(unsigned)ctimer];
	agenda_list.agendas[(unsigned)ctimer]=agenda;
	agenda_list.timers_in_use[i]++;
	e_start_timer_processing(ctimer+1);

	return ctimer;
}

void migrate(int timer)
{
	if (agenda_list.timers_in_use[timer]>0) {
		Agenda* current=agenda_list.agendas[timer];
		Agenda* next=0;

		while (current) {
			next=current->next;
			current->next=0;
			assign_agenda(current);
			current=next;
		}
		agenda_list.agendas[timer]=0;
		agenda_list.timers_in_use[timer]=0;
	}
}

/*------ external calls ------*/

/*! \brief Initialise the accounting structure
 *
 * Start the agendas processing by initialising the accounting structures.
 * \n Don't activate any timer which is done by e_start_timer_processing.
 * \sa e_start_timer_processing
 */
void e_start_agendas_processing(void)
{
	agenda_list.agendas[0] = 0;
	agenda_list.agendas[1] = 0;
	agenda_list.agendas[2] = 0;
	
	agenda_list.speed[0] = 1;
	agenda_list.speed[1] = 1;
	agenda_list.speed[2] = 1;
	
	agenda_list.waiting = 0;

	agenda_list.timers_in_use[0] = 0;
	agenda_list.timers_in_use[1] = 0;
	agenda_list.timers_in_use[2] = 0;

	agenda_list.motors=-1;
}


/*! \brief Configure the timer(s) used
 *
 * Configure one or all the timers to be used by the agenda
 * \param timer the timer's number to configure (between [0-3] with 0 configuring all of them)
 */
void e_configure_timer(int timer)
{
	switch(timer) {
	case 0:
		e_configure_timer(1);
		e_configure_timer(2);
		e_configure_timer(3);
		break;
	case 1:
		T1CON = 0;							// reset Timer1 CONtrol register
		T1CONbits.TCKPS = 0;				// prescaler = 1
		TMR1 = 0;							// clear timer 1
		PR1 = (int)(0.1*MILLISEC);		    // interrupt every 0.1 ms without prescaler
		IFS0bits.T1IF = 0;					// clear interrupt flag
		IEC0bits.T1IE = 1;					// set interrupt enable bit
		break;
	case 2:
		T2CON = 0;							// reset Timer2 CONtrol register
		T2CONbits.TCKPS = 0;		        // prescaler = 1
		TMR2 = 0;							// clear timer 2
		PR2 = (int)(0.1*MILLISEC);			// interrupt every 0.1 ms without prescaler
		IFS0bits.T2IF = 0;					// clear interrupt flag
		IEC0bits.T2IE = 1;					// set interrupt enable bit
		break;
	case 3:
		T3CON = 0;							// reset Timer3 CONtrol register
		T3CONbits.TCKPS = 0;				// prescaler = 1
		TMR3 = 0;							// clear timer 3
		PR3 = (int)(0.1*MILLISEC);		    // interrupt every 0.1 ms without prescaler
		IFS0bits.T3IF = 0;					// clear interrupt flag
		IEC0bits.T3IE = 1;					// set interrupt enable bit
		break;
	default:
		break;
	}
}

/*! \brief Start the timer(s) used
 *
 * Activate one or all the timers to be used by the agenda.
 * \param timer the timer's number to activate (between [0-3] with 0 activating all of them)
 */
void e_start_timer_processing(int timer)
{
	switch(timer) {
	case 0:
		e_start_timer_processing(1);
		e_start_timer_processing(2);
		e_start_timer_processing(3);
		break;
	case 1:
		T1CONbits.TON = 1;					// start Timer1
//		agenda_list.timers_in_use[0] = 1;
		break;
	case 2:
		T2CONbits.TON = 1;					// start Timer2
//		agenda_list.timers_in_use[1] = 1;
		break;
	case 3:
		T3CONbits.TON = 1;					// start Timer3
//		agenda_list.timers_in_use[2] = 1;
		break;
	default:
		break;
	}
}

/*! \brief Stop an agenda running
 *
 * Stop the agendas running on one particular timer
 * (the memory allocated for the agenda isn't freed, use e_destroy_agenda for that).
 * \param timer the timer's number [0-3]. 0 stops all the timers.
 * \sa e_destroy_agenda
 */
void e_end_agendas_processing(int timer)
{
	switch(timer) {
	case 0:
		e_end_agendas_processing(1);
		e_end_agendas_processing(2);
		e_end_agendas_processing(3);
		break;
	case 1:
		T1CONbits.TON = 0;
		break;
	case 2:
		T2CONbits.TON = 0;
		break;
	case 3:
		T3CONbits.TON = 0;
		break;
	default:
		break;
	}
}

/*! \brief Activate a fast agenda
 *
 * Activate an agenda and allocate memory for him if there isn't already
 * an agenda with the same callback function (the agenda is active but isn't
 * processed if he have a null cycle value).
 * \n The appropriate timer is automatically selected.
 * \param func		function called if the cycle value is reached by the counter
 * \param cycle     cycle value in millisec/10
 */
void e_activate_agenda(void (*func)(void), unsigned cycle)
{
	e_end_agendas_processing(0);

	// Choose the best timer to put the task on
	int i=0;
	Agenda *current;
	char ctimer=-1;
	unsigned gcd=1;
	unsigned tgcd;
	
	if (cycle>0) {   // We don't insert the task if its cycle is zero (cfr motors)

		// Look for a free timer
		for (;i<3 && agenda_list.timers_in_use[i] > 0;++i);
		if (i<3 && agenda_list.motors!=i) {   // We have at least one free timer so we'll use this one

			e_configure_timer(i+1);   // Initialise the timer
			e_set_timer_speed(i+1,cycle);
			if (!(current = malloc(sizeof(Agenda))))
				exit(EXIT_FAILURE);
			agenda_list.agendas[i]=current;
			current->cycle=cycle;
			current->counter=0;
			current->function=func;
			current->activate=1;
			current->next=0;
			agenda_list.timers_in_use[i]++;
			e_start_timer_processing(i+1);
		}
		else { // We have no free timer so we look for one that is the closest to the cycle we need
			// First we compute the gcd with existing timers.

			for (i=0;i<3;++i)
				if (agenda_list.timers_in_use[i] > 0 && agenda_list.motors!=i && ((tgcd=compute_gcd(agenda_list.speed[i],cycle))>gcd) ) {
						gcd=tgcd;
						ctimer=i;
				}

			
			if (ctimer<0) {
				ctimer=0;
		
				if (agenda_list.speed[1]<agenda_list.speed[(unsigned)ctimer])
					ctimer=1;			
		
				if (agenda_list.speed[2]<agenda_list.speed[(unsigned)ctimer])
					ctimer=2;	
			}

			// Now that we know where to add the new task, the new speed of the timer is in gcd
			// 1. Stop the timer
			// 2. Change its speed
			// 3. Add the new task
			// 4. Restart the timer
	
//			e_end_agendas_processing(ctimer+1);
			e_set_timer_speed(ctimer+1,gcd);
	
			if (!(current = malloc(sizeof(Agenda))))
				exit(EXIT_FAILURE);
			current->cycle=cycle;
			current->counter=0;
			current->function=func;
			current->activate=1;
			current->next=agenda_list.agendas[(unsigned)ctimer];
			agenda_list.agendas[(unsigned)ctimer]=current;
			agenda_list.timers_in_use[(unsigned)ctimer]++;
//			e_start_timer_processing((unsigned)ctimer+1);
		}
	}
	else
	{
		// We create the structure of the new task and put it in the waiting list
			if (!(current = malloc(sizeof(Agenda))))
				exit(EXIT_FAILURE);
			current->cycle=cycle;
			current->counter=0;
			current->function=func;
			current->activate=0;
			current->next=agenda_list.waiting;
			agenda_list.waiting=current;
	}
	e_start_timer_processing(0);
}

void e_activate_motors(void (*func1)(void),void (*func2)(void))
{
	// Find the less used timer to migrate. There is no reason to look at speed as the task will be spread on other timers

	e_end_agendas_processing(0);

	char i=0,timer=0;
	unsigned nbtimer=65535;
	Agenda* current;
	for (;i<3 && agenda_list.timers_in_use[(int)i]>0;++i)
		if (agenda_list.timers_in_use[(int)i]<nbtimer) {
			nbtimer=agenda_list.timers_in_use[(int)i];
			timer=i;
		}
	
	if (i==3) {    // We didn't find a free timer
		agenda_list.motors=timer;
		migrate(timer);
		agenda_list.speed[(int)timer]=1;
	}
	else
		agenda_list.motors=i;

	e_configure_timer(agenda_list.motors+1);   // Initialise the timer
	if (!(current = malloc(sizeof(Agenda))))
		exit(EXIT_FAILURE);
	agenda_list.agendas[(int)timer]=current;
	current->cycle=0;
	current->counter=0;
	current->function=func1;
	current->activate=1;
	if (!(current->next=malloc(sizeof(Agenda))))
		exit(EXIT_FAILURE);
	current=current->next;	
	current->next=0;
	current->cycle=0;
	current->counter=0;
	current->function=func2;
	current->activate=1;

	agenda_list.timers_in_use[(int)agenda_list.motors]+=2;

	e_start_timer_processing(0);
}

int e_set_motor_speed(void (*func)(void), unsigned cycle)
{
	if (agenda_list.motors>=0) {
		Agenda *current=agenda_list.agendas[(int)agenda_list.motors];
		while (current && current->function != func)
			current=current->next;
		if (!current)
			return AG_NOT_FOUND;

		e_end_agendas_processing(agenda_list.motors+1);
		if (current->next)
			e_set_timer_speed(agenda_list.motors+1,compute_gcd(current->next->cycle,cycle));
		else
			e_set_timer_speed(agenda_list.motors+1,compute_gcd(agenda_list.speed[(int)agenda_list.motors],cycle));

		current->cycle = cycle;

		e_start_timer_processing(agenda_list.motors+1);
		

		return EXIT_OK;
	}
	else
		return AG_NOT_FOUND;
}


/*! \brief Destroy an agenda
 *
 * Destroy the agenda with a given callback function
 * \param func		function to test
 * \return int      return the success of the destruction (EXIT_OK for successfull,
 * AG_NOT_FOUND for unsuccessfull).
 */
int e_destroy_agenda(void (*func)(void))
{
	Agenda *preceding;
	Agenda *current;
	int i=0;

	for (;i<3;++i) {
		preceding = 0;
		current = agenda_list.agendas[i];
		while (current && current->function != func) {
			current = current->next;
			preceding = current;
		}
		if (current) {  // We found a match
			if (preceding)
				preceding->next = current->next;
			else
				agenda_list.agendas[i] = current->next;
			free(current);
			agenda_list.timers_in_use[i]--;
			return (EXIT_OK);
		}
	}
	return (AG_NOT_FOUND);
}

/* \brief Change the cycle of an agenda
 *
 * Change the cycle value of an agenda with a given callback function.
 * \param func		 function to test
 * \param cycle      new cycle value in millisec/10 
 */
int e_set_agenda_cycle(void (*func)(void),unsigned cycle)
{
	Agenda *current=agenda_list.waiting;
	Agenda *previous=0;
	int i, timer;
	unsigned gcd;

	e_end_agendas_processing(0);


	// First check the waiting list to see if func is present if the new cycle is bigger than 0
	if (cycle>0) {
		while(current && current->function != func) {
			previous=current;
			current = current->next;
		}

	
		if (current) {   // It is in the waiting list. We take it out and assign to one timer
			if (previous) {
				previous->next=current->next;
			}
			else {
				agenda_list.waiting=current->next;
			}
			current->next=0;
			current->cycle=cycle;
			timer=assign_agenda(current);
	
			return EXIT_OK;
		}
	}

	for (i=0;i<3;++i) {
		previous=0;
		current = agenda_list.agendas[i];
		while(current && current->function != func) {
			previous=current;
			current = current->next;
		}
		if (current)
			break;
	}

	if (!current)
		return AG_NOT_FOUND;


	if (cycle==0) {   // Removal of the task from the list to the waiting list
		if (previous)
			previous->next=current->next;
		else {
 			agenda_list.agendas[i]=current->next;
//			if (agenda_list.agendas[i]==0)
//				e_end_agendas_processing(i+1);
		}
		agenda_list.timers_in_use[i]--;
		current->next=agenda_list.waiting;
		agenda_list.waiting=current;
		current->cycle=0;
		current->activate=0;
	}
	else {     // Compute the new speed of the timer according
	
		gcd=compute_gcd(cycle,agenda_list.speed[i]);
	
//		e_end_agendas_processing(i+1);
		e_set_timer_speed(i+1,gcd);
		
		current->cycle = cycle;
		
//		e_start_timer_processing(i+1);
	}
	e_start_timer_processing(0);

	return EXIT_OK;
}

 /*! \brief Reset an agenda
 *
 * Reset an agenda's counter with a given callback function
 * \param func		function to reset
 */
int e_reset_agenda(void (*func)(void))
{
	Agenda* current=find_function(func);

	if (current) {
		current->counter = 0;
		return EXIT_OK;
	}
	else
		return AG_NOT_FOUND;
	
}

/*! \brief Pause an agenda
 *
 * Pause an agenda but do not reset its information.
 * @param func		 function to pause
 */
int e_pause_agenda(void (*func)(void))
{
	Agenda* current=find_function(func);

	if (current) {
		current->activate = 0;
		return EXIT_OK;
	}
	else
		return AG_NOT_FOUND;
}

/*! \brief Restart an agenda previously paused
 *
 * Restart an agenda previously paused
 * @param func		 function to restart
 */
int e_restart_agenda(void (*func)(void))
{
	Agenda* current=find_function(func);

	if (current) {
		current->activate = 1;
		return EXIT_OK;
	}
	else
		return AG_NOT_FOUND;
}

/*! \brief Interrupt from timer1
 *
 * Parse the chained list of agenda.
 * \n Increment counter only.
 * \n Check if agenda has to be treated according to the cycle value 
 * and current counter value.
 * \n Do it for number of cycle positive or null.
 * \n Check if a service has to be activated. 
 */
void __attribute__((__interrupt__, auto_psv))
 _T1Interrupt(void)
{
	if (agenda_list.timers_in_use[0] == 0 || agenda_list.speed[0] == 0)
		return;

	Agenda *current = agenda_list.agendas[0];

	IFS0bits.T1IF = 0;


	while (current)
	{
		// agenda must be active with a positive non-null cycle value
		if(current->activate == 1 && current->cycle > 0) 
		{
			current->counter+=agenda_list.speed[0];
			// check if the agenda event must be triggered
			if(current->counter >= current->cycle) // a cycle value of 1 will be performed every interupt
			{
				current->function();	// trigger the associated function
				current->counter=0;		// reset the counter
			}
		}
		current = current->next;
	}
	current=0;
  return;
}

/*! \brief Interrupt from timer2
 *
 * Parse the chained list of agenda.
 * \n Increment counter only.
 * \n Check if agenda has to be treated according to the cycle value 
 * and current counter value.
 * \n Do it for number of cycle positive or null.
 * \n Check if a service has to be activated. 
 */
void __attribute__((__interrupt__, auto_psv))
 _T2Interrupt(void)
{

	if (agenda_list.timers_in_use[1] == 0  || agenda_list.speed[1] == 0)
		return;

	Agenda *current = agenda_list.agendas[1];

	IFS0bits.T2IF = 0;

	while (current)
	{
		// agenda must be active with a positive non-null cycle value
		if(current->activate == 1 && current->cycle > 0) 
		{
			current->counter+=agenda_list.speed[1];
			// check if the agenda event must be triggered
			if(current->counter > current->cycle-1) // a cycle value of 1 will be performed every interupt
			{
				current->function();	// trigger the associeted function
				current->counter=0;		// reset the counter
			}
		}
		current = current->next;
	}
  return;
}


/*! \brief Interrupt from timer3
 *
 * Parse the chained list of agenda.
 * \n Increment counter only.
 * \n Check if agenda has to be treated according to the cycle value 
 * and current counter value.
 * \n Do it for number of cycle positive or null.
 * \n Check if a service has to be activated. 
 */
void __attribute__((__interrupt__, auto_psv))
 _T3Interrupt(void)
{

	if (agenda_list.timers_in_use[2] == 0 || agenda_list.speed[2] == 0)
		return;

	Agenda *current = agenda_list.agendas[2];

	IFS0bits.T3IF = 0;

	while (current)
	{
		// agenda must be active with a positive non-null cycle value
		if(current->activate == 1 && current->cycle > 0) 
		{
			current->counter+=agenda_list.speed[2];
			// check if the agenda event must be triggered
			if(current->counter > current->cycle-1) // a cycle value of 1 will be performed every interupt
			{
				current->function();	// trigger the associeted function
				current->counter=0;		// reset the counter
			}
		}
		current = current->next;
	}
  return;
}

/*void SendAgendaStatus()
{
	e_end_agendas_processing(0);	
	Agenda* current;

	unsigned i=0;
	for (i=0;i<3;++i) {
		sprintf(buffer,"agenda=%u  speed=%u\r\n",i,agenda_list.speed[i]);
		uart_send_text(buffer);
		current=agenda_list.agendas[i];
		if (!current) {
			uart_send_static_text("  Agenda not in use \r\n");
		}
		else {
			while (current) {
				sprintf(buffer," %u\r\n",current->cycle);
				uart_send_text(buffer);
				current=current->next;
			}

		}
	}

	current=agenda_list.waiting;
	uart_send_static_text("Waiting list : \r\n");

	if (!current) {
		uart_send_static_text("  Waiting list empty \r\n");
	}
	else {
		while (current) {
			sprintf(buffer," %u\r\n",current->cycle);
			uart_send_text(buffer);
			current=current->next;
		}
	}
	

	e_start_timer_processing(0);	

}

*/

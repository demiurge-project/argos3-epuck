
/*
    Copyright 2007 Alexandre Campo, Alvaro Guttierez, Valentin Longchamp.

    This file is part of libIrcom.

    libIrcom is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License.

    libIrcom is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libIrcom.  If not, see <http://www.gnu.org/licenses/>.
*/


// Main file of the ircom module

#ifndef IRCOM_C
#define IRCOM_C

#include <p30f6014a.h>
#include <string.h>
#include <e_epuck_ports.h>
#include <a_d/advance_ad_scan/e_ad_conv.h>
#include "ircom.h"
#include "ircomMessages.h"
#include "ircomReceive.h"
#include "ircomSend.h"
#include "ircomTools.h"



// main structures are stored uniquely here.
volatile Ircom ircomData;
volatile IrcomReceive ircomReceiveData;
volatile IrcomSend ircomSendData;

// start the timer that operates ircom
// default values are set and ircom module is waiting for further instructions
void ircomStart ()
{
    ircomFlushMessages();    
    ircomDisableContinuousListening();
    ircomPause(0);
    ircomSetOmnidirectional();

    // set a number of default values
    ircomData.fsm = IRCOM_FSM_IDLE;
    ircomData.time = 0;
    ircomReceiveData.receiving = 0;
    ircomReceiveData.done = 1;
    ircomSendData.done = 1;

    // smart computation of timings
    double tad = (TCY_PIC * (ADCS_SETTING + 1.0) / 2.0);   // ns
    double samplingTime = (tad * 15.0 * 8.0 / 1000.0);    // us 
    double windowDuration = (samplingTime * SAMPLING_WINDOW);    
    ircomSendData.markSwitchCount = ((int)(windowDuration / (samplingTime * IRCOM_MARK_SAMPLE_PER_PERIOD)));
    ircomSendData.spaceSwitchCount = ((int)(windowDuration / (samplingTime * IRCOM_SPACE_SAMPLE_PER_PERIOD)));
    ircomSendData.markDuration = ((windowDuration / ((double)ircomSendData.markSwitchCount) * MICROSEC)/8.0);
    ircomSendData.spaceDuration = ((windowDuration / ((double)ircomSendData.spaceSwitchCount) * MICROSEC)/8.0);    

    // activate timer
    T1CON = 0;                    // 
    T1CONbits.TCKPS = 1;          // prescsaler = 8
    TMR1 = 0;                     // clear timer 1
    PR1 = (IRCOM_RECEIVESPEED * MICROSEC)/8.0; 
    IFS0bits.T1IF = 0;            // clear interrupt flag
    IEC0bits.T1IE = 1;            // set interrupt enable bit
    IPC0bits.T1IP = 6;            // timer1 priority should be above user code
    T1CONbits.TON = 1;            // start Timer1
}

// stop the timer that operates ircom
// no ircom functions are anymore available
inline void ircomStop (void)
{
    T1CONbits.TON = 0;    // disable Timer1
    ad_activate_proximity = 1;
    ad_disable_proximity = 0;
    e_ad_scan_on();
}

inline void ircomRestart (void)
{
    T1CONbits.TON = 1;    // enable Timer1
}

inline void ircomPause(int status)
{
    ircomData.paused = status;
}

void ircomEnableProximity ()
{
    ad_activate_proximity = 1;
}

void ircomDisableProximity ()
{
    ad_activate_proximity = 0;
}

void ircomEnableContinuousListening()
{
    ircomReceiveData.continuousListening = 1;
}

void ircomDisableContinuousListening()
{
    ircomReceiveData.continuousListening = 0;
}

void ircomSetOmnidirectional()
{
    ircomSendData.type = IRCOM_SEND_OMNI;
}

void ircomSetDirectional(int sensor)
{
    ircomSendData.type = IRCOM_SEND_DIRECTIONAL;
    ircomSendData.sensor = sensor % 4; // ir emitters are coupled... -> only 4 pulses
}



//==================================================================================
// the 1st interrupt is continuously called, it implements all the functions
// of ircom using finite state machines
void _ISRFAST _T1Interrupt(void)
{
    // clear interrupt flag
    IFS0bits.T1IF = 0;

    // system is not on pause
    if (ircomData.paused == 0)
    {
	// use appropriate FSM, if any
	switch (ircomData.fsm)
	{
	case IRCOM_FSM_RECEIVE :
	    ircomReceiveMain(); break;
	case IRCOM_FSM_SEND :
	    ircomSendMain(); break;
	}
    }
    
    // update time counter
    ircomData.time ++;
}

// IRCOM_C
#endif







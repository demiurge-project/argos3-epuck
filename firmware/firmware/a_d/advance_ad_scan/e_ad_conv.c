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

    This file was strongly inspired by another e_ad_conv.c from the 
    library released by EPFL. Authors of this "ancestor" are 
    Jean-Joël Borter, Michael Bonani, Valentin Longchamp.
*/

#include <e_epuck_ports.h>
#include <e_uart_char.h>
#include <e_led.h>
#include "e_ad_conv.h"
#include "../../ircom/ircom.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int ad_buffer1[NB_IR_SENSORS * SAMPLING_WINDOW];
int ad_buffer2[NB_IR_SENSORS * SAMPLING_WINDOW];

int* ad_sampling;
int* ad_received;

volatile char array_filled = 0;
volatile unsigned int e_last_ir_scan_id = 0;
volatile int e_ad_samples_to_skip = 0;

// mode can be 0 for continuous sampling or 1 for proximity
volatile int e_ad_mode;

// activate / deactivate proximity (long term)
volatile int ad_activate_proximity;

// enable disable (short term -> ircomReceive mainly)
volatile int ad_disable_proximity;
volatile int ad_disable_ircom;

volatile int e_ad_prox_trigger;
volatile int e_ad_prox_counter;
volatile int e_ad_prox_maxcounter;
volatile int e_ad_prox_index;
volatile int e_ad_prox_phase;

volatile int e_ambient_ir[NB_IR_SENSORS]; // ambient light measurement
volatile int e_ambient_and_reflected_ir[NB_IR_SENSORS]; // light when led is on
volatile int e_reflected_ir[NB_IR_SENSORS]; // variation of light
volatile int e_init_value_ir[NB_IR_SENSORS];


/**
 * Set up the different ADC register to process the AD conversion
 * by scanning the used AD channels. Each value of the channels will
 * be stored in a different AD buffer register and an inturrupt will
 * occure at the end of the scan.
 *
 * @param  void
 * @return void
 */

void e_init_ad_scan(void)
{
    // mode set to proximity by default
    e_ad_mode = 0;

    // proximity
    e_ad_prox_trigger = 0;
    ad_disable_proximity = 0;
    ad_activate_proximity = 1;
    int i;
    for (i = 0; i < NB_IR_SENSORS; i++)
    {
	e_init_value_ir[i] = 0;
	e_ambient_and_reflected_ir[i] = 0;
	e_ambient_ir[i] = 0;
	e_reflected_ir[i] = 0;
    }

    // continuous sampling
    ad_disable_ircom = 0;
    ad_sampling = ad_buffer1;
    ad_received = ad_buffer2;

    ADCON1 = 0;		//reset to default value
    ADCON2 = 0;		//reset to default value
    ADCON3 = 0;		//reset to default value
    ADCHS = 0;		//reset to default value

    // ADPCFGbits.PCFGx 
    // = 0 for Analog input mode, 
    // = 1 for digital input mode (default)
    ADPCFGbits.PCFG0 = 1;   // Debugger 
    ADPCFGbits.PCFG1 = 1;   // Debugger 
    ADPCFGbits.PCFG2 = 0;   // micro 0
    ADPCFGbits.PCFG3 = 0;   // micro 1
    ADPCFGbits.PCFG4 = 0;   // micro 2
    ADPCFGbits.PCFG5 = 0;   // axe x acc
    ADPCFGbits.PCFG6 = 0;   // axe y acc
    ADPCFGbits.PCFG7 = 0;   // axe z acc
    ADPCFGbits.PCFG8 = 0;   // ir0
    ADPCFGbits.PCFG9 = 0;   // ir1
    ADPCFGbits.PCFG10 = 0;  // ir2
    ADPCFGbits.PCFG11 = 0;  // ir3
    ADPCFGbits.PCFG12 = 0;  // ir4
    ADPCFGbits.PCFG13 = 0;  // ir5
    ADPCFGbits.PCFG14 = 0;  // ir6
    ADPCFGbits.PCFG15 = 0;  // ir7

    //specifie the channels to be scanned
    ADCSSLbits.CSSL0 = 0;   // Debugger
    ADCSSLbits.CSSL1 = 0;   // Debugger
    ADCSSLbits.CSSL2 = 0;   // micro 0
    ADCSSLbits.CSSL3 = 0;   // micro 1
    ADCSSLbits.CSSL4 = 0;   // micro 2
    ADCSSLbits.CSSL5 = 0;   // axe x acc
    ADCSSLbits.CSSL6 = 0;   // axe y acc
    ADCSSLbits.CSSL7 = 0;   // axe z acc
    ADCSSLbits.CSSL8 = 1;   // ir0
    ADCSSLbits.CSSL9 = 1;   // ir1
    ADCSSLbits.CSSL10 = 1;  // ir2
    ADCSSLbits.CSSL11 = 1;  // ir3
    ADCSSLbits.CSSL12 = 1;  // ir4
    ADCSSLbits.CSSL13 = 1;  // ir5
    ADCSSLbits.CSSL14 = 1;  // ir6
    ADCSSLbits.CSSL15 = 1;  // ir7

    ADCON1bits.FORM = 0;	//output = unsigned int
    ADCON1bits.ASAM = 1;	//automatic sampling on
    ADCON1bits.SSRC = 7;	//automatic convertion mode

    ADCON2bits.SMPI = 8-1;	//interupt on 8th sample
    ADCON2bits.CSCNA = 1;	//scan channel input mode on
	
    ADCON3bits.SAMC = 1;	//number of cycle between acquisition and conversion (need 2 for the prox)
    ADCON3bits.ADCS = ADCS_SETTING;	//Tad = (ADCS + SAMC) * Tcy/2 = 2170[ns], 
				//WARNING: Tad min must be 667 [ns]
				//WARNING: MAX 63 !!

    IFS0bits.ADIF = 0;	//Clear the A/D interrupt flag bit
    IEC0bits.ADIE = 1;	//Set the A/D interrupt enable bit
    IPC2bits.ADIP = 7;  // highest priority : sampling must be accurate !

    ADCON1bits.ADON = 1;	//enable AD conversion
}

void e_ad_scan_on(void)
{
    ADCON1bits.ADON = 1;	// enable AD conversion
    IEC0bits.ADIE = 1;          // enable AD interrupt
}

inline void e_ad_scan_reset(void)
{
    e_last_ir_scan_id = 0;      // reset array index
    array_filled = 0;	        // reset array filled flag
}

inline void e_ad_skip_samples (int samples_count)
{
    e_ad_samples_to_skip = samples_count;
    e_last_ir_scan_id = 0;      // reset array index
    array_filled = 0;	        // reset array filled flag
}

void e_ad_scan_off(void)
{
    ADCON1bits.ADON = 0;	// disable AD conversion
    IEC0bits.ADIE = 0;          // disable AD interrupt
}

void e_ad_ircom_interrupt()
{
    if (ad_disable_ircom == 1)
	return;

    if (e_ad_samples_to_skip > 0)
    {
	e_ad_samples_to_skip--;
	return;
    }
    
    memcpy (ad_sampling + e_last_ir_scan_id * NB_IR_SENSORS, (char *) &ADCBUF0, NB_IR_SENSORS * 2);
    e_last_ir_scan_id++;
    
    // Array full ?
    if (e_last_ir_scan_id >= SAMPLING_WINDOW) 
    {
	e_last_ir_scan_id = 0;
	array_filled = 1;
	
	// swap buffers
	int* tmp = ad_received;
	ad_received = ad_sampling;
	ad_sampling = tmp;
    }    
}

// sampling time = 81.3 usec
// counters limits are based on this value
void e_ad_proximity_interrupt()
{
    unsigned int readings[NB_IR_SENSORS];

    // check if we can enter the interrupt
    e_ad_prox_counter++;
    if (e_ad_prox_counter < e_ad_prox_maxcounter)
	return;

    // reset counter
    e_ad_prox_counter = 0;

    // acquisition of prox data is over, switch mode    
    if (e_ad_prox_index >= 4)
    {
	e_ad_scan_reset();
	e_ad_mode = 0;    
	return;
    }

    // copy ad buffer
    memcpy (readings, (char *) &ADCBUF0, NB_IR_SENSORS * 2);        

    // check current phase 
    int idx = e_ad_prox_index;
    if (e_ad_prox_phase == 0)
    {
	// ok ir beam is off, we can measure ambient light
	e_ambient_ir[idx] = (int)(readings[idx]);
	e_ambient_ir[idx+4] = (int)(readings[idx+4]);

	switch (idx)
	{
	case 0 : PULSE_IR0 = 1; break;
	case 1 : PULSE_IR1 = 1; break;
	case 2 : PULSE_IR2 = 1; break;
	case 3 : PULSE_IR3 = 1; break;
	}
	e_ad_prox_phase = 1;

	// set delay to enter inside interrupt next time (beam goes on, full power)
	e_ad_prox_maxcounter = 4; // 325 usec
    }
    else
    {	
	e_ambient_and_reflected_ir[idx] = (int)(readings[idx]);
	e_ambient_and_reflected_ir[idx+4] = (int)(readings[idx+4]);

	e_reflected_ir[idx] = e_ambient_ir[idx] - e_ambient_and_reflected_ir[idx];
	e_reflected_ir[idx+4] = e_ambient_ir[idx+4] - e_ambient_and_reflected_ir[idx+4];

	switch (idx)
	{
	case 0 : PULSE_IR0 = 0; break;
	case 1 : PULSE_IR1 = 0; break;
	case 2 : PULSE_IR2 = 0; break;
	case 3 : PULSE_IR3 = 0; break;
	}
	e_ad_prox_phase = 0;
	e_ad_prox_index++;

	// set delay to enter inside interrupt next time (beam goes on, full power)
	e_ad_prox_maxcounter = 26; // 2113 usec
    }
}


/**
 * Save the AD buffer registers in differents arrays
 *
 * @param  void
 * @return void
 */
void __attribute__((__interrupt__)) _ADCInterrupt(void)
{
    // clear the A/D Interrupt flag bit
    IFS0bits.ADIF = 0;

    // update prox counter
    if (ad_activate_proximity == 1)
	e_ad_prox_trigger++;

    // check if we switch mode
    if (e_ad_prox_trigger >= IRCOM_PROX_TRIGGER)
    {
	e_ad_prox_trigger = 0;

	if (ad_disable_proximity == 0)
	{
	    // shut down IR
	    PULSE_IR0 = 0;
	    PULSE_IR1 = 0;
	    PULSE_IR2 = 0;
	    PULSE_IR3 = 0;
	    
	    // set delay till IR beam goes off
	    e_ad_prox_counter = 0;
	    e_ad_prox_maxcounter = 26; // 2113 usec
	    e_ad_prox_phase = 0;
	    e_ad_prox_index = 0;
	    e_ad_mode = 1;
	}
    }

    // direct interrupt to proximity or ir communication
    switch (e_ad_mode)
    {
    case 0: e_ad_ircom_interrupt(); break;
    case 1: e_ad_proximity_interrupt(); break;
    }
}

inline char e_ad_is_array_filled(void)
{
    char result = array_filled;
    array_filled = 0;
    return result;
}

inline int e_get_prox(int s)
{
    return e_reflected_ir[s];
}

inline int e_get_calibrated_prox(int s)
{
    return e_reflected_ir[s] - e_init_value_ir[s];
}

inline int e_get_ambient_light(int s)
{
    return e_ambient_ir[s];
}

void e_calibrate_ir()
{
    long int tmp[NB_IR_SENSORS];    
    memset(tmp, 0, NB_IR_SENSORS * sizeof (long int));

    // take average of 100 samples
    int i, j;
    for (j = 0; j < 100; j++) 
    {
	// set up proximity
	e_ad_prox_trigger = 0;
	ad_disable_proximity = 1;
	ad_disable_ircom = 1;

	// wait till ad module is ready
	while (e_ad_mode == 1);
	
	e_ad_prox_counter = 0;
	e_ad_prox_maxcounter = 26; // 2113 usec
	e_ad_prox_phase = 0;
	e_ad_prox_index = 0;
	
	// start acquisition
	e_ad_mode = 1;
	
	// wait till sample arrived
	while (e_ad_mode == 1);

	// collect data
	for (i = 0; i < NB_IR_SENSORS; i++)
	{
	    tmp[i] += e_reflected_ir[i];
	}
    }

    ad_disable_proximity = 0;
    ad_disable_ircom = 0;

    // get mean values
    for (i = 0; i < NB_IR_SENSORS; i++) 
    {
	e_init_value_ir[i] = (int)(tmp[i] / 100.0);
    }
}


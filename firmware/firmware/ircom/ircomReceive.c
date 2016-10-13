
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


#ifndef IRCOM_RECEIVE_C
#define IRCOM_RECEIVE_C
#include <a_d/advance_ad_scan/e_ad_conv.h>
#include "ircom.h"
#include "ircomTools.h"
#include "ircomReceive.h"



#include <stdio.h>
#include <e_epuck_ports.h>
#include <p30f6014a.h>

// interface for end user
void ircomListen ()
{
    // stop processing messages
    ircomPause(1);

    // initialise FSM
    ircomReceiveData.done = 0;
    ircomReceiveData.receiving = 0;
    ircomReceiveData.state = 0;
    ircomData.fsm = IRCOM_FSM_RECEIVE;
    ircomReceiveData.distance = -1;
    
    // set interrupt trigger speed
    PR1 = (IRCOM_RECEIVESPEED * MICROSEC) / 8.0;
    
    // process messages again
    ircomPause(0);
}

// interface for end user
int ircomReceiveDone ()
{
    return ircomReceiveData.done;
}

inline int ircomIsReceiving ()
{
    if (ircomReceiveData.receiving == 1)
	return 1;
    else 
	return 0;
}

inline int ircomIsListening ()
{
    if (ircomData.fsm == IRCOM_FSM_RECEIVE)
	return 1;
    else 
	return 0;
}

void ircomStopListening ()
{
    // stop receiving
    ircomReceiveData.done = 0;
    ircomReceiveData.receiving = 0;    
    ircomReceiveData.state = 0;
    
    // reenable prox in ad
    ad_disable_proximity = 0;
    
    // set fsm in idle mode
    ircomData.fsm = IRCOM_FSM_IDLE;
}


//================================================================
// FSM Receive : entry point
// this should be called by AD interrupt 
// after a full window of samples has been acquired
void ircomReceiveMain()
{
    switch (ircomReceiveData.state)
    {
    case 0 :
	ircomReceiveListen(); break;
    case 1 : 
	ircomReceiveWord(); break;
    }
}

void ircomReceiveListen()
{
    // process signal only when a full window is available
    if (e_ad_is_array_filled() == 0)
	return;

    int maxSensor = ircomReceiveGetMaxSensor();
    
    // no significant modulation perceived ?
    if (maxSensor < 0)
	return;

    int switchCount = -1;
    ircomReceiveData.receivingSensor = maxSensor;
    if (maxSensor >= 0)
	switchCount = ircomReceiveDemodulate(1);

    // no significant signal perceived ?
    if (switchCount < IRCOM_DETECTION_THRESHOLD_SWITCH)
    {
	return;
    }
  
    // first thing, find out the shift of the signal
    int shift, u;
    int min = 4096;
    int max = 0;
    for (shift = 0, u = maxSensor; shift < IRCOM_SAMPLING_WINDOW; shift++, u += IRCOM_NB_IR_SENSORS)
    {
	int s = ad_received[u];
	if (s > max)
	    max = s;
	if (s < min)
	    min = s;
	if (max - min >= IRCOM_DETECTION_THRESHOLD_AMPLITUDE)
	    break;
    }
    // skip samples, restart sampling the 3rd bit only (2nd is already on the way, not synced)
    e_ad_skip_samples(IRCOM_SAMPLING_WINDOW - e_last_ir_scan_id + shift);

    // prevent ad from triggering prox sampling
    ad_disable_proximity = 1;

    // ok setup everything : start receiveing word, record all
    ircomReceiveData.currentBit = 0;
    ircomReceiveData.receivingSensor = maxSensor;
    ircomReceiveData.done = 0;
    ircomReceiveData.error = 0;
    ircomReceiveData.receiving = 1;    
    ircomReceiveData.state = 1;
}


// FSM Receive, state 2
// receive a word
void ircomReceiveWord()
{
    // process signal only when a full window is available
    if (e_ad_is_array_filled() == 0)
	return;

    int signal = ircomReceiveDemodulate(0);

    // no significant signal perceived ?
    if (signal < 0)
    {
	// stop receiveing, discard message
	ircomReceiveData.done = 1;
	ircomReceiveData.error = 1;
	ircomReceiveData.receiving = 0;    
	ircomReceiveData.state = 0;

	// reenable prox in ad
	ad_disable_proximity = 0;

	// set fsm in idle mode (except continuous listening mode...)
	if (ircomReceiveData.continuousListening == 1)
	    ircomListen();
	else
	    ircomData.fsm = IRCOM_FSM_IDLE;

	return;
    }
    
    ircomReceiveData.word[ircomReceiveData.currentBit++] = signal;

    // end of message ?
    if (ircomReceiveData.currentBit >= IRCOM_WORDSIZE + 2)
    {
	// evaluate CRC
	ircomReceiveData.error = ircomReceiveCheckCRC();

	// stop receiving
	ircomReceiveData.done = 1;
	ircomReceiveData.receiving = 0;    
	ircomReceiveData.state = 0;
	//Set distance and direction
	ircomReceiveData.distance = ircomEstimateDistance ( (int) ircomReceiveData.distance);
	ircomReceiveData.direction = ircomEstimateDirection( ircomReceiveData.receivingSensor); 

	// record in the msg queue
	long int value = ircomBin2Int(ircomReceiveData.word);
	ircomPushMessage(value, 
			 ircomReceiveData.distance, 
			 ircomReceiveData.direction, 
			 ircomReceiveData.receivingSensor, 
			 ircomReceiveData.error);

	// reenable prox in ad
	ad_disable_proximity = 0;

	// set fsm in idle mode (except continuous listening mode...)
	if (ircomReceiveData.continuousListening == 1)
	    ircomListen();
	else
	    ircomData.fsm = IRCOM_FSM_IDLE;

	return;	
    }
}

int ircomReceiveGetMaxSensor()
{
    //find the ir sensor with most interesting signal
    int i;

    int maxDiff = 0;
    int maxSensor = -1;

    for (i = 0; i < IRCOM_NB_IR_SENSORS; i++)
    {
	int index = i, j = 0, min = 4096, max = 0;
	for (j = 0; j < IRCOM_SAMPLING_WINDOW; j++)
	{
	    int v = ad_received[index];
	    if (v < min)
	    {
		min = v;
	    }
	    else if (v > max)
	    {
		max = v;
	    }
	    index += IRCOM_NB_IR_SENSORS;
	}
		    
	if (max - min > maxDiff)
	{
	    maxDiff = max - min;
	    maxSensor = i;
	}
    }

    if (maxDiff < IRCOM_DETECTION_THRESHOLD_AMPLITUDE)
	return -1;

    return maxSensor;
}

int ircomReceiveDemodulate (int rawOutput)
{
    int maxSensor = ircomReceiveData.receivingSensor;

    // demodulate signal
    //now we have the threshold and can normalize the signal if needed
    static int rs[IRCOM_SAMPLING_WINDOW];
    int i, u;
    for (i = 0, u = maxSensor; i < IRCOM_SAMPLING_WINDOW; i++, u += IRCOM_NB_IR_SENSORS)
    {
	rs[i] = ad_received[u];
    }

    // find max amplitude of signal
    int min = 4096, max = 0;
    for (i = 0; i < IRCOM_SAMPLING_WINDOW; i++)
    {
	if (rs[i] < min)
	{
	    min = rs[i];
	}
	else if (rs[i] > max)
	{
	    max = rs[i];
	}
    }
    if (max - min < IRCOM_DETECTION_THRESHOLD_AMPLITUDE)
	return -1;

    // compute mean signal
    long int tmp = 0;
    for (i = 0; i < IRCOM_SAMPLING_WINDOW; i++)
    {
	tmp += rs[i];
    }
    int mean = (int)(tmp / IRCOM_SAMPLING_WINDOW);

    // substract mean from signal
    for (i = 0; i < IRCOM_SAMPLING_WINDOW; i++)
    {
	rs[i] -= mean;
    }		

    // start counting number of switch around mean signal
    int signalState;
    if (rs[0] > 0)
	signalState = 1;
    else
	signalState = -1;
	
    int switchCount = 0;

    for (i = 1; i < IRCOM_SAMPLING_WINDOW; i++)
    {
	if(rs[i] > 0)
	{
	    if (signalState < 0)
	    {
		signalState = 1;
		switchCount++;
	    }
	}
	else
	{
	    if (signalState > 0)
	    {
		signalState = -1;
		switchCount++;
	    }
	}
    }

    if (rawOutput)
	return switchCount;

    if (switchCount >= IRCOM_MARK_THRESHOLD)
	{
		ircomReceiveData.distance = max - min;
		return IRCOM_MARK;
	}
    else if (switchCount >= IRCOM_SPACE_THRESHOLD)
		return IRCOM_SPACE;
    else
	return -1;
}

int ircomReceiveCheckCRC()
{
    // compute checksum
    int i;
    int bitSum = 0;
    for(i = 0; i < IRCOM_WORDSIZE; i++)
    {
	if (ircomReceiveData.word[i] == IRCOM_SPACE)
	    bitSum++;
    }
    int crc = ircomReceiveData.word[i] * 2 + ircomReceiveData.word[i + 1];

    // sum and get only the last 2 bits
    return ((crc + bitSum) & 0x003);
}


// IRCOM_RECEIVE_C
#endif


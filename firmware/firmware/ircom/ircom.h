
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

#ifndef IRCOM_H
#define IRCOM_H

#include "e_ad_conv.h"

// constants that define behaviour of ircom module
#define IRCOM_WORDSIZE 8 // bits
#define IRCOM_RECEIVESPEED 100.0 // microseconds
#define IRCOM_MARK_SAMPLE_PER_PERIOD 3.0 
#define IRCOM_SPACE_SAMPLE_PER_PERIOD 6.0 
#define IRCOM_SAMPLING_WINDOW 32
#define IRCOM_MARK 0
#define IRCOM_SPACE 1
#define IRCOM_MARK_THRESHOLD 8 // min number of signal switch
#define IRCOM_SPACE_THRESHOLD 4 // min number of signal switch
#define IRCOM_NB_IR_SENSORS 8
#define IRCOM_DETECTION_THRESHOLD_AMPLITUDE 10 // min amplitude of signal -> this is limiting the communication range
#define IRCOM_DETECTION_THRESHOLD_SWITCH 2 // min number of switch to initiate message
#define IRCOM_PROX_TRIGGER 2460 // trigger proximity procedure every 81.3 usec * IRCOM_PROX_TRIGGER

typedef char ircomWord[IRCOM_WORDSIZE + 4]; 

// message queue
typedef struct
{
    long int value;
    float distance;
    float direction;
    int receivingSensor;
    int error; // -1 = inexistent msg, 0 = all is ok, 1 = error in transmission
} IrcomMessage;


// structure that keeps all relevant information to run ircom module
// mainly which FSM is running
#define IRCOM_FSM_IDLE 0
#define IRCOM_FSM_RECEIVE 1
#define IRCOM_FSM_SEND 2
#define IRCOM_MESSAGES_MAX 20
typedef struct
{        
    int paused;
    int fsm;

    IrcomMessage messages[IRCOM_MESSAGES_MAX];
    int messagesBegin;
    int messagesEnd;
    int messagesCount;

    long int time;
} Ircom;

// structure that keeps track of all information related to receiveing
typedef struct
{
    // public :
    int receiving;
    int done;
    int error;
    ircomWord word;
    float distance;
    float direction;

    // private :
    int state;
    int currentBit;
    int receivingSensor;

    int continuousListening;
} IrcomReceive;

// structure that keeps track of all information related to writing
#define IRCOM_SEND_OMNI 0
#define IRCOM_SEND_DIRECTIONAL 1
typedef struct
{
    // public : 
    int done;
    ircomWord word;
    int type; // omni or directionnal        
    int sensor;

    // private : 
    int pulseState;
    int switchCounter;
    int switchCountMax;
    int signal;
    int currentBit;

    int markSwitchCount;
    int spaceSwitchCount;
    double markDuration;
    double spaceDuration;

    double durations[IRCOM_WORDSIZE+4];
    int switchCounts[IRCOM_WORDSIZE+4];
    int interruptedListening;
} IrcomSend;

extern volatile Ircom ircomData;
extern volatile IrcomReceive ircomReceiveData;
extern volatile IrcomSend ircomSendData;

//extern float sensorDir[NB_IR_SENSORS];

inline void ircomStart ();
inline void ircomStop (void);
inline void ircomRestart (void);
inline void ircomPause (int status);
void ircomEnableProximity ();
void ircomDisableProximity ();
void ircomEnableContinuousListening ();
void ircomDisableContinuousListening ();
void ircomSetOmnidirectional ();
void ircomSetDirectional (int sensor);

// make available to others read and write public functions
#include "ircomReceive.h"
#include "ircomSend.h"
#include "ircomTools.h"
#include "ircomMessages.h"


#endif

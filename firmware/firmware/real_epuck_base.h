/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * @file argos2/real_robot/epuck/real_epuck_base.h
 *
 * This file provides the data structures exchanged between the linux board and
 * the PIC of the epuck.
 * NOTE: To allow the correct alignment of the data structures some padding are
 * inserted. The paddings are marked with comments in the code.
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 * @author Gianpiero Francesca - <gianpiero.francesca@ulb.ac.be>
 */


#ifndef CREAL_EPUCK_BASE_H
#define CREAL_EPUCK_BASE_H

#ifdef __cplusplus
#include <argos2/common/utility/datatypes/datatypes.h>
#include <cstddef>
#include <iostream>
#else
#include "datatypes.h"
#endif

#ifdef __cplusplus
namespace argos {
#endif
    typedef struct{
        UInt8 RBRange; /*1 byte */
        UInt8 RBEnabled;/* 1 byte */
        UInt16 padding; /*2 byte PADDING */
    }BaseSensorConfiguration;
typedef struct {
    UInt16 Data;  // 2 bytes
    UInt16 Range; // 2 bytes
    Real Bearing; // 4 bytes
}BaseRangeAndBearingMessage;

typedef struct {
    UInt8 Data;         // 1 byte
    UInt8 padding;      // 1 byte PADDING
    UInt16 Sensor;      // 2 bytes
    Real Distance;      // 4 bytes
    Real Direction;     // 4 bytes
}BaseIRComMessage;

typedef struct {
   BaseRangeAndBearingMessage RBMessage; // 8 bytes
   SInt16 Proximity[8];                  // 8 x 2 bytes = 16 bytes
   UInt16  Battery_LOW;                   // 2 byte battery low
   SInt16 Accel[3];                      // (3 accelerator sensor readings) x 2 bytes = 8 bytes
   SInt16 Micro[4];                      // (3 micro-volume sensor readings + 1 of PADDING) x 2 bytes = 8 bytes
   SInt16 Light[8];                      // 8 x 2 bytes = 16 bytes
   UInt16 RBhasReceived;                 // 2 bytes
   UInt16 IRComHasReceived;              // 2 bytes
   BaseIRComMessage IRComMessage;        // 8 bytes
} BaseSensorState;

typedef struct {
   SInt16 LWheel;               // 2 bytes
   SInt16 RWheel;               // 2 bytes
   UInt16 RBMessageData;        // 2 bytes
   UInt8 RingLeds;              // 1 byte
   UInt8 OtherLeds;             // 1 byte
   //UInt16 IRComEnabled;       // 2 bytes
   //UInt16 Reset;              // 2 bytes
   UInt8 IRComEnabled;          // 1 byte
   UInt8 IRComMessageData;      // 1 byte
   UInt8 Reset;                 // 1 byte
   UInt8 Padding;               // PADDING 1 byte
   //UInt16 Padding2;           // PADDING 2 bytes

} BaseActuatorState;

#ifdef __cplusplus
bool isset(char un_byte, size_t un_pos);
std::ostream& operator<<(std::ostream& t_os, const BaseSensorState& s_state);
std::ostream& operator<<(std::ostream& t_os, const BaseActuatorState& s_state);
#endif

#ifdef __cplusplus
}
#endif

#endif

/**
 * @file <argos3/plugins/robots/e-puck/real_robot/real_epuck_base.h>
 *
 * This file provides the data structures exchanged between the linux
 * board and the PIC of the epuck.
 *
 * NOTE: To allow the correct alignment of the data structures some
 * padding are inserted. The paddings are marked with comments in the
 * code.
 * @author Lorenzo Garattoni <lgaratto@ulb.ac.be>
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef REAL_EPUCK_BASE_H
#define REAL_EPUCK_BASE_H
#ifdef __cplusplus
#include <argos3/core/utility/datatypes/datatypes.h>
#include <cstddef>
#include <iostream>
#else
#include "datatypes.h"
#endif
#ifdef __cplusplus
namespace argos {
#endif
/**
    * Structure to store the sensor configuration
    */
typedef struct{
    UInt8 RBRange;   // 1 byte
    UInt8 RBEnabled; // 1 byte
    UInt16 padding;  // 2 byte PADDING
} BaseSensorConfiguration;

/**
    * Structure to store a RAB message
    */
typedef struct {
    UInt16 Data;  // 2 bytes
    UInt16 Range; // 2 bytes
    Real Bearing; // 4 bytes
} BaseRangeAndBearingMessage;

/**
    * Structure to store a IR message
    */
typedef struct {
    UInt8 Data;     // 1 byte
    UInt8 padding;  // 1 byte PADDING
    UInt16 Sensor;  // 2 bytes
    Real Range;  // 4 bytes
    Real Bearing; // 4 bytes
} BaseIRComMessage;

/**
    * Structure to store the sensors datas
    */
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

/**
    * Structure to store the actuators data
    */
typedef struct {
    SInt16 LWheel;               // 2 bytes
    SInt16 RWheel;               // 2 bytes
    UInt16 RBMessageData;        // 2 bytes
    UInt8 BaseLEDs;              // 1 byte
    UInt8 OtherLEDs;             // 1 byte
    UInt8 IRComEnabled;          // 1 byte
    UInt8 IRComMessageData;      // 1 byte
    UInt8 Reset;                 // 1 byte
    UInt8 padding;               // PADDING 1 byte
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

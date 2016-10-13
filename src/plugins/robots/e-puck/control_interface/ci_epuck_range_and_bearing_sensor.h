/**
 * @file <argos3/plugins/robots/e-puck/control_interface/ci_epuck_range_and_bearing_sensor.h>
 *
 * This file provides the definition of the sensor part of the range and bearing system.
 *
 * The range and bearing sensor/actuator pair allows robots to
 * sense other robots in their locality and exchange data with
 * them.  When the presence of a robot cannot be sensed anymore
 * (i.e., the sensing robot does not receive an IR beam from the
 * robot), the data sent over the local radio is ignored. Thus,
 * this sensor/actuator pair provides localization and local
 * communication to the robot.  The range of the sensor depends on
 * the robot type.  This sensor can receive only ONE message for
 * each time step.  For each message, the following data is
 * provided:
 *
 *  1) range (=distance) in meters relative to the sensing robot
 *  2) bearing horizontal (=angle) in signed radians relative to the sensing robot
 *  3) data (8,16,24,or 32 bits) transmitted by the other robot
 *
 * All this data is encapsulated in a
 * TRangeAndBearingReceivedPacket struct.
 *
 * The sensor provides a vector that holds the received
 * packets.  The vector can be accessed by calling the
 * CCI_RangeAndBearingSensor::GetPackets() method.
 *
 * @author Lorenzo Garattoni- <lgaratto@ulb.ac.be>
 */

#ifndef CCI_EPUCK_RANGE_AND_BEARING_SENSOR_H
#define CCI_EPUCK_RANGE_AND_BEARING_SENSOR_H

namespace argos {
   class CCI_EPuckRangeAndBearingSensor;
}

#include <argos3/core/utility/math/angles.h>
#include <argos3/core/control_interface/ci_sensor.h>

#include <vector>
#include <iostream>

namespace argos {

   class CCI_EPuckRangeAndBearingSensor : public CCI_Sensor {

   public:
      /* the number of bytes authorized by the RAB board */
      static const UInt32 MAX_BYTES_RECEIVED = 4;

   public:
      typedef UInt8 TData[MAX_BYTES_RECEIVED];

      /**
       * This struct defines a received range and bearing packet.  It
       * holds the id of the sending robot, the distance (in cms), the
       * horizontal, the packet payload data (8 bits) and a packet id,
       * that is sequentially increased each time a packet is received.
       *
       * Please note that this packet is only used to hold data on the
       * receiving end of the communication.
       */
      struct SReceivedPacket {

         /**
          * Distance in cm.
          */
         Real Range;

         /**
          * Angle with respect to the robot heading.
          * The X axis corresponds to the front of the robot;
          * the Y axis corresponds to the left wheel.
          */
         CRadians Bearing;

         /**
          * Datatype for the range and bearing payload.
          */
         TData Data;

         /**
          * Empty constructor
          */
         SReceivedPacket() :
            Range(0.0) {
            Data[0] = 0;
            Data[1] = 0;
            Data[2] = 0;
            Data[3] = 0;
         }

         /**
          * Constructor of a packet from another one
          */
         SReceivedPacket(const SReceivedPacket & t_packet) :
            Range(t_packet.Range),
            Bearing(t_packet.Bearing) {
            Data[0] = t_packet.Data[0];
            Data[1] = t_packet.Data[1];
            Data[2] = t_packet.Data[2];
            Data[3] = t_packet.Data[3];
         }

         /**
          * Redefine the "=" operator
          */
         SReceivedPacket& operator=(const SReceivedPacket & t_packet) {
            if (&t_packet != this) {
               Range = t_packet.Range;
               Bearing = t_packet.Bearing;
               Data[0] = t_packet.Data[0];
               Data[1] = t_packet.Data[1];
               Data[2] = t_packet.Data[2];
               Data[3] = t_packet.Data[3];
            }
            return *this;
         }

         /**
          * Redefine the "<<" operator with a packet
          */
         friend std::ostream& operator<<(std::ostream& os,
                                         const SReceivedPacket & t_packet);
      };

      /**
       * type alias for received packets
       */
      typedef std::vector<SReceivedPacket*> TPackets;

   public:

      /**
       * Destructor
       */
      ~CCI_EPuckRangeAndBearingSensor() {}

      void Init(TConfigurationNode& t_node);

      /**
       * Clears the messages received from the range and bearing.
       * Call this at the end of your time-step to be sure that at each
       * time-step you only have the most recently received packets.
       */
      void ClearPackets();

      /**
       * Returns the currently stored packets.
       * @return The currently stored packets
       * @see ClearPackets
       */
      inline const TPackets& GetPackets() const {
         return m_tPackets;
      }

   protected:

      UInt8 m_uDataSize;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

   protected:

      TPackets m_tPackets;
   };
}
#endif

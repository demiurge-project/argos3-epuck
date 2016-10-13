/**
 * @file <argos3/plugins/robots/e-puck/control_interface/ci_epuck_ircom_sensor.h
 *
 * @brief This file provides the control interface of the e-puck IR Communication sensor
 *
 * @author Lorenzo Garattoni- <lgaratto@ulb.ac.be>
 */

#ifndef CCI_EPUCK_IRCOM_SENSOR_H
#define CCI_EPUCK_IRCOM_SENSOR_H

namespace argos {
   class CCI_EPuckIRComSensor;
}

#include <string>
#include <vector>
#include <iostream>

#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/utility/math/angles.h>
#include <argos3/core/utility/logging/argos_log.h>

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   class CCI_EPuckIRComSensor : public CCI_Sensor {

   public:

      /**
       * This struct defines a received range and bearing packet.
       * It holds the id of the sending robot, the distance (in cms), the horizontal,
       * the packet payload data (8 bits) and a packet id, that is sequentially
       * increased each time a packet is received.
       *
       * Please note that this packet is only used to hold data on the receiving
       * end of the communication.
       */
      struct SPacket {
         /**
          * The data of the packet
          */
         UInt8 Data;         // 1 byte

         /**
          * Id of the sensor
          */
         UInt16 Sensor;      // 2 bytes

         /**
          * Range of emitter
          */
         Real Range;      // 4 bytes

         /**
          * Bearing of the packet
          */
         CRadians Bearing;

         SPacket():
            Data(0),
            Sensor(0),
            Range(0)
         {}

         friend std::ostream& operator<<(std::ostream& os,
                                         const SPacket& t_packet) {
            os << "IRCOM_RECEIVED_DATA < data = " << t_packet.Data
               << ", Sensor = " << t_packet.Sensor
               << ", Range = " << t_packet.Range
               << ", Bearing = " << t_packet.Bearing;
            return os;
         }
      };

   public:

      /** Type for the vector that holds the latest received packet for each robot */
      typedef std::vector<SPacket*> TPackets;

   public:

      virtual ~CCI_EPuckIRComSensor() {}

      /**
       * Clears the list of packets.
       */
      void ClearPackets() {
         while(!m_tPackets.empty()) {
            delete m_tPackets.back();
            m_tPackets.pop_back();
         }
      }

      /**
       * Returns the list of packets.
       */
      inline const TPackets& GetPackets() const {
         return m_tPackets;
      }

       /****************************************/
       /****************************************/

    #ifdef ARGOS_WITH_LUA
       virtual void CreateLuaState(lua_State* pt_lua_state) {
          CLuaUtility::OpenRobotStateTable(pt_lua_state, "ircom");
          for(size_t i = 0; i < m_tPackets.size(); ++i) {
             CLuaUtility::StartTable(pt_lua_state, i+1);
             CLuaUtility::AddToTable(pt_lua_state, "range", m_tPackets[i]->Range);
             CLuaUtility::AddToTable(pt_lua_state, "sensor", m_tPackets[i]->Sensor);
             CLuaUtility::AddToTable(pt_lua_state, "data", m_tPackets[i]->Data);
             CLuaUtility::AddToTable(pt_lua_state, "bearing", m_tPackets[i]->Bearing);
             CLuaUtility::EndTable(pt_lua_state);
          }
          CLuaUtility::CloseRobotStateTable(pt_lua_state);
       }
    #endif

       /****************************************/
       /****************************************/

    #ifdef ARGOS_WITH_LUA
       virtual void ReadingsToLuaState(lua_State* pt_lua_state) {
          lua_getfield(pt_lua_state, -1, "ircom");
          /* Save the number of elements in the RAB table */
          size_t unLastMsgNum = lua_rawlen(pt_lua_state, -1);
          /* Overwrite the table with the new messages */
          for(size_t i = 0; i < m_tPackets.size(); ++i) {
             CLuaUtility::StartTable(pt_lua_state, i+1);
             CLuaUtility::AddToTable(pt_lua_state, "range", m_tPackets[i]->Range);
             CLuaUtility::AddToTable(pt_lua_state, "sensor", m_tPackets[i]->Sensor);
             CLuaUtility::AddToTable(pt_lua_state, "data", m_tPackets[i]->Data);
             CLuaUtility::AddToTable(pt_lua_state, "bearing", m_tPackets[i]->Bearing);
             CLuaUtility::EndTable(pt_lua_state);
          }
          /* Are the new messages less than the old ones? */
          if(m_tPackets.size() < unLastMsgNum) {
             /* Yes, set to nil all the extra entries */
             for(size_t i = m_tPackets.size()+1; i <= unLastMsgNum; ++i) {
                lua_pushnumber(pt_lua_state,  i);
                lua_pushnil   (pt_lua_state    );
                lua_settable  (pt_lua_state, -3);
             }
          }
          lua_pop(pt_lua_state, 1);
       }
    #endif

   protected:

      TPackets m_tPackets;
   };
}
#endif

/**
 * @file <argos3/plugins/robots/e-puck/control_interface/ci_epuck_range_and_bearing_sensor.cpp>
 *
 * @author Lorenzo Garattoni- <lgaratto@ulb.ac.be>
 */

#include "ci_epuck_range_and_bearing_sensor.h"
#include <argos3/core/utility/logging/argos_log.h>

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

   void CCI_EPuckRangeAndBearingSensor::Init(TConfigurationNode& t_node) {
      /* get the data size from the configuration file or set it to the data struct size */
      /* sizeof return the number of byte */
      GetNodeAttributeOrDefault(t_node, "data_size", m_uDataSize, sizeof(TData));
      if (m_uDataSize > sizeof(TData)) {
         LOGERR << "[WARNING] RAB Sensor data_size=" << m_uDataSize
                << ", value incorrect, back to default value data_size=" << sizeof(TData) << std::endl;
         m_uDataSize = sizeof(TData);
      }
   }

   void CCI_EPuckRangeAndBearingSensor::ClearPackets() {
      while(!m_tPackets.empty()) {
         delete m_tPackets.back();
         m_tPackets.pop_back();
      }
   }

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_EPuckRangeAndBearingSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "range_and_bearing");
      for(size_t i = 0; i < m_tPackets.size(); ++i) {
         CLuaUtility::StartTable(pt_lua_state, i+1);
         CLuaUtility::AddToTable(pt_lua_state, "range", m_tPackets[i]->Range);
         CLuaUtility::AddToTable(pt_lua_state, "bearing", m_tPackets[i]->Bearing);
         CLuaUtility::StartTable(pt_lua_state, "data");

         CLuaUtility::AddToTable(pt_lua_state, 1, m_tPackets[i]->Data[0]);
         CLuaUtility::AddToTable(pt_lua_state, 2, m_tPackets[i]->Data[1]);
         CLuaUtility::AddToTable(pt_lua_state, 3, m_tPackets[i]->Data[2]);
         CLuaUtility::AddToTable(pt_lua_state, 4, m_tPackets[i]->Data[3]);

         CLuaUtility::EndTable(pt_lua_state);
         CLuaUtility::EndTable(pt_lua_state);
      }
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_EPuckRangeAndBearingSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "range_and_bearing");
      /* Save the number of elements in the RAB table */
      size_t unLastMsgNum = lua_rawlen(pt_lua_state, -1);
      /* Overwrite the table with the new messages */
      for(size_t i = 0; i < m_tPackets.size(); ++i) {
         CLuaUtility::StartTable(pt_lua_state, i+1);
         CLuaUtility::AddToTable(pt_lua_state, "range", m_tPackets[i]->Range);
         CLuaUtility::AddToTable(pt_lua_state, "bearing", m_tPackets[i]->Bearing);
         CLuaUtility::StartTable(pt_lua_state, "data");
         
         CLuaUtility::AddToTable(pt_lua_state, 1, m_tPackets[i]->Data[0]);
         CLuaUtility::AddToTable(pt_lua_state, 2, m_tPackets[i]->Data[1]);
         CLuaUtility::AddToTable(pt_lua_state, 3, m_tPackets[i]->Data[2]);
         CLuaUtility::AddToTable(pt_lua_state, 4, m_tPackets[i]->Data[3]);

         CLuaUtility::EndTable(pt_lua_state);
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

   /****************************************/
   /****************************************/

   std::ostream& operator<<(std::ostream& os,
                            const CCI_EPuckRangeAndBearingSensor::SReceivedPacket & t_packet) {
      os << "RAB_RECV_DATA < range = " << t_packet.Range
         << ", bearing horizontal = " << t_packet.Bearing
         << ", data = " << t_packet.Data;
      return os;
   }

   /****************************************/
   /****************************************/

}

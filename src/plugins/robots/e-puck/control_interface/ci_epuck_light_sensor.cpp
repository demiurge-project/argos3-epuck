/**
 * @file <argos3/plugins/robots/e-puck/control_interface/ci_epuck_light_sensor.cpp>
 *
 * @author Lorenzo Garattoni- <lgaratto@ulb.ac.be>
 */

#include "ci_epuck_light_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

   CCI_EPuckLightSensor::CCI_EPuckLightSensor() :
      m_tReadings(8) {
      // Set the values for the proximity sensor angles
      m_tReadings[0].Angle = CRadians::PI / 10.5884f;
      m_tReadings[1].Angle = CRadians::PI / 3.5999f;
      m_tReadings[2].Angle = CRadians::PI_OVER_TWO;
      m_tReadings[3].Angle = CRadians::PI / 1.2f;
      m_tReadings[4].Angle = CRadians::PI / 0.8571f;
      m_tReadings[5].Angle = CRadians::PI / 0.6667f;
      m_tReadings[6].Angle = CRadians::PI / 0.5806f;
      m_tReadings[7].Angle = CRadians::PI / 0.5247f;
   }

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_EPuckLightSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "light");
      for(size_t i = 0; i < GetReadings().size(); ++i) {
         CLuaUtility::StartTable(pt_lua_state, i+1                           );
         CLuaUtility::AddToTable(pt_lua_state, "angle",  m_tReadings[i].Angle);
         CLuaUtility::AddToTable(pt_lua_state, "value",  m_tReadings[i].Value);
         CLuaUtility::EndTable  (pt_lua_state                                );
      }
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_EPuckLightSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "light");
      for(size_t i = 0; i < GetReadings().size(); ++i) {
         lua_pushnumber(pt_lua_state, i+1                 );
         lua_gettable  (pt_lua_state, -2                  );
         lua_pushnumber(pt_lua_state, m_tReadings[i].Value);
         lua_setfield  (pt_lua_state, -2, "value"         );
         lua_pop(pt_lua_state, 1);
      }
      lua_pop(pt_lua_state, 1);
   }
#endif

   /****************************************/
   /****************************************/

   std::ostream& operator<<(std::ostream& c_os,
                            const CCI_EPuckLightSensor::SReading& s_reading) {
      c_os << "SReading("
           << s_reading.Value << ","
           << s_reading.Angle << ")";
      return c_os;
   }

   /****************************************/
   /****************************************/

   std::ostream& operator<<(std::ostream& c_os,
                            const CCI_EPuckLightSensor::TReadings& t_readings) {
      if(! t_readings.empty()) {
         c_os << t_readings[0];
         for(UInt16 i = 1; i < t_readings.size(); ++i) {
            c_os << " " << t_readings[i];
         }
      }
      return c_os;
   }

   /****************************************/
   /****************************************/
}

/**
 * @file <argos3/plugins/robots/e-puck/control_interface/ci_epuck_battery_sensor.cpp>
 *
 */

#include "ci_epuck_battery_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   
   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_EPuckBatterySensor::CreateLuaState(lua_State* pt_lua_state) {
       CLuaUtility::StartTable(pt_lua_state, "battery");
      CLuaUtility::AddToTable(pt_lua_state, "value",   m_fBatteryLevel  );
      CLuaUtility::EndTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_EPuckBatterySensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "battery");
      lua_pushnumber(pt_lua_state, m_fBatteryLevel  );
      lua_pop(pt_lua_state, 1);
   }
#endif

   /****************************************/
   /****************************************/
}

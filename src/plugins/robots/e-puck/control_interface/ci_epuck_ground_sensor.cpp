/**
 * @file <argos3/plugins/robots/e-puck/control_interface/ci_epuck_ground_sensor.cpp>
 *
 * @author Lorenzo Garattoni- <lgaratto@ulb.ac.be>
 */

#include "ci_epuck_ground_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_EPuckGroundSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::StartTable(pt_lua_state, "ground");
      CLuaUtility::AddToTable(pt_lua_state, "left",   m_sReadings.Left  );
      CLuaUtility::AddToTable(pt_lua_state, "center", m_sReadings.Center);
      CLuaUtility::AddToTable(pt_lua_state, "right",  m_sReadings.Right );
      CLuaUtility::EndTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_EPuckGroundSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "ground");
      lua_pushnumber(pt_lua_state, m_sReadings.Left  );
      lua_setfield  (pt_lua_state, -2, "left"        );
      lua_pushnumber(pt_lua_state, m_sReadings.Center);
      lua_setfield  (pt_lua_state, -2, "center"      );
      lua_pushnumber(pt_lua_state, m_sReadings.Right );
      lua_setfield  (pt_lua_state, -2, "right"       );
      lua_pop(pt_lua_state, 1);
   }
#endif


   /****************************************/
   /****************************************/

}

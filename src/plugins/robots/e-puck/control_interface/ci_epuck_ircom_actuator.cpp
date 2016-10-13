/**
 * @file <argos3/plugins/robots/e-puck/control_interface/ci_epuck_ircom_actuator.h
 *
 * @brief This file provides the control interface of the e-puck IR Communication actuator
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#include "ci_epuck_ircom_actuator.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   // The stack should have one value in the range [0,255]
   int LuaIRComSetData(lua_State* pt_lua_state) {
      if(lua_gettop(pt_lua_state) == 1) {
         /* Check parameter */
         luaL_checktype(pt_lua_state, 1, LUA_TNUMBER);
         /* Get reference to actuator */
         CCI_EPuckIRComActuator* pcAct = CLuaUtility::GetDeviceInstance<CCI_EPuckIRComActuator>(pt_lua_state, "ircom");
         /* Check parameter values */
         UInt8 unData = lua_tonumber(pt_lua_state, 1);
         /* Perform action */
         pcAct->SetData(unData);
         return 0;
      }
      else {
         return luaL_error(pt_lua_state, "robot.ircom.set_data() expects one argument");
      }
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_EPuckIRComActuator::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::StartTable(pt_lua_state, "ircom");
      CLuaUtility::AddToTable(pt_lua_state, "_instance", this);
      CLuaUtility::AddToTable(pt_lua_state, "set_data", &argos::LuaIRComSetData);
      CLuaUtility::EndTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

}


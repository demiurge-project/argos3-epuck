/**
 * @file <argos3/plugins/robots/e-puck/control_interface/ci_epuck_base_leds_actuator.cpp
 *
 * @author Lorenzo Garattoni- <lgaratto@ulb.ac.be>
 */

#include "ci_epuck_base_leds_actuator.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   /*
    * This function expects the stack to have either two arguments:
    * 1. the index of the LED to set;
    * 2. the desired state for the LED (on/off) as a boolean value
    */
   int LuaSwitchLED(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 2) {
         return luaL_error(pt_lua_state, "robot.base_leds.switch_led() expects 2 arguments");
      }
      luaL_checktype(pt_lua_state, 1, LUA_TNUMBER);
      luaL_checktype(pt_lua_state, 2, LUA_TBOOLEAN);
      size_t unIdx = lua_tonumber(pt_lua_state, 1);
      if(unIdx < 1 || unIdx > 8) {
         return luaL_error(pt_lua_state, "passed index %d out of bounds [1,8]", unIdx);
      }
      /* Perform action */
      CLuaUtility::GetDeviceInstance<CCI_EPuckBaseLEDsActuator>(pt_lua_state, "base_leds")->
         SwitchLED(unIdx - 1,
                   lua_toboolean(pt_lua_state, 2));
      return 0;
   }

   /*
    * This function expects the stack to have one argument, the desired state
    * for the LED (on/off) as a boolean value.
    */
   int LuaSwitchLEDs(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 1) {
         return luaL_error(pt_lua_state, "robot.base_leds.switch_leds() expects 1 argument");
      }
      luaL_checktype(pt_lua_state, 1, LUA_TBOOLEAN);
      /* Perform action */
      CLuaUtility::GetDeviceInstance<CCI_EPuckBaseLEDsActuator>(pt_lua_state, "base_leds")->
         SwitchLEDs(lua_toboolean(pt_lua_state, 1));
      return 0;
   }
#endif

   /****************************************/
   /****************************************/

   CCI_EPuckBaseLEDsActuator::CCI_EPuckBaseLEDsActuator() :
      m_unLEDSettings(0) {
   }

   /****************************************/
   /****************************************/

   void CCI_EPuckBaseLEDsActuator::SwitchLED(size_t un_led_number,
                                             bool b_switched_on) {
      if(b_switched_on) {
         m_unLEDSettings |= (1 << un_led_number);
      } else {
         m_unLEDSettings &= ~(1 << un_led_number);
      }
   }

   /****************************************/
   /****************************************/

   void CCI_EPuckBaseLEDsActuator::SwitchLEDs(bool b_switched_on) {
      if(b_switched_on) {
         m_unLEDSettings = 255;
      } else {
         m_unLEDSettings = 0;
      }
   }

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_EPuckBaseLEDsActuator::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "base_leds");
      CLuaUtility::AddToTable(pt_lua_state, "_instance", this);
      CLuaUtility::AddToTable(pt_lua_state, "switch_led", &LuaSwitchLED);
      CLuaUtility::AddToTable(pt_lua_state, "switch_leds", &LuaSwitchLEDs);
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

}

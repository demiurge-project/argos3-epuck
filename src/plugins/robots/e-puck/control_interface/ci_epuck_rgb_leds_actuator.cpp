/**
 * @file <argos3/plugins/robots/e-puck/control_interface/ci_epuck_RGB_leds_actuator.h
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#include "ci_epuck_rgb_leds_actuator.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

#ifdef ARGOS_WITH_LUA
   /*
    * This function expects the stack to have either two or four arguments.
    * The first argument must always be the index of the LED to set.
    * Then, in case two arguments are passed, the second argument can be the string
    * definition of a color. In case of four arguments, the RGB values are expected.
    */
   int LuaLEDSetColor(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 2 && lua_gettop(pt_lua_state) != 4) {
         return luaL_error(pt_lua_state, "robot.overo_leds.set_single_color() expects 2 or 4 arguments");
      }
      luaL_checktype(pt_lua_state, 1, LUA_TNUMBER);
      size_t unIdx = lua_tonumber(pt_lua_state, 1);
      /* Get reference to actuator */
      CCI_EPuckRGBLEDsActuator* pcAct = CLuaUtility::GetDeviceInstance<CCI_EPuckRGBLEDsActuator>(pt_lua_state, "overo_leds");
      if(unIdx < 1 || unIdx > 3) {
         return luaL_error(pt_lua_state, "passed index %d out of bounds [1,3]", unIdx);
      }
      /* Create color buffer */
      CColor cColor;
      if(lua_gettop(pt_lua_state) == 2) {
         luaL_checktype(pt_lua_state, 2, LUA_TSTRING);
         try {
            cColor.Set(lua_tostring(pt_lua_state, 2));
         }
         catch(CARGoSException& ex) {
            return luaL_error(pt_lua_state, ex.what());
         }
      }
      else {
         luaL_checktype(pt_lua_state, 2, LUA_TNUMBER);
         luaL_checktype(pt_lua_state, 3, LUA_TNUMBER);
         luaL_checktype(pt_lua_state, 4, LUA_TNUMBER);
         cColor.Set(lua_tonumber(pt_lua_state, 2),
                    lua_tonumber(pt_lua_state, 3),
                    lua_tonumber(pt_lua_state, 4));
      }
      /* Perform action */
      pcAct->SetColor(unIdx - 1, cColor);
      return 0;
   }

   /*
    * This function expects the stack to have either one or three arguments.
    * In case one argument is passed, it must be the string definition of a color.
    * In case of three arguments, the RGB values are expected.
    */
   int LuaLEDSetColors(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 1 && lua_gettop(pt_lua_state) != 3) {
         return luaL_error(pt_lua_state, "robot.overo_leds.set_all_colors() expects 1 or 3 arguments");
      }
      /* Create color buffer */
      CColor cColor;
      if(lua_gettop(pt_lua_state) == 1) {
         luaL_checktype(pt_lua_state, 1, LUA_TSTRING);
         try {
            cColor.Set(lua_tostring(pt_lua_state, 1));
         }
         catch(CARGoSException& ex) {
            return luaL_error(pt_lua_state, ex.what());
         }
      }
      else {
         luaL_checktype(pt_lua_state, 1, LUA_TNUMBER);
         luaL_checktype(pt_lua_state, 2, LUA_TNUMBER);
         luaL_checktype(pt_lua_state, 3, LUA_TNUMBER);
         cColor.Set(lua_tonumber(pt_lua_state, 1),
                    lua_tonumber(pt_lua_state, 2),
                    lua_tonumber(pt_lua_state, 3));
      }
      /* Perform action */
      CLuaUtility::GetDeviceInstance<CCI_EPuckRGBLEDsActuator>(pt_lua_state, "overo_leds")->SetColors(cColor);
      return 0;
   }
#endif

   /****************************************/
   /****************************************/

   CCI_EPuckRGBLEDsActuator::CCI_EPuckRGBLEDsActuator() :
      m_tLEDSettings(3) {}

   /****************************************/
   /****************************************/

   void CCI_EPuckRGBLEDsActuator::SetColor(size_t un_led_number,
                                                 const CColor& c_color) {
      m_tLEDSettings[un_led_number] = c_color;
   }

   /****************************************/
   /****************************************/

   void CCI_EPuckRGBLEDsActuator::SetColors(const CColor& c_color) {
      SetColor(0, c_color);
      SetColor(1, c_color);
      SetColor(2, c_color);
   }

   /****************************************/
   /****************************************/

   void CCI_EPuckRGBLEDsActuator::SetColors(const TLEDSettings& t_colors) {
      SetColor(0, t_colors[0]);
      SetColor(1, t_colors[1]);
      SetColor(2, t_colors[2]);
   }

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_EPuckRGBLEDsActuator::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "overo_leds");
      CLuaUtility::AddToTable(pt_lua_state, "_instance", this);
      CLuaUtility::AddToTable(pt_lua_state, "set_single_color", &LuaLEDSetColor);
      CLuaUtility::AddToTable(pt_lua_state, "set_all_colors", &LuaLEDSetColors);
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

}

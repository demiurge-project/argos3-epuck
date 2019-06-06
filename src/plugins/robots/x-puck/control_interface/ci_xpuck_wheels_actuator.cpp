#include "ci_xpuck_wheels_actuator.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

   const Real CCI_XPuckWheelsActuator::MIN_VELOCITY_CM_SEC = -30.0;
   const Real CCI_XPuckWheelsActuator::MAX_VELOCITY_CM_SEC = 30.0;

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   /*
    * The stack must have two values in this order:
    * 1. left wheel speed (a number)
    * 2. right wheel speed (a number)
    */
   int LuaSetLinearVelocity(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 2) {
         return luaL_error(pt_lua_state, "robot.wheels.set_velocity() expects 2 arguments");
      }
      luaL_checktype(pt_lua_state, 1, LUA_TNUMBER);
      luaL_checktype(pt_lua_state, 2, LUA_TNUMBER);
      /* Perform action */
      CLuaUtility::GetDeviceInstance<CCI_XPuckWheelsActuator>(pt_lua_state, "wheels")->
         SetLinearVelocity(lua_tonumber(pt_lua_state, 1),
                           lua_tonumber(pt_lua_state, 2));
      return 0;
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_XPuckWheelsActuator::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "wheels");
      CLuaUtility::AddToTable(pt_lua_state, "_instance", this);
      CLuaUtility::AddToTable(pt_lua_state, "set_velocity", &LuaSetLinearVelocity);
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

}

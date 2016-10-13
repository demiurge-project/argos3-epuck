/**
 * @file <argos3/plugins/robots/e-puck/control_interface/ci_epuck_ircom_actuator.h
 *
 * @brief This file provides the control interface of the e-puck IR Communication actuator
 *
 * @author Lorenzo Garattoni- <lgaratto@ulb.ac.be>
 */

#ifndef CCI_EPUCK_IRCOM_ACTUATOR_H
#define CCI_EPUCK_IRCOM_ACTUATOR_H

namespace argos {
   class CCI_EPuckIRComActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   class CCI_EPuckIRComActuator : public CCI_Actuator {

   public:

      virtual ~CCI_EPuckIRComActuator() {}

      /**
       * Set the data to send.
       * This will be ignored if the last packet has not been sent yet.
       * @param t_data The data to send.
       */
      virtual void SetData(UInt8 un_data) = 0;

      /**
       * Set the state of the actuator.
       * @param b_enabled <tt>true</tt> for on, <tt>false</tt> for off.
       */
      virtual void SetEnabled(bool b_enabled) = 0;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
#endif

   };

}
#endif

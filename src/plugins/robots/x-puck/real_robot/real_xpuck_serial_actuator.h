/**
 * @file <argos3/plugins/robots/x-puck/real_robot/real_xpuck_serial_actuator.h>
 *
 * @brief This file provides the interface for the xpuck uart communication.
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#ifndef REAL_XPUCK_SERIAL_ACTUATOR_H
#define REAL_XPUCK_SERIAL_ACTUATOR_H

namespace argos {
   class CRealXPuckSerialActuator;
}

#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/plugins/robots/x-puck/real_robot/real_xpuck_base.h>

namespace argos {

   class CRealXPuckSerialActuator {

   public:

      virtual ~CRealXPuckSerialActuator() {}

      /**
       * Sets the actuator state.
       */
      inline void SetState(BaseActuatorState& s_state){
         m_psState = &s_state;
      }

   protected:

      /**
       * Pointer to the actuator state.
       */
      BaseActuatorState* m_psState;

   };

}
#endif

/**
 * @file <argos3/plugins/robots/e-puck/real_robot/real_epuck_serial_actuator.h>
 *
 * @brief This file provides the interface for the epuck uart communication.
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#ifndef REAL_EPUCK_SERIAL_ACTUATOR_H
#define REAL_EPUCK_SERIAL_ACTUATOR_H

namespace argos {
   class CRealEPuckSerialActuator;
}

#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_base.h>

namespace argos {

   class CRealEPuckSerialActuator {

   public:

      virtual ~CRealEPuckSerialActuator() {}

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

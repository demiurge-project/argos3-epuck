/**
 * @file argos3/plugins/robots/e-puck/real_robots/real_epuck_wheels_actuator.cpp
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#include "real_epuck_wheels_actuator.h"
#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

   static const Real SPEED_FACTOR = 1.29f / 0.0147f * (5.0f / 5.7f);

   /****************************************/
   /****************************************/

   void CRealEPuckWheelsActuator::SetLinearVelocity(Real f_left_velocity,
                                                    Real f_right_velocity) {
      m_psState->LWheel = (SInt16)(f_left_velocity * SPEED_FACTOR);
      m_psState->RWheel = (SInt16)(f_right_velocity * SPEED_FACTOR);
   }

   /****************************************/
   /****************************************/

}

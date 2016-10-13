/**
 * @file <argos3/plugins/robots/e-puck/real_robot/real_epuck_ircom_actuator.cpp
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#include "real_epuck_ircom_actuator.h"

namespace argos {

   /****************************************/
   /****************************************/

   void CRealEPuckIRComActuator::SetData(UInt8 un_data) {
      m_psState->IRComEnabled = 1;
      m_psState->IRComMessageData = un_data;
   }

   /****************************************/
   /****************************************/

   void CRealEPuckIRComActuator::SetEnabled(bool b_enabled) {
      m_psState->IRComEnabled = b_enabled ? 1 : 0;
   }

   /****************************************/
   /****************************************/
}

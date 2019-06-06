/**
 * @file <argos3/plugins/robots/x-puck/real_robot/real_xpuck_ircom_actuator.cpp
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#include "real_xpuck_ircom_actuator.h"

namespace argos {

   /****************************************/
   /****************************************/

   void CRealXPuckIRComActuator::SetData(UInt8 un_data) {
      m_psState->IRComEnabled = 1;
      m_psState->IRComMessageData = un_data;
   }

   /****************************************/
   /****************************************/

   void CRealXPuckIRComActuator::SetEnabled(bool b_enabled) {
      m_psState->IRComEnabled = b_enabled ? 1 : 0;
   }

   /****************************************/
   /****************************************/
}

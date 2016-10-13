/**
 * @file <argos3/plugins/robots/e-puck/real_robot/real_epuck_ircom_sensor.cpp
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */
#include "real_epuck_ircom_sensor.h"

namespace argos {

   /****************************************/
   /****************************************/

   void CRealEPuckIRComSensor::UpdateValues() {
      if (m_psState->IRComHasReceived) {
         SPacket* psPacket = new SPacket();
         psPacket->Data    = m_psState->IRComMessage.Data;
         psPacket->Sensor  = m_psState->IRComMessage.Sensor;
         psPacket->Bearing.SetValue(m_psState->IRComMessage.Bearing);
         m_tPackets.push_back(psPacket);
      }
   }

   /****************************************/
   /****************************************/
}

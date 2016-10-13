/**
 * @file <argos3/plugins/robots/e-puck/simulator/epuck_ircom_default_actuator.cpp>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#include "epuck_ircom_default_actuator.h"
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

   /****************************************/
   /****************************************/

   void CEPuckIRComDefaultActuator::SetRobot(CComposableEntity& c_entity) {
      m_pcRangeAndBearingEquippedEntity = &c_entity.GetComponent<CEpuckRABEquippedEntity>("rab[rab_1]");
      m_pcRangeAndBearingEquippedEntity->Enable();
      m_cData.Resize(m_pcRangeAndBearingEquippedEntity->GetMsgSize());
   }

   /****************************************/
   /****************************************/

   void CEPuckIRComDefaultActuator::Init(TConfigurationNode& t_tree) {
      CCI_EPuckIRComActuator::Init(t_tree);
      Real fRange=0.05f;
      GetNodeAttributeOrDefault(t_tree, "range", fRange,fRange);
      m_pcRangeAndBearingEquippedEntity->SetRange(fRange);
      m_cData.Resize(1);
   }

   /****************************************/
   /****************************************/

   void CEPuckIRComDefaultActuator::Update() {
      m_pcRangeAndBearingEquippedEntity->SetData(m_cData);
   }

   /****************************************/
   /****************************************/

   void CEPuckIRComDefaultActuator::Reset() {
      m_cData.Zero();
   }

   /****************************************/
   /****************************************/

   void CEPuckIRComDefaultActuator::SetData(UInt8 un_data) {
     m_cData[0] = un_data;
   }

   /****************************************/
   /****************************************/

   void CEPuckIRComDefaultActuator::SetEnabled(bool b_enabled) {

   }

   /****************************************/
   /****************************************/

   REGISTER_ACTUATOR(CEPuckIRComDefaultActuator,
                     "epuck_ircom", "medium",
                     "Lorenzo Garattoni [lgaratto@ulb.ac.be]",
                     "1.0",
                     "The IRCom actuator of the e-puck.",
                     "This actuator allows epucks to perform situated communication, i.e., a form of\n"
                     "wireless communication whereby the receiver also knows the location of the\n"
                     "sender with respect to its own frame of reference.\n"
                     "This actuator allows a epucks to send messages. To receive messages, you need\n"
                     "the IRCom sensor.\n"
                     "For usage, refer to [ci_epuck_ircom_actuator.h]\n\n"
                     "REQUIRED XML CONFIGURATION\n\n"
                     "   <controllers>\n"
                     "      ...\n"
                     "      <my_controller>\n"
                     "         ...\n"
                     "         <actuators>\n"
                     "            ...\n"
                     "            <ircom implementation=\"medium\" medium=\"ircom\"/>\n"
                     "            ...\n"
                     "         <actuators/>\n"
                     "         ...\n"
                     "      <my_controller/>\n"
                     "      ...\n"
                     "   <controllers>\n\n"
                     "The attribute [medium] refers to the [id] of the [ircom] medium defined\n"
                     "in the [media] xml section\n\n"
                     "   <media>\n"
                     "      ...\n"
                     "      <range_and_bearing id=\"ircom\"/>\n"
                     "      ...\n"
                     "   </media>"
                     , "Usable");

}

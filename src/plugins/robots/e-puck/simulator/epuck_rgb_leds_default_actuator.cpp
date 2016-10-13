/**
 * @file <argos3/plugins/robots/e-puck/simulator/epuck_rgb_leds_default_actuator.cpp>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#include "epuck_rgb_leds_default_actuator.h"
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/plugins/simulator/media/led_medium.h>

namespace argos {

   /****************************************/
   /****************************************/

   CEPuckRGBLEDsDefaultActuator::CEPuckRGBLEDsDefaultActuator() :
      m_pcLEDEquippedEntity(NULL) {
   }

   /****************************************/
   /****************************************/

   CEPuckRGBLEDsDefaultActuator::~CEPuckRGBLEDsDefaultActuator() {}

   /****************************************/
   /****************************************/

   void CEPuckRGBLEDsDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         CCI_EPuckRGBLEDsActuator::Init(t_tree);
         std::string strMedium;
         GetNodeAttribute(t_tree, "medium", strMedium);
         m_pcLEDMedium = &CSimulator::GetInstance().GetMedium<CLEDMedium>(strMedium);
         m_pcLEDEquippedEntity->AddToMedium(*m_pcLEDMedium);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing the e-puck RGB LEDs default actuator", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CEPuckRGBLEDsDefaultActuator::SetRobot(CComposableEntity& c_entity) {
      m_pcLEDEquippedEntity = &(c_entity.GetComponent<CLEDEquippedEntity>("leds"));
      m_pcLEDEquippedEntity->Enable();
   }

   /****************************************/
   /****************************************/

   void CEPuckRGBLEDsDefaultActuator::Update() {
      m_pcLEDEquippedEntity->SetLEDColor(8,  m_tLEDSettings[0]);
      m_pcLEDEquippedEntity->SetLEDColor(9,  m_tLEDSettings[1]);
      m_pcLEDEquippedEntity->SetLEDColor(10, m_tLEDSettings[2]);
   }

   /****************************************/
   /****************************************/

   void CEPuckRGBLEDsDefaultActuator::Reset() {
      m_tLEDSettings[0] = CColor::BLACK;
      m_tLEDSettings[1] = CColor::BLACK;
      m_tLEDSettings[2] = CColor::BLACK;
   }

   /****************************************/
   /****************************************/

   REGISTER_ACTUATOR(CEPuckRGBLEDsDefaultActuator,
                     "epuck_rgb_leds", "default",
                     "Lorenzo Garattoni [lgaratto@ulb.ac.be]",
                     "1.0",
                     "The epuck rgb leds actuator.",
                     "This actuator controls a group of RGB LEDs.\n"
                     "For usage, refer to [ci_epuck_rgb_leds_actuator.h]\n\n"
                     "REQUIRED XML CONFIGURATION\n\n"
                     "   <controllers>\n"
                     "      ...\n"
                     "      <my_controller>\n"
                     "         ...\n"
                     "         <actuators>\n"
                     "            ...\n"
                     "            <epuck_rgb_leds implementation=\"default\" medium=\"leds\"/>\n"
                     "            ...\n"
                     "         <actuators/>\n"
                     "         ...\n"
                     "      <my_controller/>\n"
                     "      ...\n"
                     "   <controllers>\n\n"
                     "The attribute [medium] refers to the [id] of the [led] medium defined\n"
                     "in the [media] xml section\n\n"
                     "   <media>\n"
                     "      ...\n"
                     "      <led id=\"leds\" grid_size=\"1,1,1\"/>\n"
                     "      ...\n"
                     "   </media>\n\n"
                     "OPTIONAL XML CONFIGURATION\n\n"
                     "None.\n",
                     "Usable");
}

/**
 * @file <argos3/plugins/robots/x-puck/simulator/xpuck_base_leds_default_actuator.cpp>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#include "xpuck_base_leds_default_actuator.h"

#include <argos3/core/simulator/simulator.h>
#include <argos3/plugins/simulator/media/led_medium.h>

namespace argos {

   /****************************************/
   /****************************************/

   CXPuckBaseLEDsDefaultActuator::CXPuckBaseLEDsDefaultActuator() :
      m_pcLEDEquippedEntity(NULL) {
   }

   /****************************************/
   /****************************************/

   CXPuckBaseLEDsDefaultActuator::~CXPuckBaseLEDsDefaultActuator() {
      //*m_pcLEDMedium
      // m_pcLEDEquippedEntity->RemoveFromMedium();
   }

   /****************************************/
   /****************************************/

   void CXPuckBaseLEDsDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         CCI_XPuckBaseLEDsActuator::Init(t_tree);
         std::string strMedium;
         GetNodeAttribute(t_tree, "medium", strMedium);
         m_pcLEDMedium = &CSimulator::GetInstance().GetMedium<CLEDMedium>(strMedium);
         m_pcLEDEquippedEntity->AddToMedium(*m_pcLEDMedium);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing the x-puck base LEDs default actuator", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CXPuckBaseLEDsDefaultActuator::SetRobot(CComposableEntity& c_entity) {
      m_pcLEDEquippedEntity = &(c_entity.GetComponent<CLEDEquippedEntity>("leds"));
      m_pcLEDEquippedEntity->Enable();
   }

   /****************************************/
   /****************************************/

   void CXPuckBaseLEDsDefaultActuator::Update() {
      size_t pos = 0;
      for (size_t i = 0; i < 8; i++) {
         pos = 7 - i;
         if((1 << i) & m_unLEDSettings) {
            /* The control interface set the leds non clockwise, the LED entity does, simple conversion */
            m_pcLEDEquippedEntity->SetLEDColor(pos, CColor::RED);
         }
         else {
            m_pcLEDEquippedEntity->SetLEDColor(pos, CColor::BLACK);
         }
      }
   }

   /****************************************/
   /****************************************/

   void CXPuckBaseLEDsDefaultActuator::Reset() {
      m_pcLEDEquippedEntity->SetLEDColor(0, CColor::BLACK);
      m_pcLEDEquippedEntity->SetLEDColor(1, CColor::BLACK);
      m_pcLEDEquippedEntity->SetLEDColor(2, CColor::BLACK);
      m_pcLEDEquippedEntity->SetLEDColor(3, CColor::BLACK);
      m_pcLEDEquippedEntity->SetLEDColor(4, CColor::BLACK);
      m_pcLEDEquippedEntity->SetLEDColor(5, CColor::BLACK);
      m_pcLEDEquippedEntity->SetLEDColor(6, CColor::BLACK);
      m_pcLEDEquippedEntity->SetLEDColor(7, CColor::BLACK);
   }

   /****************************************/
   /****************************************/

}

REGISTER_ACTUATOR(CXPuckBaseLEDsDefaultActuator,
                  "xpuck_base_leds", "default",
                  "Lorenzo Garattoni [lgaratto@ulb.ac.be]",
                  "1.0",
                  "The xpuck base LEDs actuator.",
                  "This actuator controls a group of LEDs on the x-puck base body.\n"
                  "For usage, refer to [ci_xpuck_base_leds_actuator.h]\n\n"
                  "REQUIRED XML CONFIGURATION\n\n"
                  "   <controllers>\n"
                  "      ...\n"
                  "      <my_controller>\n"
                  "         ...\n"
                  "         <actuators>\n"
                  "            ...\n"
                  "            <xpuck_base_leds implementation=\"default\" medium=\"leds\"/>\n"
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
                  "Usable"
   );

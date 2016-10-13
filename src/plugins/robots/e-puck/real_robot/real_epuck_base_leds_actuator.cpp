/**
 * @file <argos3/plugins/robots/e-puck/real_robot/real_epuck_base_leds_actuator.cpp>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#include "real_epuck_base_leds_actuator.h"
#include <argos3/core/utility/logging/argos_log.h>
#include <algorithm>

namespace argos {

   /****************************************/
   /****************************************/

   CRealEPuckBaseLEDsActuator::~CRealEPuckBaseLEDsActuator() {
      SwitchLEDs(false);
   }

   /****************************************/
   /****************************************/

   void CRealEPuckBaseLEDsActuator::SwitchLED(size_t un_led_number,
                                              bool b_switched_on) {
       un_led_number = (8 - un_led_number)%8;
      if(b_switched_on) {
         m_psState->BaseLEDs |= (1 << un_led_number);
      } else {
         m_psState->BaseLEDs &= ~(1 << un_led_number);
      }
   }

   /****************************************/
   /****************************************/

   void CRealEPuckBaseLEDsActuator::SwitchLEDs(bool b_switched_on) {
      if(b_switched_on) {
         m_psState->BaseLEDs = 255;
      } else {
         m_psState->BaseLEDs = 0;
      }
   }

   /****************************************/
   /****************************************/

}

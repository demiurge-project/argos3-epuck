/**
 * @file <argos3/plugins/robots/x-puck/real_robot/real_xpuck_base_leds_actuator.cpp>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#include "real_xpuck_base_leds_actuator.h"
#include <argos3/core/utility/logging/argos_log.h>
#include <algorithm>

namespace argos {

   /****************************************/
   /****************************************/

   CRealXPuckBaseLEDsActuator::~CRealXPuckBaseLEDsActuator() {
      SwitchLEDs(false);
   }

   /****************************************/
   /****************************************/

   void CRealXPuckBaseLEDsActuator::SwitchLED(size_t un_led_number,
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

   void CRealXPuckBaseLEDsActuator::SwitchLEDs(bool b_switched_on) {
      if(b_switched_on) {
         m_psState->BaseLEDs = 255;
      } else {
         m_psState->BaseLEDs = 0;
      }
   }

   /****************************************/
   /****************************************/

}

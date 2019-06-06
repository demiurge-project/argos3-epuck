/**
 * @file <argos3/plugins/robots/x-puck/real_robot/real_xpuck_base_leds_actuator.h>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 *
 */

#ifndef REAL_XPUCK_BASE_LEDS_ACTUATOR_H
#define REAL_XPUCK_BASE_LEDS_ACTUATOR_H

namespace argos {
   class CRealXPuckBaseLEDsActuator;
}

#include <argos3/plugins/robots/x-puck/real_robot/real_xpuck_serial_actuator.h>
#include <argos3/plugins/robots/x-puck/control_interface/ci_xpuck_base_leds_actuator.h>
#include <argos3/plugins/robots/x-puck/real_robot/real_xpuck_base.h>

namespace argos {

   class CRealXPuckBaseLEDsActuator : virtual public CCI_XPuckBaseLEDsActuator,
                                      virtual public CRealXPuckSerialActuator {

   public:

      CRealXPuckBaseLEDsActuator() {
      }

      virtual ~CRealXPuckBaseLEDsActuator();

      virtual void SwitchLED(size_t un_led_number,
                             bool b_switched_on);

      virtual void SwitchLEDs(bool b_switched_on);

   };
}
#endif

/**
 * @file <argos3/plugins/robots/e-puck/real_robot/real_epuck_base_leds_actuator.h>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 *
 */

#ifndef REAL_EPUCK_BASE_LEDS_ACTUATOR_H
#define REAL_EPUCK_BASE_LEDS_ACTUATOR_H

namespace argos {
   class CRealEPuckBaseLEDsActuator;
}

#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_serial_actuator.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_base_leds_actuator.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_base.h>

namespace argos {

   class CRealEPuckBaseLEDsActuator : virtual public CCI_EPuckBaseLEDsActuator,
                                      virtual public CRealEPuckSerialActuator {

   public:

      CRealEPuckBaseLEDsActuator() {
      }

      virtual ~CRealEPuckBaseLEDsActuator();

      virtual void SwitchLED(size_t un_led_number,
                             bool b_switched_on);

      virtual void SwitchLEDs(bool b_switched_on);

   };
}
#endif

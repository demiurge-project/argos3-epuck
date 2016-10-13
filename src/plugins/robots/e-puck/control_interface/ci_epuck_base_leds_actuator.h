/**
 * @file <argos3/plugins/robots/e-puck/control_interface/ci_epuck_base_leds_actuator.h>
 *
 * @brief This file provides the definition of the e-puck base LEDs actuator.
 *
 * This file provides the definition of the e-puck base LEDs actuator.
 * These LEDs are not RGB - they can only be switched on or off. When on, the
 * LEDs are lit up in red.
 * The LEDs are distributed on the base of the robot as follows:
 *
 * (View from the top)
 *
 *        front
 *        0   7    r
 *   l 1         6 i
 *   e             g
 *   f 2         5 h
 *   t    3   4    t
 *        back
 *
 * @author Lorenzo Garattoni- <lgaratto@ulb.ac.be>
 */

#ifndef CCI_EPUCK_BASE_LEDS_ACTUATOR_H
#define CCI_EPUCK_BASE_LEDS_ACTUATOR_H

namespace argos {
   class CCI_EPuckBaseLEDsActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>
#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

   class CCI_EPuckBaseLEDsActuator : public CCI_Actuator {

   public:

      CCI_EPuckBaseLEDsActuator();
      ~CCI_EPuckBaseLEDsActuator() {}

      /**
       * Switches a single LED on/off.
       * @param un_led_number The number of the wanted LED [0-7].
       * @param b_on <tt>true</tt> to switch the LED on; <tt>false</tt> to switch it off.
       */
      virtual void SwitchLED(size_t un_led_number,
                             bool b_on);

      /**
       * Switches all LEDs on/off.
       * @param b_on <tt>true</tt> to switch the LEDs on; <tt>false</tt> to switch it off.
       */
      virtual void SwitchLEDs(bool b_on);

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
#endif

   protected:

      /**
       * State of the LED (8 bits, one for each LED, 0 for off, 1 for on)
       */
      UInt8 m_unLEDSettings;
   };

}
#endif

/**
 * @file <argos3/plugins/robot/x-puck/real_robot/real_xpuck_rgb_leds_actuator.h>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#ifndef REAL_XPUCK_RGB_LEDS_ACTUATOR_H
#define REAL_XPUCK_RGB_LEDS_ACTUATOR_H

namespace argos {
   class CRealXPuckRGBLEDsActuator;
}

#include <argos3/plugins/robots/x-puck/control_interface/ci_xpuck_rgb_leds_actuator.h>
#include <argos3/plugins/robots/x-puck/real_robot/real_xpuck_i2c_actuator.h>

namespace argos {

   class CRealXPuckRGBLEDsActuator : virtual public CCI_XPuckRGBLEDsActuator,
                                     virtual public CRealXPuckI2CActuator {

   public:

      CRealXPuckRGBLEDsActuator();
      virtual ~CRealXPuckRGBLEDsActuator();

      virtual void SendData();

   private:

      TI2CDeviceStream m_tDeviceStream;

   };
}
#endif

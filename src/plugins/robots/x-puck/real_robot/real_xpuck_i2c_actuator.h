/**
 * @file <argos3/plugins/robots/x-puck/real_robot/real_xpuck_i2c_actuator.h>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 * @author Carlo Pinciroli <cpinciro@ulb.ac.be>
 */

#ifndef REAL_XPUCK_I2C_ACTUATOR_H
#define REAL_XPUCK_I2C_ACTUATOR_H

namespace argos {
   class CRealXPuckI2CActuator;
}

#include <argos3/plugins/robots/x-puck/real_robot/real_xpuck_i2c_device.h>

namespace argos {

   class CRealXPuckI2CActuator : public CRealXPuckI2CDevice {

   public:

      virtual ~CRealXPuckI2CActuator() {}

      virtual void SendData() = 0;

   };

}
#endif

/**
 * @file <argos3/plugins/robots/e-puck/real_robot/real_epuck_i2c_actuator.h>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 * @author Carlo Pinciroli <cpinciro@ulb.ac.be>
 */

#ifndef REAL_EPUCK_I2C_ACTUATOR_H
#define REAL_EPUCK_I2C_ACTUATOR_H

namespace argos {
   class CRealEPuckI2CActuator;
}

#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_i2c_device.h>

namespace argos {

   class CRealEPuckI2CActuator : public CRealEPuckI2CDevice {

   public:

      virtual ~CRealEPuckI2CActuator() {}

      virtual void SendData() = 0;

   };

}
#endif

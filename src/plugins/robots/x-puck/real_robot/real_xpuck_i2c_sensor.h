/**
 * @file <argos3/plugins/robots/x-puck/real_robot/real_xpuck_i2c_sensor.h>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 * @author Carlo Pinciroli <cpinciro@ulb.ac.be>
 */

#ifndef REAL_XPUCK_I2C_SENSOR_H
#define REAL_XPUCK_I2C_SENSOR_H

namespace argos {
   class CRealXPuckI2CSensor;
}

#include <argos3/plugins/robots/x-puck/real_robot/real_xpuck_i2c_device.h>

namespace argos {

   class CRealXPuckI2CSensor : public CRealXPuckI2CDevice {

   public:

      virtual ~CRealXPuckI2CSensor() {}

      virtual void ReceiveData() = 0;

   };

}
#endif

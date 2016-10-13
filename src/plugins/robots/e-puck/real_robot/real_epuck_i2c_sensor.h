/**
 * @file <argos3/plugins/robots/e-puck/real_robot/real_epuck_i2c_sensor.h>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 * @author Carlo Pinciroli <cpinciro@ulb.ac.be>
 */

#ifndef REAL_EPUCK_I2C_SENSOR_H
#define REAL_EPUCK_I2C_SENSOR_H

namespace argos {
   class CRealEPuckI2CSensor;
}

#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_i2c_device.h>

namespace argos {

   class CRealEPuckI2CSensor : public CRealEPuckI2CDevice {

   public:

      virtual ~CRealEPuckI2CSensor() {}

      virtual void ReceiveData() = 0;

   };

}
#endif

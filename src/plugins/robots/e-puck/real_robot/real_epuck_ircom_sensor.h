/**
 * @file <argos3/plugins/robots/e-puck/real_robot/real_epuck_ircom_sensor.h>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#ifndef REAL_EPUCK_IRCOM_SENSOR_H
#define REAL_EPUCK_IRCOM_SENSOR_H

namespace argos {
   class CRealEPuckIRComSensor;
}

#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_ircom_sensor.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_serial_sensor.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_base.h>

namespace argos {

   class CRealEPuckIRComSensor : virtual public CCI_EPuckIRComSensor,
                                 virtual public CRealEPuckSerialSensor {

   public:

      CRealEPuckIRComSensor() {};
      virtual ~CRealEPuckIRComSensor() {};

      virtual void UpdateValues();

   };
}
#endif

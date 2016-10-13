/**
 * @file <argos3/plugins/robots/e-puck/real_epuck_accelerometer_sensor.h>
 *
 * @brief This file provides the implementation of the epuck accelerometer sensor.
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 *
 */

#ifndef REAL_EPUCK_ACCELEROMETER_SENSOR_H
#define REAL_EPUCK_ACCELEROMETER_SENSOR_H

/* To avoid dependency problems when including */
namespace argos {
   class CRealEPuckAccelerometerSensor;
}

#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_accelerometer_sensor.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_base.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_serial_sensor.h>

namespace argos {

   class CRealEPuckAccelerometerSensor : virtual public CCI_EPuckAccelerometerSensor,
      virtual public CRealEPuckSerialSensor {

     public:

         CRealEPuckAccelerometerSensor() {};
         virtual ~CRealEPuckAccelerometerSensor() {};

         virtual void Init(TConfigurationNode& t_node) {};
         virtual void UpdateValues();
      };
}

#endif

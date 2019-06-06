/**
 * @file <argos3/plugins/robots/x-puck/real_xpuck_accelerometer_sensor.h>
 *
 * @brief This file provides the implementation of the xpuck accelerometer sensor.
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 *
 */

#ifndef REAL_XPUCK_ACCELEROMETER_SENSOR_H
#define REAL_XPUCK_ACCELEROMETER_SENSOR_H

/* To avoid dependency problems when including */
namespace argos {
   class CRealXPuckAccelerometerSensor;
}

#include <argos3/plugins/robots/x-puck/control_interface/ci_xpuck_accelerometer_sensor.h>
#include <argos3/plugins/robots/x-puck/real_robot/real_xpuck_base.h>
#include <argos3/plugins/robots/x-puck/real_robot/real_xpuck_serial_sensor.h>

namespace argos {

   class CRealXPuckAccelerometerSensor : virtual public CCI_XPuckAccelerometerSensor,
      virtual public CRealXPuckSerialSensor {

     public:

         CRealXPuckAccelerometerSensor() {};
         virtual ~CRealXPuckAccelerometerSensor() {};

         virtual void Init(TConfigurationNode& t_node) {};
         virtual void UpdateValues();
      };
}

#endif

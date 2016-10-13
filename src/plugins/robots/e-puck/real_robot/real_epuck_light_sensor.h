/**
 * @file <argos3/plugins/robots/e-puck/real_robot/real_epuck_light_sensor.h>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 * @author Matteo Amaducci <matteo.amaducci2@studio.unibo.it>
 */

#ifndef REAL_EPUCK_LIGHT_SENSOR_H
#define REAL_EPUCK_LIGHT_SENSOR_H

namespace argos {
   class CRealEPuckLightSensor;
}

#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_light_sensor.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_serial_sensor.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_base.h>
#include <argos3/core/utility/math/angles.h>
#include <argos3/core/utility/math/range.h>
#include <vector>

namespace argos {

   class CRealEPuckLightSensor: virtual public CCI_EPuckLightSensor,
                                virtual public CRealEPuckSerialSensor {

   public:

      CRealEPuckLightSensor();
      virtual ~CRealEPuckLightSensor() {}

      virtual void Init(TConfigurationNode& t_node);
      virtual void UpdateValues();

      /**
       * Calibrates the sensor.
       */
      virtual void Calibrate();

   private:

      /**
       * Path of the low level configuration file
       */
      std::string m_sLowLevelConfigurationFile;

      /**
       * <tt>true</tt> when the sensor is calibrated
       */
      bool m_bCalibrated;

      /**
       * Contains all the ranges
       */
      std::vector<CRange<Real> > m_vecRanges;

      /**
       * Contains the state of the sensors: each item is either
       * true if it's disabled, either false if not.
       */
      std::vector<bool> m_vecDisabledSensors;
   };
}
#endif

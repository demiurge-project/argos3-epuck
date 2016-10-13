/**
 * @file <argos3/plugins/robots/e-puck/real_robot/real_epuck_proximity_sensor.h>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#ifndef REAL_EPUCK_PROXIMITY_SENSOR_H
#define REAL_EPUCK_PROXIMITY_SENSOR_H

namespace argos {
   class CRealEPuckProximitySensor;
}

#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_proximity_sensor.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_serial_sensor.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_base.h>
#include <argos3/core/utility/math/angles.h>
#include <argos3/core/utility/math/range.h>

namespace argos {

   class CRealEPuckProximitySensor : virtual public CCI_EPuckProximitySensor,
                                     virtual public CRealEPuckSerialSensor {

   public:

      CRealEPuckProximitySensor();
      virtual ~CRealEPuckProximitySensor() {}

      virtual void Init(TConfigurationNode& t_node);
      virtual void UpdateValues();

      /**
       * Calibrates the sensor.
       */
      void Calibrate(const std::string& str_conf_file);

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
       * Store of the values of sensors
       */
      std::vector<CRange<Real> > m_vecRanges;

      /**
       * Store the id's of disabled sensor (broken, and so on)
       */
      std::vector<bool>m_vecDisabledSensors;
   };
}
#endif

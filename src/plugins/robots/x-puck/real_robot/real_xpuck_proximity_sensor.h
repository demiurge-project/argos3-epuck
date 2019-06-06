/**
 * @file <argos3/plugins/robots/x-puck/real_robot/real_xpuck_proximity_sensor.h>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#ifndef REAL_XPUCK_PROXIMITY_SENSOR_H
#define REAL_XPUCK_PROXIMITY_SENSOR_H

namespace argos {
   class CRealXPuckProximitySensor;
}

#include <argos3/plugins/robots/x-puck/control_interface/ci_xpuck_proximity_sensor.h>
#include <argos3/plugins/robots/x-puck/real_robot/real_xpuck_serial_sensor.h>
#include <argos3/plugins/robots/x-puck/real_robot/real_xpuck_base.h>
#include <argos3/core/utility/math/angles.h>
#include <argos3/core/utility/math/range.h>

namespace argos {

   class CRealXPuckProximitySensor : virtual public CCI_XPuckProximitySensor,
                                     virtual public CRealXPuckSerialSensor {

   public:

      CRealXPuckProximitySensor();
      virtual ~CRealXPuckProximitySensor() {}

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

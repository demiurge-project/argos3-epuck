/**
 * @file <argos3/plugins/robots/x-puck/real_robot/real_xpuck_micro_sensor.h>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 * @author Matteo Amaducci <matteo.amaducci2@studio.unibo.it>
 */

#ifndef REAL_XPUCK_MICRO_SENSOR_H
#define REAL_XPUCK_MICRO_SENSOR_H

namespace argos {
   class CRealXPuckMicroSensor;
}

#include <argos3/plugins/robots/x-puck/real_robot/real_xpuck_base.h>
#include <argos3/plugins/robots/x-puck/real_robot/real_xpuck_serial_sensor.h>
#include <argos3/plugins/robots/x-puck/control_interface/ci_xpuck_micro_sensor.h>
#include <argos3/core/utility/math/range.h>

namespace argos {

   class CRealXPuckMicroSensor : virtual public CCI_XPuckMicroSensor,
                                 virtual public CRealXPuckSerialSensor {

   public:

      CRealXPuckMicroSensor();
      virtual ~CRealXPuckMicroSensor() {}

      virtual void Init(TConfigurationNode& t_node);
      virtual void UpdateValues();

      /**
       * Calibrates the sensor.
       */
      void Calibrate();

   private:

      /**
       * Path to the low level config file.
       */
      std::string m_sLowLevelConfigurationFile;

      /**
       * Flag to check if the sensor is calibrated or not.
       */
      bool m_bCalibrated;

      /**
       * Ranges of the individual sensors.
       */
      std::vector<CRange<Real> > m_vecRanges;

      /**
       * Disabled sensors.
       */
      std::vector<bool> m_vecDisabledSensors;
   };
}
#endif

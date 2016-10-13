/**
 * @file <argos3/plugins/robots/e-puck/real_robot/real_epuck_ground_sensor.h>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#ifndef REAL_EPUCK_GROUND_SENSOR_H
#define REAL_EPUCK_GROUND_SENSOR_H

namespace argos {
   class CRealEPuckGroundSensor;
}

#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_ground_sensor.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_i2c_sensor.h>
#include <argos3/core/utility/math/range.h>

namespace argos {

   class CRealEPuckGroundSensor : virtual public CCI_EPuckGroundSensor,
                                  virtual public CRealEPuckI2CSensor {

   public:

      /**
       * Constructor
       */
      CRealEPuckGroundSensor();

      /**
       * Destructor
       */
      ~CRealEPuckGroundSensor();

      /**
       * Initialize the sensor
       *
       * @param t_node configuration node of the ground
       * sensor found in the xml configuration file
       */
      virtual void Init(TConfigurationNode& t_node);

      /**
       * Read the datas of the sensor
       */
      virtual void ReceiveData();

      /**
       * Calibrate the sensor regarding the low level config
       */
      virtual void Calibrate();

   private:

      /** The device stream to interact with the ground sensor */
      TI2CDeviceStream m_tDeviceStream;

      /** Path of the low level configuration file */
      std::string m_sLowLevelConfigurationFile;

      /** Flag to check if the sensor is calibrated */
      bool m_bCalibrated;

      /** Range values for sensor calibration */
      std::vector< CRange<Real> > m_vecRanges;

      /** Contains the disabled sensors (e.g., broken sensors) */
      std::vector< bool > m_vecDisabledSensors;

      /** Buffer to store the raw data received from the device stream */
      UInt8 m_unRawData[6];
   };

}
#endif

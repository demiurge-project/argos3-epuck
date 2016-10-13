/**
 * @brief This file provides the control interface of the e-puck accelerometer sensor.
 *
 * @file <argos3/plugins/robots/e-puck/control_interface/ci_epuck_accelerometer_sensor.h
 *
 * @author Lorenzo Garattoni- <lgaratto@ulb.ac.be>
 */
#ifndef CCI_EPUCK_ACCELEROMETER_SENSOR_H
#define CCI_EPUCK_ACCELEROMETER_SENSOR_H

namespace argos {
   class CCI_EPuckAccelerometerSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>

namespace argos {

   class CCI_EPuckAccelerometerSensor : virtual public CCI_Sensor {

   public:

      /**
       * Constructor
       */
      CCI_EPuckAccelerometerSensor() {
         THROW_ARGOSEXCEPTION("The e-puck accelerometer sensor is not implemented");
      }

      /**
       * Destructor
       */
      virtual ~CCI_EPuckAccelerometerSensor() {}

      /**
       * Struct representing the values of the sensor
       */
      struct SReading {
         /**
          * OX inclination on cartesian plan
          */
         Real X;
         /**
          * OY inclination on cartesian plan
          */
         Real Y;
         /**
          * OZ inclination on cartesian plan
          */
         Real Z;

         /**
          * Constructor of an empty struct
          */
         SReading() : X(0.0f), Y(0.0f), Z(0.0f) {}

         /**
          * Constructor of a filled struct
          *
          * @param un_x the OX inclination
          *
          * @param un_y the OY inclination
          *
          * @param un_z the OZ inclination
          */
         SReading(Real un_x, Real un_y, Real un_z) :
            X(un_x),
            Y(un_y),
            Z(un_z) {}
      };

      /**
       * Get values read by the sensor
       *
       * @See SReading
       */
      inline virtual const SReading& GetReading() const {
         return m_sReading;
      }

#ifdef ARGOS_WITH_LUA
      /**
       *
       */
      virtual void CreateLuaState(lua_State* pt_lua_state) {}
#endif

   protected:

      /**
       * Values read by the sensor
       */
      SReading m_sReading;

   };

   /**
    * Redefine the operator "<<" for readings
    */
   std::ostream& operator<<(std::ostream& c_os,
                            const CCI_EPuckAccelerometerSensor::SReading& s_reading);
}
#endif

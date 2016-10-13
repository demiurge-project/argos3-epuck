/**
 * @file <argos3/plugins/robots/e-puck/control_interface/ci_epuck_light_sensor.h>
 *
 * @brief This file provides the implementation of the epuck light sensor.
 *
 * This file provides the definition of the epuck light sensor.
 * The sensors are evenly spaced on a ring around the base of the ropuck.
 * Therefore, they do not turn with the turret. The readings are in the
 * following order (seeing the e-puck from TOP, battery socket is the BACK):
 *
 *              front
 *
 *              0   7
 *     l     1         6    r
 *     e                    i
 *     f    2           5   g
 *     t                    h
 *             3     4      t
 *
 *              back
 *
 *
 * @author Lorenzo Garattoni- <lgaratto@ulb.ac.be>
 */

#ifndef CCI_EPUCK_LIGHT_SENSOR_H
#define CCI_EPUCK_LIGHT_SENSOR_H

namespace argos {
   class CCI_EPuckLightSensor;
}

#include <vector>
#include <argos3/core/utility/math/angles.h>
#include <argos3/core/control_interface/ci_sensor.h>

namespace argos {

   class CCI_EPuckLightSensor : virtual public CCI_Sensor {

     public:

      /**
       * Constructor
       */
      CCI_EPuckLightSensor();

      /**
       * Destructor
       */
      virtual ~CCI_EPuckLightSensor() {}

      /**
       * Single sensor reading
       */
      struct SReading {
         /**
          * Value of the reading
          */
         Real Value;

         /**
          * Bearing of the reading
          */
         CRadians Angle;

         /**
          * Empty constructor
          */
      SReading() :
         Value(0.0f) {}

      /**
       * Filled constructor
       *
       * @param f_value the value of the reading
       *
       * @param c_angle the angle of the reading
       */
      SReading(Real f_value,
               const CRadians& c_angle) :
         Value(f_value),
            Angle(c_angle) {}

      };

      /**
       * Sensor ring readings
       */
      typedef std::vector<SReading> TReadings;

      /**
       * Returns the sensor readings.
       * @return The sensor readings
       */
      inline const TReadings& GetReadings() const {
         return m_tReadings;
      }

      /**
       * Returns a single sensor reading.
       * @param i The id of the wanted sensor
       * @return The reading of the wanted sensor
       */
      inline const SReading& GetReading(UInt16 i) const {
         return m_tReadings[i];
      }

#ifdef ARGOS_WITH_LUA
      /**
       *
       * @param pt_lua_state
       */
      virtual void CreateLuaState(lua_State* pt_lua_state);

      /**
       *
       * @param pt_lua_state
       */
      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

     protected:

      /**
       * Store the readings
       */
      TReadings m_tReadings;

   };

   /**
    * Redefine the "<<" operator with one reading.
    */
   std::ostream& operator<<(std::ostream& c_os,
                            const CCI_EPuckLightSensor::SReading& s_reading);

   /**
    * Redefine the "<<" operator with readings.
    */
   std::ostream& operator<<(std::ostream& c_os,
                            const CCI_EPuckLightSensor::TReadings& t_readings);
}
#endif

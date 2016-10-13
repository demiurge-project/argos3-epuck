/**
 * @file <argos3/plugins/robots/e-puck/control_interface/epuck_ground_sensor.h
 *
 * @brief This file provides the interface of the e-puck ground sensor.
 *
 * The ground sensors are an additional part of the e-pucks.  They are
 * located evenly spaced on the front of the robot. They are 3 and are
 * useful for detecting holes or lines that are placed on the ground.
 *
 * This sensor provides access to the base sensors. The readings are
 * in the following order (seeing the robot from TOP):
 *
 *      front
 *
 *      0 1 2    r
 * l             i
 * e             g
 * f             h
 * t             t
 *
 *       back
 *
 * @author Lorenzo Garattoni- <lgaratto@ulb.ac.be>
 */

#ifndef CCI_EPUCK_GROUND_SENSOR_H
#define CCI_EPUCK_GROUND_SENSOR_H

namespace argos {
   class CCI_EPuckGroundSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>

namespace argos {

   class CCI_EPuckGroundSensor : public CCI_Sensor {

   public:

      /**
       * Destructor
       */
      virtual ~CCI_EPuckGroundSensor() {}

      /**
       * Sensor readings
       */
      struct SReadings {
         /**
          * Left reagind
          */
         Real Left;

         /**
          * Center reading
          */
         Real Center;

         /**
          * Right reading
          */
         Real Right;

         /**
          * Empty constructor
          */
         SReadings() :
            Left(0.0f),
            Center(0.0f),
            Right(0.0f) {}

         /**
          * Reset the reading (ie. all 0)
          */
         void Reset() {
            Left = 0.0f;
            Center = 0.0f;
            Right = 0.0f;
         }

         /**
          * Define a new operator [] to allow accessing to readings like an array
          *
          * @param un_idx id of the field to acces. 0 -> Left, 1 -> Center and 2 -> Right
          */
         Real& operator[] (size_t un_idx) {
            if(un_idx == 0) return Left;
            else if(un_idx == 1) return Center;
            else if(un_idx == 2) return Right;
            else THROW_ARGOSEXCEPTION("Index of out bound accessing the ground sensor reading. "
                                      << "Expected [0-2], got: " << un_idx);
         }

        Real const& operator[] (size_t un_idx) const {
            if(un_idx == 0) return Left;
            else if(un_idx == 1) return Center;
            else if(un_idx == 2) return Right;
            else THROW_ARGOSEXCEPTION("Index of out bound accessing the ground sensor reading. "
                                      << "Expected [0-2], got: " << un_idx);
         }
      };

      /**
       * Returns the sensor data as a SReadings.
       * @return the sensor data as a SReadings
       */
      inline const SReadings& GetReadings() const {
         return m_sReadings;
      }

      /**
       * Returns the sensor data as an array of 3 elements.
       * @return the sensor data as an array of 3 elements.
       */
      inline void GetReadings(Real* pf_buffer) const {
         pf_buffer[0] = m_sReadings.Left;
         pf_buffer[1] = m_sReadings.Center;
         pf_buffer[2] = m_sReadings.Right;
      }

      /**
       * Return a reading
       * @param un_idx the id of the sensor. 0 -> Left, 1 -> Center and 2 -> Right.
       * @return the sensor [i] reading
       */
      inline Real GetReading(size_t un_idx) {
         return m_sReadings[un_idx];
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
       * Contains the sensor readings
       */
      SReadings m_sReadings;
   };
}
#endif

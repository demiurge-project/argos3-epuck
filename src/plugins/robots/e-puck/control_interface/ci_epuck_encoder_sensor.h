/**
 * @file <argos3/plugins/robots/e-puck/control_interface/ci_epuck_encoder_sensor.h
 *
 * @brief This file provides the definition of the epuck encoder sensor.
 *
 * This file provides the definition of the epuck encoder sensor. It
 * returns the distance covered by the wheels in the last control
 * step.
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#ifndef CCI_EPUCK_ENCODER_SENSOR_H
#define CCI_EPUCK_ENCODER_SENSOR_H

namespace argos {
   class CCI_EPuckEncoderSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>

namespace argos {

   class CCI_EPuckEncoderSensor : public CCI_Sensor {

   public:

      static const Real WHEEL_AXIS_LENGTH;

   public:

      /**
       * Struct to store the datas read by the encoder sensor of the e-puck
       */
      struct SReading {
         /**
          * Distance covered by the left wheel
          */
         Real CoveredDistanceLeftWheel;

         /**
          * Distance covered by the right wheel
          */
         Real CoveredDistanceRightWheel;

         /**
          * Constructor of an empty SReading
          */
         SReading() :
            CoveredDistanceLeftWheel(0.0f),
            CoveredDistanceRightWheel(0.0f){}

         /**
          * Constructor of a filled SReading
          *
          * @param f_covered_distance_left_wheel
          *
          * @param f_covered_distance_right_wheel
          */
         SReading(Real f_covered_distance_left_wheel,
                  Real f_covered_distance_right_wheel) :
            CoveredDistanceLeftWheel(f_covered_distance_left_wheel),
            CoveredDistanceRightWheel(f_covered_distance_right_wheel){
         }

      };

      /**
       * Destructor
       */
      virtual ~CCI_EPuckEncoderSensor() {}

      /**
       * Returns the reading of the encoder sensor.
       *
       * @see SReading
       */
      virtual const SReading& GetReading() const {
         return m_sReading;
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
       * Stores data read from the encoder sensor.
       */
      SReading m_sReading;
   };
}
#endif

/**
 * @file common/control_interface/e-puck/ci_epuck_micro_sensor.h
 *
 * @brief This file provides the implementation of the epuck micro sensor.
 *
 * This file provides the definition of the epuck micro sensor.
 * The sensors are evenly spaced on the left, right and back to the base of the epuck.
 * The micros are placed as follow:
 *
 *              front
 *   l                      r
 *   e   MIC_L        MIC_R i
 *   f           .          g
 *   t                      h
 *              MIC_B       t
 *
 *              back
 *
 *
 * @author Lorenzo Garattoni- <lgaratto@ulb.ac.be>
 * @author Matteo Amaducci <matteo.amaducci2@studio.unibo.it>
 */
#ifndef CCI_EPUCK_MICRO_SENSOR_H
#define CCI_EPUCK_MICRO_SENSOR_H

namespace argos {
   class CCI_EPuckMicroSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>

namespace argos {

   class CCI_EPuckMicroSensor : virtual public CCI_Sensor {

     public:

      /**
       * Represent the data read from the sensor
       */
      struct SReading {
         /**
          * Reading of the right mic
          */
         Real Right;

         /**
          * Reading of the left mic
          */
         Real Left;

         /**
          * Reading of the back mic
          */
         Real Back;

      /**
       * Empty constructor
       */
      SReading() : Right(0), Left(0), Back(0) {}

      /**
       * Filled constructor
       *
       * @param un_right the right mic reading
       *
       * @param un_left the left mic reading
       *
       * @param un_back the back mic reading
       */
      SReading( Real un_right, Real un_left, Real un_back) :
         Right(un_right),
            Left(un_left),
            Back(un_back) {}
      };

      /**
       * Constructor
       */
      CCI_EPuckMicroSensor() {
         THROW_ARGOSEXCEPTION("The e-puck micro sensor is not implemented");
      }

      /**
       * Destructor
       */
      ~CCI_EPuckMicroSensor() {}

      /**
       * Returns the last micros reading (all 3 sensors)
       */
      inline const SReading& GetReading() const {
         return m_sReading;
      }

#ifdef ARGOS_WITH_LUA
      /**
       *
       * @param pt_lua_state
       */
      virtual void CreateLuaState(lua_State* pt_lua_state) {}
#endif

     protected:

      /**
       * The readings of the mics
       */
      SReading m_sReading;

   };

   /**
    * Redefine the "<<" opérator with readings
    */
   std::ostream& operator<<(std::ostream& c_os,
                            const CCI_EPuckMicroSensor::SReading& s_reading);
}
#endif

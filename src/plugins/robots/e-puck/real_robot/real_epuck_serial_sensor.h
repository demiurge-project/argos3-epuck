/**
 * @file <argos3/plugins/robots/e-puck/real_robot/real_epuck_serial_sensor.h>
 *
 * @brief This file provides the interface for the epuck i2c communication.
 *
 * This file provides the signature of the UpdateValues() function
 * that allows data transfer from the shared data structure
 * (BaseSensorState) and the specific private sensors structure.
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#ifndef REAL_EPUCK_SERIAL_SENSOR_H
#define REAL_EPUCK_SERIAL_SENSOR_H

namespace argos {
   class CRealEPuckSerialSensor;
}

#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_base.h>

namespace argos {

   class CRealEPuckSerialSensor {

   public:

      virtual ~CRealEPuckSerialSensor() {}

      /**
       * Sets the sensor state.
       */
      inline void SetState(BaseSensorState& s_state){
         m_psState = &s_state;
      }

      /**
       * Updates values of the sensor.
       */
      virtual void UpdateValues() = 0;

   protected:

      /**
       * Pointer to the sensor state.
       */
      BaseSensorState* m_psState;

   };

}
#endif

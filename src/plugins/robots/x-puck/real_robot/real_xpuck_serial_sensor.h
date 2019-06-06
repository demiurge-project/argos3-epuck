/**
 * @file <argos3/plugins/robots/x-puck/real_robot/real_xpuck_serial_sensor.h>
 *
 * @brief This file provides the interface for the xpuck i2c communication.
 *
 * This file provides the signature of the UpdateValues() function
 * that allows data transfer from the shared data structure
 * (BaseSensorState) and the specific private sensors structure.
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#ifndef REAL_XPUCK_SERIAL_SENSOR_H
#define REAL_XPUCK_SERIAL_SENSOR_H

namespace argos {
   class CRealXPuckSerialSensor;
}

#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/plugins/robots/x-puck/real_robot/real_xpuck_base.h>

namespace argos {

   class CRealXPuckSerialSensor {

   public:

      virtual ~CRealXPuckSerialSensor() {}

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

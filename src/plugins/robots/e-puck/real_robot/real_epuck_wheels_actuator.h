/**
 * @file <argos3/plugins/robots/e-puck/control_interface/ci_epuck_wheels_actuator.h>
 *
 * @brief This file provides the definition of the epuck wheels actuator.
 *
 * This file provides the definition of the epuck wheels actuator.
 * This actuator sets the speed of the wheels. On the real e-puck, the
 * speed is set by a PID controller, which has a specific period here
 * referred to as T_PID. The actual value accepted by the ropuck as
 * speed is expressed in pulse per pid period.  In the common
 * interface, the user can specify the linear velocity of the ropuck,
 * that is the distance covered by the ropuck (in cm) per second. This
 * value is related to the pulse input to the motors. Therefore, here
 * we provide conversion factors that are also used in other related
 * sensors, such as the epuck wheel speed sensor and the epuck encoder
 * sensor.
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#ifndef REAL_EPUCK_WHEELS_ACTUATOR_H
#define REAL_EPUCK_WHEELS_ACTUATOR_H

namespace argos {
   class CRealEPuckWheelsActuator;
}

#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_serial_actuator.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_wheels_actuator.h>

namespace argos {

   class CRealEPuckWheelsActuator : virtual public CCI_EPuckWheelsActuator,
                                    virtual public CRealEPuckSerialActuator {

   public:

      virtual ~CRealEPuckWheelsActuator() {
      }

      virtual void SetLinearVelocity(Real f_left_velocity, Real f_right_velocity);

   };
}
#endif

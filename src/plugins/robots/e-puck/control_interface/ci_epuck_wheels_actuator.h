/**
 * @file <argos3/plugins/robots/e-puck/control_interface/ci_epuck_wheels_actuator.h>
 *
 * @brief This file provides the definition of the epuck wheels actuator.
 *
 * This file provides the definition of the epuck wheels actuator.
 * This actuator sets the speed of the wheels. On the real robot,
 * the speed is set by a PID controller, which has a specific
 * period here referred to as T_PID. The actual value accepted by
 * the ropuck as speed is expressed in pulse per pid period.  In
 * the common interface, the user can specify the linear velocity
 * of the ropuck, that is the distance covered by the e-puck (in
 * cm) per second. This value is related to the pulse input to the
 * motors. Therefore, here we provide conversion factors that are
 * also used in other related sensors, such as the epuck wheel
 * speed sensor and the epuck encoder sensor.
 *
 * @author Lorenzo Garattoni- <lgaratto@ulb.ac.be>
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#ifndef CCI_EPUCK_WHEELS_ACTUATOR_H
#define CCI_EPUCK_WHEELS_ACTUATOR_H

namespace argos {
   class CCI_EPuckWheelsActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>
#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

   class CCI_EPuckWheelsActuator : virtual public CCI_Actuator {

   public:

      /**
       * Maximum linear speed for the wheels in cm/sec
       */
      static const Real MAX_VELOCITY_CM_SEC;

      /**
       * Minimum linear speed for the wheels in cm/sec
       */
      static const Real MIN_VELOCITY_CM_SEC;

   public:

      /**
       * This method set the velocity of the wheels
       *
       * @param f_left_velocity velocity of the left wheel
       * @param f_right_velocity velocity of the right wheel
       */
      virtual void SetLinearVelocity(Real f_left_velocity,
                                     Real f_right_velocity) = 0;

#ifdef ARGOS_WITH_LUA
      /**
       *
       * @param pt_lua_state
       */
      virtual void CreateLuaState(lua_State* pt_lua_state);
#endif
   };
}
#endif

/**
 * @file <argos3/plugins/robots/e-puck/control_interface/epuck/ci_epuck_front_camera_sensor.h>
 *
 * @brief This file provides the definition of the e-puck front camera
 * sensor.  Which is a modified copy/paste of the FootBot ceiling
 * camera sensor done by Carlo Pinciroli - <cpinciro@ulb.ac.be>.
 *
 * @author Mikael Lenaertz - <miklenae@ulb.ac.be>
 */

#ifndef CI_EPUCK_FRONT_CAMERA_SENSOR_H
#define CI_EPUCK_FRONT_CAMERA_SENSOR_H

namespace argos {
   class CCI_EpuckFrontCameraSensor;
}

#include <argos2/common/control_interface/ci_camera_sensor.h>

namespace argos {

   class CCI_EpuckFrontCameraSensor: virtual public CCI_CameraSensor {

     public:

      /**
       * Constructor
       */
     CCI_EpuckFrontCameraSensor() :
      CCI_CameraSensor()
      {}

      /**
       * Destructor
       */
      virtual ~CCI_EpuckFrontCameraSensor() {}

#ifdef ARGOS_WITH_LUA
      /**
       *
       * @param pt_lua_state
       */
      virtual void CreateLuaState(lua_State* pt_lua_state) {}
#endif
   };
}
#endif

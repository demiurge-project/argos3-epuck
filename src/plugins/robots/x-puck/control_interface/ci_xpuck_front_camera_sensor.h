/**
 * @file <argos3/plugins/robots/x-puck/control_interface/xpuck/ci_xpuck_front_camera_sensor.h>
 *
 * @brief This file provides the definition of the x-puck front camera
 * sensor.  Which is a modified copy/paste of the FootBot ceiling
 * camera sensor done by Carlo Pinciroli - <cpinciro@ulb.ac.be>.
 *
 * @author Mikael Lenaertz - <miklenae@ulb.ac.be>
 */

#ifndef CI_XPUCK_FRONT_CAMERA_SENSOR_H
#define CI_XPUCK_FRONT_CAMERA_SENSOR_H

namespace argos {
   class CCI_XpuckFrontCameraSensor;
}

#include <argos2/common/control_interface/ci_camera_sensor.h>

namespace argos {

   class CCI_XpuckFrontCameraSensor: virtual public CCI_CameraSensor {

     public:

      /**
       * Constructor
       */
     CCI_XpuckFrontCameraSensor() :
      CCI_CameraSensor()
      {}

      /**
       * Destructor
       */
      virtual ~CCI_XpuckFrontCameraSensor() {}

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

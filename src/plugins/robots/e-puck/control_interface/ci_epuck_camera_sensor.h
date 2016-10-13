/**
 * @file <argos3/plugins/robots/e-puck/control_interface/ci_camera_sensor.h
 *
 * @brief This file provides the definition of the e-puck camera
 * sensor and of the data structure representing its readings.  The
 * camera sensor enables the user to extract information from the
 * image acquired by the simulated or by the physical camera (under
 * development).
 *
 * This interface defines also the basic type of information that at
 * the moment it is possible to extract from image processing on the
 * real robot, that is the position of the LED of neighboring robots.
 *
 * FIXME: This has a standard resolution of 1x60 pixels, but returns
 *        readings of 1x120 pixels.  Why? This is quite some
 *        bullshit. Please rework and sync with new epuck board. Arne
 *
 * @author Laurent Compere <laurent.compere@ulb.ac.be>
 */
#ifndef CI_CAMERA_SENSOR_H
#define CI_CAMERA_SENSOR_H

namespace argos {
   class CCI_CameraSensor;
}

#include <vector>
#include <argos3/core/utility/math/angles.h>
#include <argos3/core/utility/datatypes/color.h>
#include <argos3/core/control_interface/ci_sensor.h>

namespace argos {

   class CCI_EPuckCameraSensor: virtual public CCI_Sensor {

     public:

      /**
       * Constructor
       */
      CCI_EPuckCameraSensor() {
      }

      /**
       * Destructor
       */
      virtual ~CCI_EPuckCameraSensor() {
      }

      /**
       * Returns a reference to the current camera readings.
       */
      inline virtual UInt8 ** GetCameraReadings() {
         return m_punCameraReadings;
      }

      /**
       * Enables image acquisition and processing.
       */
      virtual void Enable() = 0;

      /**
       * Disables image acquisition and processing.
       */
      virtual void Disable() = 0;

      /**
       * Sets the resolution of the camera (size limited).
       */
      virtual void ChangeResolution(UInt16 un_width, UInt16 un_height) = 0;

      /**
       * Return the width of the image
       */
      virtual UInt16 GetWidth() = 0;

      /**
       * Return the height of the image
       */
      virtual UInt16 GetHeight() = 0;

#ifdef ARGOS_WITH_LUA
      /**
       *
       * @param pt_lua_state
       */
      virtual void CreateLuaState(lua_State* pt_lua_state) {}
#endif

     protected:
      /**
       * camera readings
       */
      UInt8 ** m_punCameraReadings;
   };
}
#endif

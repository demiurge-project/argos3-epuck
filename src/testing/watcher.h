#ifndef WATCHER_H
#define WATCHER_H

#include <argos3/core/control_interface/ci_controller.h>

#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_wheels_actuator.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_base_leds_actuator.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_rgb_leds_actuator.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_range_and_bearing_actuator.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_ircom_actuator.h>

#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_proximity_sensor.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_light_sensor.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_ground_sensor.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_range_and_bearing_sensor.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_ircom_sensor.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_omnidirectional_camera_sensor.h>

namespace argos {
   class CWatcherController : public CCI_Controller {
     public:

      CWatcherController();
      virtual ~CWatcherController();

      virtual void Init(TConfigurationNode& t_node);
      virtual void ControlStep();
      virtual void Reset() {};
      virtual void Destroy() {};

     private:

      /* parse the <params> xml tree from the config file */
      void ParseParams(TConfigurationNode& t_node);

      UInt32 m_uRobotID;
      UInt32 m_nDelay;
      UInt32 m_nRange;
      UInt32 m_nDataSize;
      UInt32 m_nDistance;
      UInt32 m_nRun;

      std::string filename;

      SInt32 m_sLeftWheelSpeed;
      SInt32 m_sRightWheelSpeed;

      UInt32 m_nControlStep;

      CCI_EPuckWheelsActuator* m_pcWheelsActuator;
      CCI_EPuckBaseLEDsActuator* m_pcLEDsActuator;
      CCI_EPuckRGBLEDsActuator* m_pcRGBLEDsActuator;
      CCI_EPuckRangeAndBearingActuator* m_pcRABActuator;
      CCI_EPuckIRComActuator* m_pcIRComActuator;

      CCI_EPuckProximitySensor* m_pcProximitySensor;
      CCI_EPuckLightSensor* m_pcLightSensor;
      CCI_EPuckGroundSensor* m_pcGroundSensor;
      CCI_EPuckRangeAndBearingSensor* m_pcRABSensor;
      CCI_EPuckIRComSensor* m_pcIRComSensor;
      CCI_EPuckOmnidirectionalCameraSensor* m_pcCamera;
   };
};

#endif

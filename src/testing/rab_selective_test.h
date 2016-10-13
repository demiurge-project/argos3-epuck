#ifndef RAB_SELECTIVE_EPUCK_H
#define RAB_SELECTIVE_EPUCK_H

#include <argos3/core/control_interface/ci_controller.h>

#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_base_leds_actuator.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_rgb_leds_actuator.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_wheels_actuator.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_ircom_actuator.h>

#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_proximity_sensor.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_ircom_sensor.h>

/* Definition of the camera sensor */
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_omnidirectional_camera_sensor.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_range_and_bearing_sensor.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_range_and_bearing_actuator.h>
namespace argos {
   class CEPuckController : public CCI_Controller {
     public:

      CEPuckController();
      virtual ~CEPuckController();

      virtual void Init(TConfigurationNode& t_node);
      virtual void ControlStep();
      virtual void Reset() {};
      virtual void Destroy() {};

     private:

      /* parse the <params> xml tree from the config file */
      void ParseParams(TConfigurationNode& t_node);

      UInt32 m_uRobotID;
      bool m_bChangingDirection;

      SInt32 m_sLeftWheelSpeed;
      SInt32 m_sRightWheelSpeed;

      UInt32 m_nControlStep;

      CCI_EPuckWheelsActuator* m_pcWheelsActuator;
      CCI_EPuckIRComActuator* m_pcIRComActuator;
      CCI_EPuckBaseLEDsActuator* m_pcLEDsActuator;
      CCI_EPuckRGBLEDsActuator* m_pcRGBLEDsActuator;
      CCI_EPuckRangeAndBearingActuator* m_pcRABActuator;

      CCI_EPuckProximitySensor* m_pcProximitySensor;
      CCI_EPuckIRComSensor* m_pcIRComSensor;
      CCI_EPuckRangeAndBearingSensor* m_pcRABSensor;

      /* Pointer to the omnidirectional camera sensor */
      CCI_EPuckOmnidirectionalCameraSensor* m_pcCamera;

   };
};

#endif

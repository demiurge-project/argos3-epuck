/*
 * @author Lorenzo Garattoni - <lgarattoni@iridia.ulb.ac.be>
 */

#ifndef _CEPUCKCALIBRATOR_H_
#define _CEPUCKCALIBRATOR_H_


#include <argos3/core/control_interface/ci_controller.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_light_sensor.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_proximity_sensor.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_ground_sensor.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_range_and_bearing_sensor.h>

#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_wheels_actuator.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_base_leds_actuator.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_range_and_bearing_actuator.h>

namespace argos {

   class CEPuckCalibrator : public CCI_Controller {

   private:
      CCI_EPuckWheelsActuator* m_pcWheelsActuator;

      SInt32 m_nCurSamples;
      SInt32 m_nNumSamples;
      SInt32 m_nIgnoreSamples;
      Real m_fWheelSpeed;

      // light sensors
      CCI_EPuckLightSensor* m_pcLightSensor;
      Real* m_pfLightMinValues;
      bool  m_bMinLightAcquired;
      std::string m_sLightConfFile;

      // proximity sensors
      CCI_EPuckProximitySensor* m_pcProximitySensor;
      Real* m_pfProximityMinValues;
      Real* m_pfProximityMaxValues;
      bool  m_bMinProximityAcquired;
      bool  m_bMaxProximityAcquired;
      std::string m_sProximityConfFile;

      // base ground sensor
      CCI_EPuckGroundSensor* m_pcGroundSensor;
      Real* m_pfGroundMinValues;
      Real* m_pfGroundMaxValues;
      bool  m_bMinGroundAcquired;
      bool  m_bMaxGroundAcquired;
      std::string m_sGroundConfFile;

      // range and bearing sensors
      CCI_EPuckRangeAndBearingSensor* m_pcRangeAndBearingSensor;
      CCI_EPuckRangeAndBearingActuator* m_pcRangeAndBearingActuator;
      Real m_pfRangeAndBearingKValue;
      bool  m_bRangeAndBearingKAcquired;
      std::string m_sRangeAndBearingConfFile;
      UInt8 m_unId;
      std::vector<CRange<CRadians> > angleRanges;
      std::vector<UInt32 > angleRangesCount;
      Real m_fExpA;
      Real m_fExpB;
   public:

      CEPuckCalibrator();
      ~CEPuckCalibrator();

      virtual void Init( TConfigurationNode& t_node );
      virtual void ControlStep( void );
      virtual void Reset();

      inline virtual bool IsControllerFinished() const
      {
         return (((m_pcLightSensor           == NULL) || m_bMinLightAcquired) &&
                 (((m_pcProximitySensor       == NULL) || (m_bMinProximityAcquired && m_bMaxProximityAcquired))) &&
                 ((m_pcGroundSensor          == NULL) || (m_bMinGroundAcquired && m_bMaxGroundAcquired)) &&
                 ((m_pcRangeAndBearingSensor == NULL)));
      };

   };

};

#endif

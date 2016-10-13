/**
 * @file <argos3/plugins/robots/real_robot/real_epuck_light_sensor.cpp>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 * @author Matteo Amaducci <matteo.amaducci2@studio.unibo.it>
 */

#include "real_epuck_light_sensor.h"
#include <argos3/core/utility/logging/argos_log.h>

static const std::string CONFIGURATION_PATH = "/home/root/ARGoS/low_level_configuration/light_sensor.xml";

namespace argos {

   /****************************************/
   /****************************************/

   CRealEPuckLightSensor::CRealEPuckLightSensor():
      m_sLowLevelConfigurationFile(CONFIGURATION_PATH),
      m_bCalibrated(false),
      m_vecRanges(8, CRange<Real>(0.0f, 4096.0f)) {}

   /****************************************/
   /****************************************/

   void CRealEPuckLightSensor::UpdateValues() {
      /* go through all the light sensors */
      for(int i = 0 ; i < 8 ; ++i){
         /* if the sensor is calibrated */
         if(m_bCalibrated){
            /* if the sensor [i] is disabled */
            if (m_vecDisabledSensors[i]) {
               /* set the min light value: 0 */
               m_tReadings[7-i].Value = 0;
            } else {
               /* calcul the normalized value */
               Real rValue = 4096.0f - m_psState->Light[i];
               m_tReadings[7-i].Value = m_vecRanges[i].NormalizeValue(rValue);
            }
         } else {
            /* set the raw value */
            m_tReadings[7 - i].Value = 4096.0f - m_psState->Light[i];
         }
      }
   }

   /****************************************/
   /****************************************/

   void CRealEPuckLightSensor::Init(TConfigurationNode& t_node) {
      try {
         GetNodeAttributeOrDefault(t_node,
                                   "calibrated",
                                   m_bCalibrated,
                                   m_bCalibrated);
      } catch (CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Unable to init light sensor, \"calibrated\" attribute "
                                     << "is missing, mandatory", ex);
      }

      if(m_bCalibrated) {
         try {
            GetNodeAttributeOrDefault(t_node,
                                      "low_level_config",
                                      m_sLowLevelConfigurationFile,
                                      m_sLowLevelConfigurationFile);
         } catch (CARGoSException& ex) {
            THROW_ARGOSEXCEPTION_NESTED("Unable to init light sensor, \"low_level_config\" "
                                        << "attribute is missing, "
                                        << "mandatory when \"calibrated\" attribute is true", ex);
         }
         Calibrate();
      } else {
         LOGERR << "[WARNING] Using light sensor without calibration." << std::endl;
      }
   }


   /****************************************/
   /****************************************/

   void CRealEPuckLightSensor::Calibrate() {
      try {
         ticpp::Document tConfiguration(m_sLowLevelConfigurationFile);
         tConfiguration.LoadFile();
         TConfigurationNode tGroundNode =
            GetNode(*tConfiguration.FirstChildElement(), "light_sensors");
         TConfigurationNode tMinNode = GetNode(tGroundNode, "base_values");
         std::string sMinText = tMinNode.GetText();
         Real pfMinInput[8];
         ParseValues<Real>(sMinText,8,pfMinInput,',');
         // optional tag to disable broken sensors
         bool bDisabledInput[8];
         ::memset(&bDisabledInput, 0, 8);
         if (NodeExists(tGroundNode, "disabled") == true) {
            TConfigurationNode tDisabledNode =
               GetNode(tGroundNode, "disabled");
            std::string sDisabledText = tDisabledNode.GetText(false);
            ParseValues<bool>(sDisabledText,
                              8,
                              bDisabledInput,
                              ',');
         }
         // set ranges and disabled sensors; warn about bad behavior here
         for( UInt8 i = 0; i < 8; ++i ) {
            m_vecDisabledSensors.push_back(bDisabledInput[i]);
            if (m_vecDisabledSensors[i]) {
               LOGERR << "[WARNING] The light sensor ["
                      << i
                      << "] has been disabled." << std::endl;
            }
            m_vecRanges[7 - i].SetMin(pfMinInput[i]);
            LOGERR << "[LIGHT] RANGE" << 7-i << " = " << m_vecRanges[7 - i] << std::endl;
         }
      } catch (std::exception& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Unable to read light sensor range values from file", ex);
      }
   }

   /****************************************/
   /****************************************/
}

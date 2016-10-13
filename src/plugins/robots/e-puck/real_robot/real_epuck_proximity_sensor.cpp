/**
 * @file <argos3/plugins/robots/e-puck/real_robot/real_epuck_proximity_sensor.cpp>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 * @author Matteo Amaducci <matteo.amaducci2@studio.unibo.it>
 */

#include "real_epuck_proximity_sensor.h"
#include <argos3/core/utility/logging/argos_log.h>

static const std::string CONFIGURATION_FILE = "/home/root/ARGoS/low_level_configuration/proximity_sensor.xml";

namespace argos {

   /****************************************/
   /****************************************/

   CRealEPuckProximitySensor::CRealEPuckProximitySensor():
      m_sLowLevelConfigurationFile(CONFIGURATION_FILE),
      m_bCalibrated(false),
      m_vecRanges(8, CRange<Real>(0.0f,4096.0f)) {
   }

   /****************************************/
   /****************************************/

   void CRealEPuckProximitySensor::UpdateValues() {
      if (m_bCalibrated) {
         /* if the proximity sensor is calibrated */
         /* go through all the sensors */
         for (int i = 0 ; i < 8 ; ++i) {
            if (m_vecDisabledSensors[i]) {
               /* if disabled, set value to 0.0 */
               m_tReadings[7 - i].Value = 0.0;
            } else {
               /* if active, set value to the normalized information received */
               m_tReadings[7 - i].Value = m_vecRanges[i].NormalizeValue(m_psState->Proximity[i]);
            }
         }
      } else {
         /* as the sensor is not calibrated, we are unable to normalize values */
         /* so set the value received without normalization */
         for (int i = 0 ; i < 8 ; ++i)
            m_tReadings[7 - i].Value = m_psState->Proximity[i];
      }
   }

   /****************************************/
   /****************************************/

   void CRealEPuckProximitySensor::Init(TConfigurationNode& t_node) {

      /* get the attribute "calibrated" in the node configuration of this sensor */
      GetNodeAttributeOrDefault(t_node,
                                "calibrated",
                                m_bCalibrated,
                                m_bCalibrated);

      /* if the attribute "calibrated" was there */
      if (m_bCalibrated) {
            /* search for the low_level_config attribute in the node configuration of this sensor */
            GetNodeAttributeOrDefault(t_node,
                             "low_level_config",
                             m_sLowLevelConfigurationFile,
                             m_sLowLevelConfigurationFile);
            /* then calibrate the sensor */
            Calibrate(m_sLowLevelConfigurationFile);
      } else {
         LOGERR << "[WARNING] Using proximity sensor without calibration." << std::endl;
      }

   }

   /****************************************/
   /****************************************/

   void CRealEPuckProximitySensor::Calibrate(const std::string& str_conf_file) {
      try {
         ticpp::Document tConfiguration(str_conf_file);
         tConfiguration.LoadFile();
         TConfigurationNode tGroundNode =
            GetNode(*tConfiguration.FirstChildElement(),
                    "proximity_sensors");
         TConfigurationNode tMinNode = GetNode(tGroundNode, "base_values");
         TConfigurationNode tMaxNode = GetNode(tGroundNode, "max_values");
         std::string sMinText = tMinNode.GetText();
         std::string sMaxText = tMaxNode.GetText();
         Real pfMinInput[8], pfMaxInput[8];
         ParseValues<Real>(sMinText, 8, pfMinInput, ',');
         ParseValues<Real>(sMaxText, 8, pfMaxInput, ',');

         /* optional tag to disable broken sensors */
         bool bDisabledInput[8];
         ::memset(&bDisabledInput, 0, 8);
         if (NodeExists(tGroundNode, "disabled")) {
            TConfigurationNode tDisabledNode = GetNode(tGroundNode, "disabled");
            /* NOTE: why false and not "false"? */
            std::string sDisabledText = tDisabledNode.GetText(false);
            ParseValues<bool>(sDisabledText, 8, bDisabledInput, ',');
         }

         // set ranges and disabled sensors; warn about bad behavior here
         for(UInt8 i = 0; i < 8; ++i) {
            m_vecDisabledSensors.push_back(bDisabledInput[i]);
            /* if sensor is disabled, warn the user */
            if (m_vecDisabledSensors[i]) {
               LOGERR << "[WARNING] The " << i
                      << ". Proximity sensor has been disabled." << std::endl;
            }

            /* if the min and max value are what's expected */
            if (pfMinInput[i] < pfMaxInput[i]) {
               m_vecRanges[7-i].Set(pfMinInput[i], pfMaxInput[i]);
            } else if (pfMinInput[i] > pfMaxInput[i]) {
               /* if the min value is not rly the min value, warn the user*/
               LOGERR << "[WARNING] The "
                      << i
                      << ". min value of the proximity sensor"
                      << " is larger than the corresponding max value."
                      << " The sensor is broken."
                      << std::endl;
               /* swap the min and max */
               m_vecRanges[7-i].Set(pfMaxInput[i], pfMinInput[i]);
            } else {
               LOGERR << "[WARNING] The "
                      << i
                      << ". min value of the proximity sensor"
                      << " is equal to the corresponding max value."
                      << " The sensor is broken."
                      << std::endl;
               /* NOTE: min = max and we do nothing? */
            }
         }
      } catch (std::exception& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Unable to read proximity sensor range values from file", ex);
      }
   }

   /****************************************/
   /****************************************/
}

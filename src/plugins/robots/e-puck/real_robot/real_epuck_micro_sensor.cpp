/**
 * @file <argos3/plugins/robots/e-puck/real_robot/real_epuck_micro_sensor.cpp>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 * @author Matteo Amaducci <matteo.amaducci2@studio.unibo.it>
 */

#include "real_epuck_micro_sensor.h"
#include <argos3/core/utility/logging/argos_log.h>

static const std::string CONFIGURATION_FILE = "/home/root/ARGoS/low_level_configuration/epuck_calibration.xml";

namespace argos {

   /****************************************/
   /****************************************/

   CRealEPuckMicroSensor::CRealEPuckMicroSensor():
      m_sLowLevelConfigurationFile(CONFIGURATION_FILE),
      m_bCalibrated(false),
      m_vecRanges(3, CRange<Real>(0.0f,1550.0f)) {}

   /****************************************/
   /****************************************/

   void CRealEPuckMicroSensor::Init(TConfigurationNode& t_node) {
      try {
          GetNodeAttributeOrDefault(t_node,
                                    "calibrated",
                                    m_bCalibrated,
                                    m_bCalibrated);

          if (m_bCalibrated) {
              GetNodeAttributeOrDefault(t_node,
                                        "low_level_config",
                                        m_sLowLevelConfigurationFile,
                                        m_sLowLevelConfigurationFile);
              Calibrate();
          } else {
              LOGERR << "[WARNING] Using micro sensor without calibration."
                     << std::endl;
          }
      } catch (CARGoSException& ex) {
          THROW_ARGOSEXCEPTION_NESTED("Unable to initialize micro sensor.",
                                      ex);
      }
   }

   /****************************************/
   /****************************************/

   void CRealEPuckMicroSensor::UpdateValues() {
      if (m_bCalibrated) {
          m_sReading.Right = (m_vecDisabledSensors[0]) ?
              0.0 :
              1.0f - m_vecRanges[0].NormalizeValue(m_psState->Micro[0]);
          m_sReading.Left = (m_vecDisabledSensors[1]) ?
              0.0 :
              1.0f - m_vecRanges[1].NormalizeValue(m_psState->Micro[1]);
          m_sReading.Back = (m_vecDisabledSensors[2]) ?
              0.0 :
              1.0f - m_vecRanges[2].NormalizeValue(m_psState->Micro[2]);
      } else {
          m_sReading.Right =
              1.0f - m_vecRanges[0].NormalizeValue(m_psState->Micro[0]);
          m_sReading.Left =
              1.0f - m_vecRanges[1].NormalizeValue(m_psState->Micro[1]);
          m_sReading.Back =
              1.0f - m_vecRanges[2].NormalizeValue(m_psState->Micro[2]);
      }
   }

   /****************************************/
   /****************************************/

   void CRealEPuckMicroSensor::Calibrate() {
   }

   /****************************************/
   /****************************************/
}

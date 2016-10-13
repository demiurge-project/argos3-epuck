/**
 * @file <argos3/plugins/robots/e-puck/real_robot/real_epuck_ground_sensor.cpp>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#include "real_epuck_ground_sensor.h"
#include <argos3/core/utility/logging/argos_log.h>

static const std::string CONFIGURATION_FILE = "/home/root/ARGoS/low_level_configuration/ground_sensor.xml";

namespace argos {

   /****************************************/
   /****************************************/

   CRealEPuckGroundSensor::CRealEPuckGroundSensor():
      m_sLowLevelConfigurationFile(CONFIGURATION_FILE),
      m_bCalibrated(false),
      m_vecRanges(3, CRange<Real>(0.0f, 1000.0f)) {
      m_tDeviceStream = OpenDevice(0x60);
   }

   /****************************************/
   /****************************************/

   CRealEPuckGroundSensor::~CRealEPuckGroundSensor() {
      CloseDevice(m_tDeviceStream);
   }

   /****************************************/
   /****************************************/

   void CRealEPuckGroundSensor::Init(TConfigurationNode& t_node) {
      try {
         GetNodeAttributeOrDefault(t_node,
                                   "calibrated",
                                   m_bCalibrated,
                                   m_bCalibrated);
         if(m_bCalibrated) {
            GetNodeAttributeOrDefault(t_node,
                                      "low_level_config",
                                      m_sLowLevelConfigurationFile,
                                      m_sLowLevelConfigurationFile);
            Calibrate();
         } else {
            LOGERR << "[WARNING] Using ground sensor without calibration."
                   << std::endl;
         }
      } catch (CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Unable to initialize base ground sensor", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CRealEPuckGroundSensor::Calibrate() {
      try {
         /* Parse configuration file */
         ticpp::Document tConfiguration(m_sLowLevelConfigurationFile);
         tConfiguration.LoadFile();
         /* Get base node for ground sensors */
         TConfigurationNode& tGroundNode = GetNode(*tConfiguration.FirstChildElement(), "ground_sensors");
         /* Get min and max nodes */
         TConfigurationNode& tMinNode = GetNode(tGroundNode, "min_values");
         TConfigurationNode& tMaxNode = GetNode(tGroundNode, "max_values");
         std::string sMinText = tMinNode.GetText();
         std::string sMaxText = tMaxNode.GetText();
         Real pfMinInput[3];
         Real pfMaxInput[3];
         ParseValues<Real>(sMinText, 3, pfMinInput,',');
         ParseValues<Real>(sMaxText, 3, pfMaxInput,',');
         /* Optional tag to disable broken sensors */
         bool bDisabledInput[3];
         ::memset(&bDisabledInput, 0, 3);
         if(NodeExists(tGroundNode, "disabled")) {
            TConfigurationNode& tDisabledNode = GetNode(tGroundNode, "disabled");
            std::string sDisabledText = tDisabledNode.GetText(false);
            ParseValues<bool>(sDisabledText,
                              3,
                              bDisabledInput,
                              ',');
         }
         /* Set ranges and disabled sensors; warn about bad behavior here */
         for(size_t i = 0; i < 3; ++i) {
            m_vecDisabledSensors.push_back(bDisabledInput[i]);
            if (m_vecDisabledSensors[i]) {
               LOGERR << "[WARNING] Ground sensor #"
                      << i
                      << " has been disabled." << std::endl;
            }

            if(pfMinInput[i] < pfMaxInput[i]) {
               m_vecRanges[i].Set(pfMinInput[i], pfMaxInput[i]);
            }
            else if(pfMinInput[i] > pfMaxInput[i]) {
               LOGERR << "[WARNING] Ground sensor #"
                      << i
                      << ": the min value is larger "
                      << "than the corresponding max value. The sensor "
                      << "is broken." << std::endl;
               m_vecRanges[i].Set(pfMaxInput[i], pfMinInput[i]);
            } else {
               LOGERR << "[WARNING] Ground sensor #"
                      << i
                      << ": the min value is equal "
                      << "to the corresponding max value. The sensor "
                      << "is broken." << std::endl;
            }
         }
      } catch (CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Unable to read ground sensor range values from file", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CRealEPuckGroundSensor::ReceiveData() {
      /* Buffer for register value */
      SInt16 nReading;
      /*
       * Go through the 6 registers and store their values
       * The values are organized in pairs; each pair is a complete ground sensor reading
       * If any register value is -1, an error occurred and we don't continue reading
       */
      for(UInt16 i = 0; i < 6; ++i) {
         nReading = ReadRegister(m_tDeviceStream, i, 1);
         if(nReading == -1){
            break;
         }
         m_unRawData[i] = nReading;
      }
      /* If no error occurred, fill the sensor readings;
       * otherwise print a warning */
      if(nReading != -1 ){
         /* Everything OK */
         if(m_bCalibrated){
            /*
             * The sensors have been calibrated, so we know whether a sensor
             * is disabled and we know the range of each sensor
             */
            m_sReadings.Left =
               m_vecDisabledSensors[0] ?
               0.0 :
               m_vecRanges[0].NormalizeValue(m_unRawData[0] * 256 + m_unRawData[1]);
            m_sReadings.Center =
               m_vecDisabledSensors[1] ?
               0.0 :
               m_vecRanges[1].NormalizeValue(m_unRawData[2] * 256 + m_unRawData[3]);
            m_sReadings.Right  =
               m_vecDisabledSensors[2] ?
               0.0 :
               m_vecRanges[2].NormalizeValue(m_unRawData[4] * 256 + m_unRawData[5]);
         } else {
            /*
             * The sensors have not been calibrated, so we copy the raw values into
             * the readings
             */
            m_sReadings.Left   = static_cast<Real>((m_unRawData[0] * 256 + m_unRawData[1])); // / 65536.0f;
            m_sReadings.Center = static_cast<Real>((m_unRawData[2] * 256 + m_unRawData[3])); // / 65536.0f;
            m_sReadings.Right  = static_cast<Real>((m_unRawData[4] * 256 + m_unRawData[5])); // / 65536.0f;
         }
      } else {
         LOGERR << "[WARNING] Error while reading ground sensor data" << std::endl;
      }
   }

   /****************************************/
   /****************************************/
}

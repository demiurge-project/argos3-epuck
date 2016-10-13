/*
 * @author Lorenzo Garattoni - <lgarattoni@iridia.ulb.ac.be>
 */

#include <unistd.h>

#include "calibrate_epuck.h"
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_wheels_actuator.h> 


using namespace argos;


/****************************************/
/****************************************/

CEPuckCalibrator::CEPuckCalibrator() : CCI_Controller() {
   m_pcWheelsActuator = NULL;

   m_nNumSamples = 0;
   m_nIgnoreSamples = 0;
   m_fWheelSpeed = 0;

   // light sensor
   m_pcLightSensor = NULL;
   m_pfLightMinValues = NULL;
   m_bMinLightAcquired = false;

   // proximity sensor
   m_pcProximitySensor = NULL;
   m_pfProximityMinValues = NULL;
   m_pfProximityMaxValues = NULL;
   m_bMinProximityAcquired = false;
   m_bMaxProximityAcquired = false;

   // ground sensor
   m_pcGroundSensor = NULL;
   m_pfGroundMinValues = NULL;
   m_pfGroundMaxValues = NULL;
   m_bMinGroundAcquired = false;
   m_bMaxGroundAcquired = false;

   // range and bearing sensor
   m_pcRangeAndBearingSensor = NULL;
   m_bRangeAndBearingKAcquired = false;	
}


/****************************************/
/****************************************/


void CEPuckCalibrator::Init(TConfigurationNode& t_tree) {
   m_pcWheelsActuator = GetActuator<CCI_EPuckWheelsActuator>("epuck_wheels");

   // number of samples for each device to configure
   GetNodeAttribute(t_tree, "num_samples", m_nNumSamples);
   GetNodeAttribute(t_tree, "ignore_first_samples", m_nIgnoreSamples);
   if (m_nIgnoreSamples < 2) {
      LOG << "[WARNING] Setting ignore_first_samples to 2, as you have to ignore at least the first 2 samples to have reliable data." << std::endl;
      m_nIgnoreSamples = 2;
   }
   m_nIgnoreSamples++;
   m_nCurSamples = -m_nIgnoreSamples;
   GetNodeAttribute(t_tree, "wheel_speed", m_fWheelSpeed);

   // LIGHT CONFIGURATION
   try {
      m_pcLightSensor = GetSensor<CCI_EPuckLightSensor>("epuck_light");
      m_pfLightMinValues = new Real[8];
      for (UInt32 j = 0; j < 8; j++) {
         //MODIFICA LIGHT
         //m_pfLightMinValues[j] = 0;
         m_pfLightMinValues[j] = 100000;
      }
      GetNodeAttribute(t_tree, "light_configuration_file", m_sLightConfFile);
      LOG << "Calibrating light sensor." << std::endl;
   } catch (CARGoSException e) {
      LOG << "Not calibrating light sensor." << std::endl;
   }

   // PROXIMITY CONFIGURATION
   try {
      m_pcProximitySensor = GetSensor<CCI_EPuckProximitySensor>("epuck_proximity");
      m_pfProximityMinValues = new Real[8];
      m_pfProximityMaxValues = new Real[8];
      for (UInt32 j = 0; j < 8; j++) {
         m_pfProximityMinValues[j] = 0;
         m_pfProximityMaxValues[j] = 0;
      }
      GetNodeAttribute(t_tree, "proximity_configuration_file", m_sProximityConfFile);
      LOG << "Calibrating proximity sensor." << std::endl;
   } catch (CARGoSException e) {
      LOG << "Not calibrating proximity sensor." << std::endl;
   }

   // BASE GROUND CONFIGURATION
   try {
      m_pcGroundSensor = GetSensor<CCI_EPuckGroundSensor>("epuck_ground");
      m_pfGroundMinValues = new Real[3];
      m_pfGroundMaxValues = new Real[3];
      for (UInt32 j = 0; j < 3; j++) {
         m_pfGroundMinValues[j] = 0;
         m_pfGroundMaxValues[j] = 0;
      }
      GetNodeAttribute(t_tree, "ground_configuration_file", m_sGroundConfFile);
      LOG << "Calibrating ground sensor." << std::endl;
   } catch (CARGoSException e) {
      LOG << "Not calibrating ground sensor." << std::endl;
   }

   // RANGE AND BEARING CONFIGURATION
   try {
      m_pcRangeAndBearingSensor = GetSensor<CCI_EPuckRangeAndBearingSensor>("epuck_range_and_bearing");
      m_pcRangeAndBearingActuator = GetActuator<CCI_EPuckRangeAndBearingActuator>("epuck_range_and_bearing");
      m_pfRangeAndBearingKValue = 0.0f;
      GetNodeAttribute(t_tree, "range_and_bearing_configuration_file", m_sRangeAndBearingConfFile);
      LOG << "Calibrating range and bearing sensor." << std::endl;
   } catch (CARGoSException e) {
      LOG << "Not calibrating range and bearing sensor." << std::endl;
   }
   int number;
   std::string num = GetId();
   number = atoi(GetId().substr(5,6).c_str());
   LOG << "TEXT ID = " << GetId() << "\n";
   LOG << "Num ID = " << number << "\n";
   m_unId = number;


   UInt32 numberOfSlices = 8;

   CRadians delta = CRadians::TWO_PI / numberOfSlices;
   for (UInt32 i = 0; i < numberOfSlices; i++) {
      angleRanges.push_back(CRange<CRadians > ((delta * i).UnsignedNormalize(), (delta * (i + 1)).UnsignedNormalize()));
      LOG << "SLICE " << i << ": " << CRange<CRadians > (delta*i, delta * (i + 1)) << "\n";
      angleRangesCount.push_back(0);
   }

   // Exponential default values (same values as in the real-epuck sensor)
   m_fExpA = 9.06422181283387f;
   m_fExpB = -0.00565074879677167f;

}


/****************************************/

/****************************************/

void CEPuckCalibrator::ControlStep() {
   //    printf("%d\n", m_nIgnoreSamples);
   //    printf("%d\n", m_nCurSamples);
   // Acquire data for light sensor
   if (m_pcLightSensor != NULL && !m_bMinLightAcquired) {
      if (m_nCurSamples == -m_nIgnoreSamples) {
         LOG << std::endl;
         LOG << "----------------[ AMBIENT LIGHT ]-------------------------------------" << std::endl;
         LOG << "Light sensor calibration (average over " << m_nNumSamples << " readings)" << std::endl;
         LOG << std::endl;
         LOG << "Instructions:" << std::endl;
         LOG << " * place your robot away from any obstacles (min distance 10cm)" << std::endl;
         LOG << " * use ambient light as in planned experimental setting" << std::endl;
         LOG << " * do NOT switch on your directional light" << std::endl;
         LOG << std::endl;
         LOG << "----------------------------------------------------------------------" << std::endl;
         LOG << std::endl;
         LOG << "<hit any key to continue. TO EXIT, press ENTER and then CTRL+C >";
         getchar();
         m_pcWheelsActuator->SetLinearVelocity(-m_fWheelSpeed, m_fWheelSpeed);
         m_nCurSamples++;
      } else {
         if (m_nCurSamples >= 0) {
            LOG << "Light sensor sample " << m_nCurSamples << ": ";
            CCI_EPuckLightSensor::TReadings readings = m_pcLightSensor->GetReadings();
            for (UInt32 j = 0; j < 8; j++) {
               //MODIFICA LIGHT
               //m_pfLightMinValues[j] += readings[j].Value;
               //REMOVE ALL THIS
               if(readings[j].Value<m_pfLightMinValues[j]){
                  m_pfLightMinValues[j] = readings[j].Value;
               }
               //TILL HERE
               printf("%f ", readings[j].Value);
            }
            LOG << std::endl;
         } else {
            LOG << "Ignoring sample." << std::endl;
         }

         m_nCurSamples++;
         if (m_nCurSamples == m_nNumSamples) {
            LOG << "Data for light sensor acquired" << std::endl;
            m_bMinLightAcquired = true;
            m_nCurSamples = -m_nIgnoreSamples;
            m_pcWheelsActuator->SetLinearVelocity(0, 0);
         }
      }
      return;
   }

   // Acquire data for proximity sensor
   if (m_pcProximitySensor != NULL && !m_bMinProximityAcquired) {
      if (m_nCurSamples == -m_nIgnoreSamples) {
         LOG << std::endl;
         LOG << "---------------[ NO OBSTACLES ]---------------------------------------" << std::endl;
         LOG << "Proximity sensor calibration (average over " << m_nNumSamples << " readings)" << std::endl;
         LOG << std::endl;
         LOG << "Instructions:" << std::endl;
         LOG << " * place your robot away from any obstacles (min distance 10cm)" << std::endl;
         LOG << " * use light conditions as in planned experimental setting" << std::endl;
         LOG << " * beware of shiny stuff in the environment (metal, reflective tape)" << std::endl;
         LOG << std::endl;
         LOG << "----------------------------------------------------------------------" << std::endl;
         LOG << std::endl;
         LOG << "<hit any key to continue. TO EXIT, press ENTER and then CTRL+C >";
         getchar();
         m_pcWheelsActuator->SetLinearVelocity(-m_fWheelSpeed, m_fWheelSpeed);
         m_nCurSamples++;
      } else {
         if (m_nCurSamples >= 0) {
            LOG << "Proximity sensor sample " << m_nCurSamples << ": ";
            CCI_EPuckProximitySensor::TReadings readings = m_pcProximitySensor->GetReadings();
            for (UInt32 j = 0; j < 8; j++) {
               m_pfProximityMinValues[j] += readings[j].Value;
               printf("%f ", readings[j].Value);
            }
            LOG << std::endl;
         } else {
            LOG << "Ignoring sample." << std::endl;
         }

         m_nCurSamples++;
         if (m_nCurSamples == m_nNumSamples) {
            LOG << "Data for proximity sensor acquired" << std::endl;
            m_bMinProximityAcquired = true;
            m_nCurSamples = -m_nIgnoreSamples;
            m_pcWheelsActuator->SetLinearVelocity(0, 0);
         }

         if (m_nCurSamples == m_nNumSamples) {
            LOG << "BASE Data for proximity sensor acquired" << std::endl;
            m_bMinProximityAcquired = (m_pcProximitySensor != NULL);
            m_nCurSamples = -m_nIgnoreSamples;
            m_pcWheelsActuator->SetLinearVelocity(0, 0);
         }
      }
      return;
   }
   //OBSTACLE PROXIMITY DATA
   if (m_pcProximitySensor != NULL && !m_bMaxProximityAcquired) {
      if (m_nCurSamples == -m_nIgnoreSamples) {
         LOG << std::endl;

         LOG << "---------------[ WITH OBSTACLES ]---------------------------------------" << std::endl;
         LOG << "Proximity sensor calibration (maximum over " << m_nNumSamples << " readings)" << std::endl;
         LOG << std::endl;
         LOG << "Instructions:" << std::endl;
         LOG << " * place your robot in contact with an obstacles (distance 0 cm) I mean 0 cm. " << std::endl;
         LOG << " * use light conditions as in planned experimental setting" << std::endl;
         LOG << " * beware of shiny stuff in the environment (metal, reflective tape)" << std::endl;
         LOG << std::endl;
         LOG << "----------------------------------------------------------------------" << std::endl;
         LOG << std::endl;
         LOG << "<hit any key to continue. TO EXIT, press ENTER and then CTRL+C >";
         getchar();
         m_pcWheelsActuator->SetLinearVelocity(-m_fWheelSpeed, m_fWheelSpeed);
         m_nCurSamples++;
      } else {
         if (m_nCurSamples >= 0) {
            LOG << "Proximity sensor sample " << m_nCurSamples << ": ";
            CCI_EPuckProximitySensor::TReadings readings = m_pcProximitySensor->GetReadings();
            for (UInt32 j = 0; j < 8; j++) {
               if (m_pfProximityMaxValues[j] < readings[j].Value) {
                  m_pfProximityMaxValues[j] = readings[j].Value;
               }
               printf("%f ", readings[j].Value);
            }
            LOG << std::endl;

         } else {
            LOG << "Ignoring sample." << std::endl;
         }

         m_nCurSamples++;
         if (m_nCurSamples == m_nNumSamples) {
            LOG << "Data for Max proximity acquired" << std::endl;
            m_bMaxProximityAcquired = (m_pcProximitySensor != NULL);
            m_nCurSamples = -m_nIgnoreSamples;
            m_pcWheelsActuator->SetLinearVelocity(0, 0);
         }
      }
      return;
   }




   // Acquire data for DARK ground
   if (m_pcGroundSensor != NULL && !m_bMinGroundAcquired) {
      if (m_nCurSamples == -m_nIgnoreSamples) {
         LOG << std::endl;
         LOG << "-----------[ DARK GROUND ]----------------------------------------------" << std::endl;
         LOG << "Ground sensor calibration (average over " << m_nNumSamples << " readings)" << std::endl;
         LOG << std::endl;
         LOG << "Instructions:" << std::endl;
         LOG << " * place your robot on a dark surface (e.g., black)" << std::endl;
         LOG << " * use light conditions as in planned experimental setting." << std::endl;
         LOG << " * beware of shiny stuff on the surface (tape, nails etc)" << std::endl;
         LOG << std::endl;
         LOG << "------------------------------------------------------------------------" << std::endl;
         LOG << std::endl;
         LOG << "<hit any key to continue. TO EXIT, press ENTER and then CTRL+C >" << std::endl;
         getchar();
         m_pcWheelsActuator->SetLinearVelocity(-m_fWheelSpeed, m_fWheelSpeed);
         m_nCurSamples++;
      } else {
         if (m_nCurSamples >= 0) {
            LOG << "Ground sensor sample " << m_nCurSamples << ": ";
            Real breadings[3];
            m_pcGroundSensor->GetReadings(breadings);
            for (UInt32 j = 0; j < 3; j++) {
               m_pfGroundMinValues[j] += breadings[j];
               printf("%f ", breadings[j]);
            }
            LOG << std::endl;
         } else {
            LOG << "Ignoring sample." << std::endl;
         }

         m_nCurSamples++;
         if (m_nCurSamples == m_nNumSamples) {
            LOG << "Data for BLACK ground acquired" << std::endl;
            m_bMinGroundAcquired = (m_pcGroundSensor != NULL);
            m_nCurSamples = -m_nIgnoreSamples;
            m_pcWheelsActuator->SetLinearVelocity(0, 0);
         }
      }
      return;
   }

   // Acquire data for LIGHT ground
   if (m_pcGroundSensor != NULL && !m_bMaxGroundAcquired) {
      if (m_nCurSamples == -m_nIgnoreSamples) {
         LOG << std::endl;
         LOG << "------------[ LIGHT GROUND ]--------------------------------------------" << std::endl;
         LOG << "Ground sensor calibration (average over " << m_nNumSamples << " readings)" << std::endl;
         LOG << std::endl;
         LOG << "Instructions:" << std::endl;
         LOG << " * place your robot on your most light surface (e.g., white/shiny)" << std::endl;
         LOG << " * use light conditions as in planned experimental setting." << std::endl;
         LOG << " * beware of black spots on the surface (tape, dirt etc)" << std::endl;
         LOG << std::endl;
         LOG << "------------------------------------------------------------------------" << std::endl;
         LOG << std::endl;
         LOG << "<hit any key to continue. TO EXIT, press ENTER and then CTRL+C >" << std::endl;
         getchar();
         m_pcWheelsActuator->SetLinearVelocity(-m_fWheelSpeed, m_fWheelSpeed);
         m_nCurSamples++;
      } else {
         if (m_nCurSamples >= 0) {
            LOG << "Ground sensor sample " << m_nCurSamples << ": ";
            Real breadings[3];
            m_pcGroundSensor->GetReadings(breadings);
            for (UInt32 j = 0; j < 3; j++) {
               m_pfGroundMaxValues[j] += breadings[j];
               printf("%f ", breadings[j]);
            }
            LOG << std::endl;
         } else {
            LOG << "Ignoring sample." << std::endl;
         }

         m_nCurSamples++;
         if (m_nCurSamples == m_nNumSamples) {
            LOG << "Data for WHITE ground acquired" << std::endl;
            m_bMaxGroundAcquired = (m_pcGroundSensor != NULL);
            m_nCurSamples = -m_nIgnoreSamples;
            m_pcWheelsActuator->SetLinearVelocity(0, 0);
         }
      }
      return;
   }
   // Acquire data for range and bearing sensor
   if (m_pcRangeAndBearingSensor != NULL && !m_bRangeAndBearingKAcquired) {
      if (m_nCurSamples == -m_nIgnoreSamples) {
         LOG << std::endl;
         LOG << "---------------[ RANGE AND BEARING]---------------------------------------" << std::endl;
         LOG << "Range and bearing sensor calibration (average over " << m_nNumSamples << " readings)" << std::endl;
         LOG << std::endl;
         LOG << "Instructions:" << std::endl;
         LOG << "You need 2 robots with the same controller and the same configuration file (same number of samples etc.)" << std::endl;
         LOG << " * place the 2 robots away from any obstacles (min distance 10cm) and with a distance of 20cm between them" << std::endl;
         LOG << " * make sure that the trajectory between the 2 robots is completely clear" << std::endl;
         LOG << " * use light conditions as in planned experimental setting" << std::endl;
         LOG << " * beware of shiny stuff in the environment (metal, reflective tape)" << std::endl;
         LOG << "When BOTH robots display their FINISHED messages, manually stop the controllers with 'Ctrl-C'." << std::endl;
         LOG << std::endl;
         LOG << "----------------------------------------------------------------------" << std::endl;
         LOG << std::endl;
         LOG << "<hit any key to continue (make sure to run the controller on both the robot at the same time)>";
         getchar();
         m_pcWheelsActuator->SetLinearVelocity(-m_fWheelSpeed, m_fWheelSpeed);
         m_nCurSamples++;
      } else {
         if (m_nCurSamples >= 0) {
            UInt8 data[4];
            data[0] = m_unId;
            data[1] = 0;
            data[2] = 0;
            data[3] = 0;
            m_pcRangeAndBearingActuator->SetData(data);
            CCI_EPuckRangeAndBearingSensor::TPackets lastReceivedPackets = m_pcRangeAndBearingSensor->GetPackets();

            CCI_EPuckRangeAndBearingSensor::TPackets::iterator itPackets;

            for (itPackets = lastReceivedPackets.begin(); itPackets != lastReceivedPackets.end(); itPackets++) {
               CCI_EPuckRangeAndBearingSensor::SReceivedPacket * cPackets = *itPackets;
			   //Ignore packets received from your own id and from id 0
               if (cPackets->Data[0] != m_unId
                   && cPackets->Data[0] != 0
                   && cPackets->Bearing != CRadians::ZERO) {
                  m_pfRangeAndBearingKValue += cPackets->Range;
                  LOG << "SAMPLE RANGE nbr " << m_nCurSamples << " : " << cPackets->Range <<std::endl;
                  m_nCurSamples++;

                  for (UInt32 i = 0; i < angleRanges.size(); i++) {
                     if (angleRanges[i].WithinMinBoundIncludedMaxBoundExcluded(cPackets->Bearing.UnsignedNormalize())) {
                        angleRangesCount[i] = angleRangesCount[i] + 1;

                        break;
                     }
                  }

               }
			}
         } else {
            m_nCurSamples++;
         }
         m_pcRangeAndBearingSensor->ClearPackets();
         if (m_nCurSamples >= m_nNumSamples) {
            LOG << "Data for range and bearing sensor acquired" << std::endl;
            m_bRangeAndBearingKAcquired = true;
            
         }
      }
      return;
   } else {
	  LOG << "FINISHED. When both robots are done, press Ctrl-C." << std::endl; 
      if (m_pcRangeAndBearingSensor != NULL) {
		 UInt8 data[4];
         data[0] = m_unId;
         data[1] = 0;
         data[2] = 0;
         data[3] = 0;
         m_pcRangeAndBearingActuator->SetData(data);
         
         m_pcRangeAndBearingSensor->ClearPackets();
      }
   }

   if (IsControllerFinished()) {
      exit(EXIT_SUCCESS);
   }
}

/****************************************/

/****************************************/

void CEPuckCalibrator::Reset() {
}

/****************************************/

/****************************************/

CEPuckCalibrator::~CEPuckCalibrator() {
   std::string sMinValues;
   ticpp::Element tMinValueElement("min_values");
   std::string sMaxValues;
   ticpp::Element tMaxValueElement("max_values");
   std::string sKValue;
   ticpp::Element tKValueElement("k_value");
   ticpp::Element tSensorElement;
   ticpp::Element tConfElement("low_level_configuration");
   tConfElement.LinkEndChild(&tSensorElement);
   ticpp::Document tSensorLowLevelConfig;
   tSensorLowLevelConfig.LinkEndChild(&tConfElement);

   // Light sensor
   if (m_pcLightSensor != NULL) {
      LOG << "Saving light sensor data of " << m_nNumSamples << " calibration steps to " << m_sLightConfFile << " ..." << std::endl;

      sMinValues = ToString<Real > (m_pfLightMinValues[0]);
      for (UInt32 i = 1; i < 8; ++i)
         sMinValues += "," + ToString<Real > (m_pfLightMinValues[i]);
      LOG << "  Base values: " << sMinValues << std::endl;

      // clean up existing min/max value elements
      if (tMinValueElement.Parent(false) != NULL)
         tSensorElement.RemoveChild(&tMinValueElement);
      if (tMaxValueElement.Parent(false) != NULL)
         tSensorElement.RemoveChild(&tMaxValueElement);

      // construct proper tree for this sensor and write to file
      tSensorElement.SetValue("light_sensors");
      tMinValueElement.SetValue("base_values");
      tMinValueElement.SetText(sMinValues);
      tSensorElement.LinkEndChild(&tMinValueElement);
      tSensorLowLevelConfig.SaveFile(m_sLightConfFile);
      LOG << "Done." << std::endl;
   }

   // Proximity sensor
   if (m_pcProximitySensor != NULL) {
      LOG << "Saving proximity sensor data of " << m_nNumSamples << " calibration steps to " << m_sProximityConfFile << " ..." << std::endl;

      for (UInt32 i = 0; i < 8; ++i) {
         m_pfProximityMinValues[i] /= (Real) m_nNumSamples;
      }
      sMinValues = ToString<Real > (m_pfProximityMinValues[0]);
      sMaxValues = ToString<Real > (m_pfProximityMaxValues[0]);
      for(UInt32 i = 1; i < 8; ++i) {

         sMinValues += "," + ToString<Real > (m_pfProximityMinValues[i]);
         sMaxValues += "," + ToString<Real > (m_pfProximityMaxValues[i]);
         if (m_pfProximityMinValues[i] >= m_pfProximityMaxValues[i]) {
            LOGERR << "Error: sensor " << i << " min >= max. Either this sensor is dead or the low-level "
                   << "calibration is off! Do NOT use this sensor!" << std::endl;
         }
      }
      LOG << "  Base values: " << sMinValues << std::endl;
      LOG << "  Max values: " << sMaxValues << std::endl;


      // clean up existing min/max value elements
      if (tMinValueElement.Parent(false) != NULL)
         tSensorElement.RemoveChild(&tMinValueElement);
      if (tMaxValueElement.Parent(false) != NULL)
         tSensorElement.RemoveChild(&tMaxValueElement);

      // construct proper tree for this sensor and write to file
      tSensorElement.SetValue("proximity_sensors");
      tMinValueElement.SetValue("base_values");
      tMinValueElement.SetText(sMinValues);
      tSensorElement.LinkEndChild(&tMinValueElement);
      tMaxValueElement.SetValue("max_values");
      tMaxValueElement.SetText(sMaxValues);
      tSensorElement.LinkEndChild(&tMaxValueElement);
      tSensorLowLevelConfig.SaveFile(m_sProximityConfFile);
      LOG << "Done." << std::endl;

   }

   // Ground sensor
   if (m_pcGroundSensor != NULL) {
      LOG << "Saving ground sensor data of " << m_nNumSamples << " calibration steps to " << m_sGroundConfFile << " ..." << std::endl;

      for (UInt32 i = 0; i < 3; ++i) {
         m_pfGroundMinValues[i] /= (Real) m_nNumSamples;
         m_pfGroundMaxValues[i] /= (Real) m_nNumSamples;
      }
      sMinValues = ToString<Real > (m_pfGroundMinValues[0]);
      sMaxValues = ToString<Real > (m_pfGroundMaxValues[0]);
      if (m_pfGroundMinValues[0] >= m_pfGroundMaxValues[0]) {
         LOGERR << "Error: sensor 0 min >= max. Either this sensor is dead or the low-level "
                << "calibration is off! Do NOT use this sensor!" << std::endl;
      }
      for (UInt32 i = 1; i < 3; ++i) {
         sMinValues += "," + ToString<Real > (m_pfGroundMinValues[i]);
         sMaxValues += "," + ToString<Real > (m_pfGroundMaxValues[i]);

         if (m_pfGroundMinValues[i] >= m_pfGroundMaxValues[i]) {
            LOGERR << "Error: sensor " << i << " min >= max. Either this sensor is dead or the low-level "
                   << "calibration is off! Do NOT use this sensor!" << std::endl;
         }
      }
      LOG << "  Min values: " << sMinValues << std::endl;
      LOG << "  Max values: " << sMaxValues << std::endl;

      // clean up existing min/max value elements
      if (tMinValueElement.Parent(false) != NULL)
         tSensorElement.RemoveChild(&tMinValueElement);
      if (tMaxValueElement.Parent(false) != NULL)
         tSensorElement.RemoveChild(&tMaxValueElement);

      // construct proper tree for this sensor and write to file
      tSensorElement.SetValue("ground_sensors");
      tMinValueElement.SetValue("min_values");
      tMinValueElement.SetText(sMinValues);
      tSensorElement.LinkEndChild(&tMinValueElement);
      tMaxValueElement.SetValue("max_values");
      tMaxValueElement.SetText(sMaxValues);
      tSensorElement.LinkEndChild(&tMaxValueElement);
      tSensorLowLevelConfig.SaveFile(m_sGroundConfFile);
      LOG << "Done." << std::endl;
   }

   // Range and bearing sensor
   if (m_pcRangeAndBearingSensor != NULL) {
      LOG << "Saving range and bearing sensor data of " << m_nCurSamples << " calibration steps to " << m_sRangeAndBearingConfFile << " ..." << std::endl;

      m_pfRangeAndBearingKValue /= (Real) m_nCurSamples;
      
      //m_pfRangeAndBearingKValue = 20.0 / m_pfRangeAndBearingKValue;
      m_pfRangeAndBearingKValue = 20.0 / exp(m_fExpA + m_fExpB * m_pfRangeAndBearingKValue);
      sKValue = ToString<Real > (m_pfRangeAndBearingKValue);

      LOG << "  K value: " << sKValue << std::endl;

      // clean up existing k value elements
      if (tKValueElement.Parent(false) != NULL)
         tSensorElement.RemoveChild(&tKValueElement);

      // construct proper tree for this sensor and write to file
      tSensorElement.SetValue("range_and_bearing_sensor");
      tKValueElement.SetValue("k_value");
      tKValueElement.SetText(sKValue);
      tSensorElement.LinkEndChild(&tKValueElement);
      tSensorLowLevelConfig.SaveFile(m_sRangeAndBearingConfFile);


      LOG << "SLICES" << std::endl;
      m_bRangeAndBearingKAcquired = true;
      m_nCurSamples = -m_nIgnoreSamples;
      for (UInt32 i = 0; i < angleRanges.size(); i++) {

         LOG << "FINISHED PACKET in range " << angleRanges[i] << ": " << angleRangesCount[i] << "\n";


      }
      LOG << "Done." << std::endl;
   }

   delete [] m_pfLightMinValues;
   delete [] m_pfProximityMinValues;
   delete [] m_pfProximityMaxValues;
   delete [] m_pfGroundMinValues;
   delete [] m_pfGroundMaxValues;
}

/****************************************/
/****************************************/

/*
 * To allow dynamic loading of this controller
 */
REGISTER_CONTROLLER(CEPuckCalibrator, "calibrate_epuck")

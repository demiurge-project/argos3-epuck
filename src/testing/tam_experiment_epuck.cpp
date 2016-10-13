#include "tam_experiment_epuck.h"
#include <stdlib.h>
#include <sys/time.h>
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_base_leds_actuator.h>

using namespace argos;

/****************************************/
/****************************************/

CEPuckController::CEPuckController() :
   m_uRobotID(0),
   m_bChangingDirection(false),
   m_sLeftWheelSpeed(0),
   m_sRightWheelSpeed(0),
   m_nControlStep(0),
   m_pcWheelsActuator(NULL),
   m_pcIRComActuator(NULL),
   m_pcProximitySensor(NULL),
   m_pcIRComSensor(NULL),
   m_pcOmniCamera(NULL){
}

/****************************************/
/****************************************/

CEPuckController::~CEPuckController() {}

/****************************************/
/****************************************/

void CEPuckController::ParseParams(TConfigurationNode& t_node) {
   try {
      /* Left wheel speed */
      GetNodeAttributeOrDefault(t_node, "LWS", m_sLeftWheelSpeed, m_sLeftWheelSpeed);
      /* Right wheel speed */
      GetNodeAttributeOrDefault(t_node, "RWS", m_sRightWheelSpeed, m_sRightWheelSpeed);
      LOG << "[PARAMS] LWS: " << m_sLeftWheelSpeed << " | RWS: " << m_sRightWheelSpeed << std::endl;
   } catch (CARGoSException& ex) {
      THROW_ARGOSEXCEPTION_NESTED("Error parsing <params>", ex);
   }
}

/****************************************/
/****************************************/

void CEPuckController::Init(TConfigurationNode& t_node) {
   /* parse the xml tree <params> */
   ParseParams(t_node);
   m_uRobotID = atoi(GetId().substr(5, 6).c_str());
   /* actuators */
   try {
      m_pcWheelsActuator = GetActuator<CCI_EPuckWheelsActuator>("epuck_wheels");
   } catch (CARGoSException ex) {}
   try {
      m_pcIRComActuator = GetActuator<CCI_EPuckIRComActuator>("epuck_ircom");
   } catch (CARGoSException ex) {}
   /* sensors */
   try {
      m_pcProximitySensor = GetSensor<CCI_EPuckProximitySensor>("epuck_proximity");
   } catch (CARGoSException ex) {}
   try {
      m_pcIRComSensor = GetSensor<CCI_EPuckIRComSensor>("epuck_ircom");
   } catch (CARGoSException ex) {}
   try {
       m_pcOmniCamera = GetSensor<CCI_EPuckOmnidirectionalCameraSensor>("epuck_omnidirectional_camera");
       m_pcOmniCamera->Enable();
   } catch (CARGoSException ex) {}
}

/****************************************/
/****************************************/

bool SmthgAround(CCI_EPuckProximitySensor* proximity_sensor) {
   if (proximity_sensor != NULL)
      for(size_t i = 0; i < 8; ++i)
         if (proximity_sensor->GetReading(i).Value > 0)
            return true;
   return false;
}

/****************************************/
/****************************************/

void CEPuckController::ControlStep() {
   /* increment the control step count */
   ++m_nControlStep;
   if (m_pcWheelsActuator != NULL) {
      if (SmthgAround(m_pcProximitySensor)) {
         if (!m_bChangingDirection) {
            m_sLeftWheelSpeed *= -1;
            m_sRightWheelSpeed *= -1;
            m_bChangingDirection = !m_bChangingDirection;
         }
      } else {
         m_bChangingDirection = false;
      }
      /* make the robot turn clockwise on itself */
      m_pcWheelsActuator->SetLinearVelocity(m_sLeftWheelSpeed, m_sRightWheelSpeed);
   }
   if (m_pcIRComActuator != NULL) {
      /* send the current id of the robot with IRCom */
      m_pcIRComActuator->SetData(m_uRobotID);
   }
   if (m_pcIRComSensor != NULL) {
      printf("[IRCom]\t\t");
      const CCI_EPuckIRComSensor::TPackets& packets = m_pcIRComSensor->GetPackets();
      for (size_t i = 0; i < packets.size(); ++i) {
         printf("(%f, %.2f, %i, %i) ",
                ToDegrees(packets[i]->Bearing).GetValue(),
                packets[i]->Range,
                packets[i]->Sensor,
                packets[i]->Data);
      }
      m_pcIRComSensor->ClearPackets();
      printf("\n");
   }
   if (m_pcOmniCamera != NULL) {
       /* prints the blobs */
       const CCI_EPuckOmnidirectionalCameraSensor::SReadings* m_sCameraReadings = &m_pcOmniCamera->GetReadings();
       std::vector<CCI_EPuckOmnidirectionalCameraSensor::SBlob*> allBlobs = m_sCameraReadings->BlobList;
       LOG << "CAMERA CYCLES " << m_sCameraReadings->Counter << "" << std::endl;
       LOG << "Found " << allBlobs.size() << " blobs." << std::endl;
       if(allBlobs.size() == 0){
           LOG<< "NO BLOB FOUND" << std::endl;
       }else{
           UInt32 max_area=0;
           UInt32 led_on=0;
           for (std::vector<CCI_EPuckOmnidirectionalCameraSensor::SBlob*>::iterator it = allBlobs.begin(); it != allBlobs.end(); ++it) {
               if(max_area<(*it)->Area){
                   led_on=(*it)->Angle.UnsignedNormalize().GetValue() * 8.0f/CRadians::TWO_PI.GetValue();
                   max_area=(*it)->Area;
               }
               LOG<< "Color: " << (*it)->Color << std::endl;
               LOG<< "Distance: " << (*it)->Distance << std::endl;
               LOG<< "Area: " << (*it)->Area<< std::endl;
               LOG<< "Angle: " << (*it)->Angle.UnsignedNormalize().GetValue() << std::endl;
               LOG<< "---------------------------------------------------------------" << std::endl;
           }
       }
   }
}
/****************************************/
/****************************************/

REGISTER_CONTROLLER(CEPuckController, "tam_epuck_ctrlr");

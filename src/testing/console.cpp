#include <stdlib.h>

#include "console.h"
#include <sys/time.h>
#include <iostream>
#include <argos3/core/utility/logging/argos_log.h>

using namespace argos;

/****************************************/
/****************************************/

CConsoleController::CConsoleController() :
   m_pcWheelsActuator(NULL),
   m_pcLEDsActuator(NULL),
   m_pcRGBLEDsActuator(NULL),
   m_pcRABActuator(NULL),
   m_pcIRComActuator(NULL),
   m_pcProximitySensor(NULL),
   m_pcLightSensor(NULL),
   m_pcGroundSensor(NULL),
   m_pcRABSensor(NULL),
   m_pcIRComSensor(NULL) {
}

/****************************************/
/****************************************/

CConsoleController::~CConsoleController() {
   LOG << "END of controller" << std::endl;
}

/****************************************/
/****************************************/

void CConsoleController::Init(TConfigurationNode& t_node) {

   m_uRobotID = atoi(GetId().substr(5, 6).c_str());

   /* actuators */
   try {
      m_pcWheelsActuator = GetActuator<CCI_EPuckWheelsActuator>("epuck_wheels");
   } catch (CARGoSException ex) {}
   try {
      m_pcLEDsActuator = GetActuator<CCI_EPuckBaseLEDsActuator>("epuck_base_leds");
   } catch (CARGoSException ex) {}
   try {
      m_pcRGBLEDsActuator = GetActuator<CCI_EPuckRGBLEDsActuator>("epuck_rgb_leds");
   } catch (CARGoSException ex) {}
   try {
      m_pcRABActuator = GetActuator<CCI_EPuckRangeAndBearingActuator>("epuck_range_and_bearing");
   } catch (CARGoSException ex) {}
   try {
      m_pcIRComActuator = GetActuator<CCI_EPuckIRComActuator>("epuck_ircom");
   } catch (CARGoSException ex) {}

   /* sensors */
   try {
      m_pcProximitySensor = GetSensor<CCI_EPuckProximitySensor>("epuck_proximity");
   } catch (CARGoSException ex) {}
   try {
      m_pcLightSensor = GetSensor<CCI_EPuckLightSensor>("epuck_light");
   } catch (CARGoSException ex) {}
   try {
      m_pcGroundSensor = GetSensor<CCI_EPuckGroundSensor>("epuck_ground");
   } catch (CARGoSException ex) {}
   try {
      m_pcRABSensor = GetSensor<CCI_EPuckRangeAndBearingSensor>("epuck_range_and_bearing");
   } catch (CARGoSException ex) {}
   try {
      m_pcIRComSensor = GetSensor<CCI_EPuckIRComSensor>("epuck_ircom");
   } catch (CARGoSException ex) {}


   /* For the connection with the server */
   client.setAllActuators(m_pcWheelsActuator, m_pcLEDsActuator, m_pcRGBLEDsActuator, m_pcRABActuator);
   client.setAllSensors(m_pcProximitySensor, m_pcLightSensor, m_pcGroundSensor, m_pcRABSensor);
   
   std::string IP;
   GetNodeAttribute(t_node, "IP", IP);
   int PORT;
   GetNodeAttribute(t_node, "PORT", PORT);
   int epuckId = m_uRobotID;
   client.init(IP, PORT, epuckId);
}

/****************************************/
/****************************************/

void CConsoleController::ControlStep() {

   //Put your code here

   client.sendAndRcv();
}

/****************************************/
/****************************************/

REGISTER_CONTROLLER(CConsoleController, "console_controller");

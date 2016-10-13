/**
 * @file <argos3/plugins/robots/e-puck/simulator/epuck_accelerometer_default_sensor.cpp>
 *
 * @author Xavier Deschuyteneer - <xavier.deschuyteneer@gmail.com>
 */

#include "epuck_accelerometer_default_sensor.h"

#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/composable_entity.h>

namespace argos {
   CEPuckAccelerometerDefaultSensor::CEPuckAccelerometerDefaultSensor() {
   }

   CEPuckAccelerometerDefaultSensor::~CEPuckAccelerometerDefaultSensor() {
   }

   void CEPuckAccelerometerDefaultSensor::SetRobot(CComposableEntity& c_entity) {

      try {
         m_pcEmbodiedEntity = &(c_entity.GetComponent<CEmbodiedEntity>("body"));
         m_pcControllableEntity = &(c_entity.GetComponent<CControllableEntity>("controller"));
         m_pcAcceleratorEntity = &(c_entity.GetComponent<CAcceleratorSensorEquippedEntity>("accelerator_sensor"));
         m_pcAcceleratorEntity->SetCanBeEnabledIfDisabled(true);
         m_pcAcceleratorEntity->Enable();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Can't set robot for the e-puck accelerometer default sensor", ex);
      }
   }

   void CEPuckAccelerometerDefaultSensor::Init(TConfigurationNode& t_tree) {
   }

   void CEPuckAccelerometerDefaultSensor::Update() {
   }

   void CEPuckAccelerometerDefaultSensor::Reset() {
   }

   REGISTER_SENSOR(CEPuckAccelerometerDefaultSensor,
                   "epuck_accelerometer", "default",
                   "Xavier Deschuyteneer [xavier.deschuyteneer@gmail.com]",
                   "1.0",
                   "The e-puck accelerometer sensor.",
                   "This sensor allow to measure the linear acceleration of the e-puck\n"
                   "For use, refer to [ci_epuck_accelerometer_sensor.h]\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "[...]\n"
                   "   <controllers>\n"
                   "      [...]\n"
                   "      <sensors>"
                   "         [...]\n"
                   "         <epuck_accelerometer implementation=\"default\"/>\n"
                   "         [...]\n"
                   "      <sensors/>"
                   "      [...]\n"
                   "   <controllers>\n"
                   "[...]\n",
                   "Usable"
                   );
}

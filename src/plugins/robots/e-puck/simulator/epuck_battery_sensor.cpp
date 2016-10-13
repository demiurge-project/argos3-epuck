/**
 * @file <argos3/plugins/robots/e-puck/simulator/e-puck_battery_default_sensor.cpp>
 *
 */

#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/plugins/robots/e-puck/simulator/battery_equipped_entity.h>

#include "epuck_battery_sensor.h"

namespace argos {

   /****************************************/
   /****************************************/

   /****************************************/
   /****************************************/

   CEPuckBatteryDefaultSensor::CEPuckBatteryDefaultSensor()
      {
   }

   /****************************************/
   /****************************************/

   void CEPuckBatteryDefaultSensor::Init(TConfigurationNode& t_tree) {
         CCI_EPuckBatterySensor::Init(t_tree);

   }

   /****************************************/
   /****************************************/

   void CEPuckBatteryDefaultSensor::SetRobot(CComposableEntity& c_entity) {
      try {
          m_pcBatteryEntity = &(c_entity.GetComponent<CBatteryEquippedEntity>("battery"));
         m_pcBatteryEntity->Enable();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Can't set robot for the e-puck battery default sensor", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CEPuckBatteryDefaultSensor::Update() {
        m_fBatteryLevel=m_pcBatteryEntity->BatteryLevel();
   }

   /****************************************/
   /****************************************/

   void CEPuckBatteryDefaultSensor::Reset() {

   }

   /****************************************/
   /****************************************/

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CEPuckBatteryDefaultSensor,
                   "epuck_battery", "default",
                   "Gianpiero Francesca [gianpiero.francesca@ulb.ac.be]"
                   "Lorenzo Garattoni   [lgaratto@ulb.ac.be]",
                   "1.0",
                   "The e-puck battery sensor",
                   "The sensor returns a value\n"
                   "between 0 and 1, where 0 means empty battery and 1 full battery.\n"
                   "For usage, refer to [ci_epuck_battery_sensor.h]\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "   <controllers>\n"
                   "      ...\n"
                   "      <my_controller>\n"
                   "         ...\n"
                   "         <sensors>\n"
                   "            ...\n"
                   "            <epuck_battery implementation=\"default\"/>\n"
                   "            ...\n"
                   "         <sensors/>\n"
                   "         ...\n"
                   "      <my_controller/>\n"
                   "      ...\n"
                   "   <controllers>\n\n"
                   ,"Usable"
      );
}

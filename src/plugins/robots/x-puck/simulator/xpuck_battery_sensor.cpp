/**
 * @file <argos3/plugins/robots/x-puck/simulator/x-puck_battery_default_sensor.cpp>
 *
 */

#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/plugins/robots/x-puck/simulator/battery_equipped_entity.h>

#include "xpuck_battery_sensor.h"

namespace argos {

   /****************************************/
   /****************************************/

   /****************************************/
   /****************************************/

   CXPuckBatteryDefaultSensor::CXPuckBatteryDefaultSensor()
      {
   }

   /****************************************/
   /****************************************/

   void CXPuckBatteryDefaultSensor::Init(TConfigurationNode& t_tree) {
         CCI_XPuckBatterySensor::Init(t_tree);

   }

   /****************************************/
   /****************************************/

   void CXPuckBatteryDefaultSensor::SetRobot(CComposableEntity& c_entity) {
      try {
          m_pcBatteryEntity = &(c_entity.GetComponent<CBatteryEquippedEntity>("battery"));
         m_pcBatteryEntity->Enable();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Can't set robot for the x-puck battery default sensor", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CXPuckBatteryDefaultSensor::Update() {
        m_fBatteryLevel=m_pcBatteryEntity->BatteryLevel();
   }

   /****************************************/
   /****************************************/

   void CXPuckBatteryDefaultSensor::Reset() {

   }

   /****************************************/
   /****************************************/

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CXPuckBatteryDefaultSensor,
                   "xpuck_battery", "default",
                   "Gianpiero Francesca [gianpiero.francesca@ulb.ac.be]"
                   "Lorenzo Garattoni   [lgaratto@ulb.ac.be]",
                   "1.0",
                   "The x-puck battery sensor",
                   "The sensor returns a value\n"
                   "between 0 and 1, where 0 means empty battery and 1 full battery.\n"
                   "For usage, refer to [ci_xpuck_battery_sensor.h]\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "   <controllers>\n"
                   "      ...\n"
                   "      <my_controller>\n"
                   "         ...\n"
                   "         <sensors>\n"
                   "            ...\n"
                   "            <xpuck_battery implementation=\"default\"/>\n"
                   "            ...\n"
                   "         <sensors/>\n"
                   "         ...\n"
                   "      <my_controller/>\n"
                   "      ...\n"
                   "   <controllers>\n\n"
                   ,"Usable"
      );
}

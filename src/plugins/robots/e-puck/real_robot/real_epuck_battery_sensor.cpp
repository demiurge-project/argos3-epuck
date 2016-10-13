/**
 * @file <argos3/plugins/robots/e-puck/real_robot/real_epuck_battery_sensor.cpp>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 * @author Matteo Amaducci <matteo.amaducci2@studio.unibo.it>
 */

#include "real_epuck_battery_sensor.h"
#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

/****************************************/
/****************************************/

CRealEPuckBatterySensor::CRealEPuckBatterySensor() {
}

/****************************************/
/****************************************/

void CRealEPuckBatterySensor::UpdateValues() {
    m_fBatteryLevel=m_psState->Battery_LOW;
}

/****************************************/
/****************************************/

void CRealEPuckBatterySensor::Init(TConfigurationNode& t_node) {
}
/****************************************/
/****************************************/
}

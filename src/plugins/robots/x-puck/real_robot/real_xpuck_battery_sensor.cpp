/**
 * @file <argos3/plugins/robots/x-puck/real_robot/real_xpuck_battery_sensor.cpp>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 * @author Matteo Amaducci <matteo.amaducci2@studio.unibo.it>
 */

#include "real_xpuck_battery_sensor.h"
#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

/****************************************/
/****************************************/

CRealXPuckBatterySensor::CRealXPuckBatterySensor() {
}

/****************************************/
/****************************************/

void CRealXPuckBatterySensor::UpdateValues() {
    m_fBatteryLevel=m_psState->Battery_LOW;
}

/****************************************/
/****************************************/

void CRealXPuckBatterySensor::Init(TConfigurationNode& t_node) {
}
/****************************************/
/****************************************/
}

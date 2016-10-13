/**
 * @file <argos3/plugins/robots/e-puck/real_robot/real_epuck_battery_sensor.h>
 *
  *
 * @author Gianpiero Francesca <gianpiero.francesca@ulb.ac.be>
 * @author Lorenzo Garattoni <lgaratto@ulb.ac.be>
 */

#ifndef REAL_EPUCK_BATTERY_SENSOR_H
#define REAL_EPUCK_BATTERY_SENSOR_H

namespace argos {
class CRealEPuckBatterySensor;
}

#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_battery_sensor.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_serial_sensor.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_base.h>
#include <argos3/core/utility/math/angles.h>
#include <argos3/core/utility/math/range.h>

namespace argos {

class CRealEPuckBatterySensor : virtual public CCI_EPuckBatterySensor,
        virtual public CRealEPuckSerialSensor {

public:

    CRealEPuckBatterySensor();
    virtual ~CRealEPuckBatterySensor() {}

    virtual void Init(TConfigurationNode& t_node);
    virtual void UpdateValues();


};
}
#endif

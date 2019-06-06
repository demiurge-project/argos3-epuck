/**
 * @file <argos3/plugins/robots/x-puck/real_robot/real_xpuck_battery_sensor.h>
 *
  *
 * @author Gianpiero Francesca <gianpiero.francesca@ulb.ac.be>
 * @author Lorenzo Garattoni <lgaratto@ulb.ac.be>
 */

#ifndef REAL_XPUCK_BATTERY_SENSOR_H
#define REAL_XPUCK_BATTERY_SENSOR_H

namespace argos {
class CRealXPuckBatterySensor;
}

#include <argos3/plugins/robots/x-puck/control_interface/ci_xpuck_battery_sensor.h>
#include <argos3/plugins/robots/x-puck/real_robot/real_xpuck_serial_sensor.h>
#include <argos3/plugins/robots/x-puck/real_robot/real_xpuck_base.h>
#include <argos3/core/utility/math/angles.h>
#include <argos3/core/utility/math/range.h>

namespace argos {

class CRealXPuckBatterySensor : virtual public CCI_XPuckBatterySensor,
        virtual public CRealXPuckSerialSensor {

public:

    CRealXPuckBatterySensor();
    virtual ~CRealXPuckBatterySensor() {}

    virtual void Init(TConfigurationNode& t_node);
    virtual void UpdateValues();


};
}
#endif

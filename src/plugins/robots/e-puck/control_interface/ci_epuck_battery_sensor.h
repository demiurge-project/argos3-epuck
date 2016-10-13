/**
 * @file <argos3/plugins/robots/e-puck/control_interface/ci_epuck_battery_sensor.cpp
 *
 * @author Lorenzo Garattoni- <lgaratto@ulb.ac.be>
 */

#ifndef CCI_EPUCK_BATTERY_SENSOR_H
#define CCI_EPUCK_BATTERY_SENSOR_H

namespace argos {
class CCI_EPuckBatterySensor;
}

#include <argos3/core/control_interface/ci_sensor.h>

namespace argos {

class CCI_EPuckBatterySensor : virtual public CCI_Sensor {

protected:

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

public:

    /**
       * Constructor
       */
    CCI_EPuckBatterySensor(): m_fBatteryLevel(1) {
    }
    Real GetBatteryLevel(){
        return m_fBatteryLevel;
    }

    /**
       * Destructor
       */
    virtual ~CCI_EPuckBatterySensor() {}
protected:
    Real m_fBatteryLevel;
};
}
#endif

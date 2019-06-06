/**
 * @file <argos3/plugins/robots/x-puck/control_interface/ci_xpuck_battery_sensor.cpp
 *
 * @author Lorenzo Garattoni- <lgaratto@ulb.ac.be>
 */

#ifndef CCI_XPUCK_BATTERY_SENSOR_H
#define CCI_XPUCK_BATTERY_SENSOR_H

namespace argos {
class CCI_XPuckBatterySensor;
}

#include <argos3/core/control_interface/ci_sensor.h>

namespace argos {

class CCI_XPuckBatterySensor : virtual public CCI_Sensor {

protected:

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

public:

    /**
       * Constructor
       */
    CCI_XPuckBatterySensor(): m_fBatteryLevel(1) {
    }
    Real GetBatteryLevel(){
        return m_fBatteryLevel;
    }

    /**
       * Destructor
       */
    virtual ~CCI_XPuckBatterySensor() {}
protected:
    Real m_fBatteryLevel;
};
}
#endif

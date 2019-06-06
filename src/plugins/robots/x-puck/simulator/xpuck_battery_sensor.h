/**
 * @file <argos3/plugins/robots/x-puck/simulator/x-puck_battery_default_sensor.h>
 *
 */

#ifndef XPUCK_BATTERY_DEFAULT_SENSOR_H
#define XPUCK_BATTERY_DEFAULT_SENSOR_H

namespace argos {
   class CXPuckBatteryDefaultSensor;
}

#include <argos3/plugins/robots/x-puck/control_interface/ci_xpuck_battery_sensor.h>
#include <argos3/core/utility/math/range.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>

namespace argos {

   class CXPuckBatteryDefaultSensor : public CSimulatedSensor,
                                        public CCI_XPuckBatterySensor {

   public:

      CXPuckBatteryDefaultSensor();

      virtual ~CXPuckBatteryDefaultSensor() {}

      virtual void SetRobot(CComposableEntity& c_entity);
      virtual void Init(TConfigurationNode& t_tree);
      virtual void Update();
      virtual void Reset();
   private:

      /** Reference to battery sensor equipped entity associated to this sensor */
      CBatteryEquippedEntity* m_pcBatteryEntity;

   };

}

#endif

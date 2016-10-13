/**
 * @file <argos3/plugins/robots/e-puck/simulator/e-puck_battery_default_sensor.h>
 *
 */

#ifndef EPUCK_BATTERY_DEFAULT_SENSOR_H
#define EPUCK_BATTERY_DEFAULT_SENSOR_H

namespace argos {
   class CEPuckBatteryDefaultSensor;
}

#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_battery_sensor.h>
#include <argos3/core/utility/math/range.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>

namespace argos {

   class CEPuckBatteryDefaultSensor : public CSimulatedSensor,
                                        public CCI_EPuckBatterySensor {

   public:

      CEPuckBatteryDefaultSensor();

      virtual ~CEPuckBatteryDefaultSensor() {}

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

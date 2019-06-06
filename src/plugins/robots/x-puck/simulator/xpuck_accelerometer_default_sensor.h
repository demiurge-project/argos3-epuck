#ifndef XPUCK_ACCELEROMETER_DEFAULT_SENSOR_H
#define XPUCK_ACCELEROMETER_DEFAULT_SENSOR_H

namespace argos {
   class CXPuckAccelerometerDefaultSensor;
}

#include <argos3/plugins/robots/x-puck/control_interface/ci_xpuck_accelerometer_sensor.h>
#include <argos3/core/utility/math/range.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>

namespace argos {
   class CXPuckAccelerometerDefaultSensor :
      public CSimulatedSensor,
      public CCI_XPuckAccelerometerSensor {

   public :
         CXPuckAccelerometerDefaultSensor();

         virtual ~CXPuckAccelerometerDefaultSensor();

         virtual void SetRobot(CComposableEntity& c_entity);

         virtual void Init(TConfigurationNode& t_tree);

         virtual void Update();

         virtual void Reset();

   private:
         /** Reference to embodied entity associated to this sensor */
         CEmbodiedEntity* m_pcEmbodiedEntity;

         /** Reference to light sensor equipped entity associated to this sensor */
         CAcceleratorSensorEquippedEntity* m_pcAcceleratorEntity;

         /** Reference to controllable entity associated to this sensor */
         CControllableEntity* m_pcControllableEntity;

         virtual void ReadingsToLuaState(lua_State* pt_lua_state);
      };
}
#endif

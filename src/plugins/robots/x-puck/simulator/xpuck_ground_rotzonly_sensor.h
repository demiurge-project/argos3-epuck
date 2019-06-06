/**
 * @file <argos3/plugins/robots/x-puck/simulator/xpuck_ground_rotzonly_sensor.h>
 *
 * @author Xavier Deschuyteneer - <xavier.deschuyteneer@gmail.com>
 */

#ifndef XPUCK_GROUND_ROTZONLY_SENSOR_H
#define XPUCK_GROUND_ROTZONLY_SENSOR_H

namespace argos {
   class CXPuckGroundRotZOnlySensor;
   class CGroundSensorEquippedEntity;
   class CFloorEntity;
}

#include <argos3/plugins/robots/x-puck/control_interface/ci_xpuck_ground_sensor.h>
#include <argos3/core/utility/math/range.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>

namespace argos {

   class CXPuckGroundRotZOnlySensor : public CSimulatedSensor,
                                      public CCI_XPuckGroundSensor {

   public:

      CXPuckGroundRotZOnlySensor();
      virtual ~CXPuckGroundRotZOnlySensor() {}
      virtual void SetRobot(CComposableEntity& c_entity);
      virtual void Init(TConfigurationNode& t_tree);
      virtual void Update();
      virtual void Reset();

   protected:

      /** Reference to the embodied entity associated to this sensor */
      CEmbodiedEntity* m_pcEmbodiedEntity;

      /** Reference to the floor entity */
      CFloorEntity* m_pcFloorEntity;

      /** Reference to the ground sensor equipped entity associated to this sensor */
      CGroundSensorEquippedEntity* m_pcGroundSensorEntity;

      /** Random number generator */
      CRandom::CRNG* m_pcRNG;

      /** Whether to add noise or not */
      bool m_bAddNoise;

      /** Noise range */
      CRange<Real> m_cNoiseRange;

      /** Reference to the space */
      CSpace& m_cSpace;
   };

}
#endif

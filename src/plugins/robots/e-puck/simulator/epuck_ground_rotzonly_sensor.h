/**
 * @file <argos3/plugins/robots/e-puck/simulator/epuck_ground_rotzonly_sensor.h>
 *
 * @author Xavier Deschuyteneer - <xavier.deschuyteneer@gmail.com>
 */

#ifndef EPUCK_GROUND_ROTZONLY_SENSOR_H
#define EPUCK_GROUND_ROTZONLY_SENSOR_H

namespace argos {
   class CEPuckGroundRotZOnlySensor;
   class CGroundSensorEquippedEntity;
   class CFloorEntity;
}

#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_ground_sensor.h>
#include <argos3/core/utility/math/range.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>

namespace argos {

   class CEPuckGroundRotZOnlySensor : public CSimulatedSensor,
                                      public CCI_EPuckGroundSensor {

   public:

      CEPuckGroundRotZOnlySensor();
      virtual ~CEPuckGroundRotZOnlySensor() {}
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

/**
 * @file <argos3/plugins/robots/e-puck/simulator/epuck_light_default_sensor.h>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#ifndef EPUCK_LIGHT_DEFAULT_SENSOR_H
#define EPUCK_LIGHT_DEFAULT_SENSOR_H

#include <string>
#include <map>

namespace argos {
   class CEPuckLightDefaultSensor;
   class CLightSensorEquippedEntity;
}

#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_light_sensor.h>
#include <argos3/core/utility/math/range.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>

namespace argos {

   class CEPuckLightDefaultSensor :
      public CSimulatedSensor,
      public CCI_EPuckLightSensor {

   public:

         CEPuckLightDefaultSensor();
         virtual ~CEPuckLightDefaultSensor() {};

         virtual void SetRobot(CComposableEntity& c_entity);
         virtual void Init(TConfigurationNode& t_tree);
         virtual void Update();
         virtual void Reset();

   private:

         CEmbodiedEntity* m_pcEmbodiedEntity;
         CLightSensorEquippedEntity* m_pcLightEntity;
         CControllableEntity* m_pcControllableEntity;
         bool m_bShowRays;
         CRandom::CRNG* m_pcRNG;
         bool m_bAddNoise;
         CRange<Real> m_cNoiseRange;
         CSpace& m_cSpace;
      };

}

#endif

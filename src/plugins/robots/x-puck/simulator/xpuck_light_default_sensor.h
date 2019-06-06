/**
 * @file <argos3/plugins/robots/x-puck/simulator/xpuck_light_default_sensor.h>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#ifndef XPUCK_LIGHT_DEFAULT_SENSOR_H
#define XPUCK_LIGHT_DEFAULT_SENSOR_H

#include <string>
#include <map>

namespace argos {
   class CXPuckLightDefaultSensor;
   class CLightSensorEquippedEntity;
}

#include <argos3/plugins/robots/x-puck/control_interface/ci_xpuck_light_sensor.h>
#include <argos3/core/utility/math/range.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>

namespace argos {

   class CXPuckLightDefaultSensor :
      public CSimulatedSensor,
      public CCI_XPuckLightSensor {

   public:

         CXPuckLightDefaultSensor();
         virtual ~CXPuckLightDefaultSensor() {};

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

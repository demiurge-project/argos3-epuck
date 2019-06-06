/**
 * @file <argos3/plugins/robots/x-puck/simulator/xpuck_rab_default_sensor.h>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */
#ifndef XPUCK_IRCOM_DEFAULT_SENSOR_H
#define XPUCK_IRCOM_DEFAULT_SENSOR_H

namespace argos {
   class CXPuckIRComDefaultSensor;
   class CXpuckRABEquippedEntity;
   class CControllableEntity;
   class CRABMedium;
}

#include <argos3/core/simulator/sensor.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/plugins/robots/x-puck/control_interface/ci_xpuck_ircom_sensor.h>

namespace argos {

   class CXPuckIRComDefaultSensor : public CSimulatedSensor,
                                    public CCI_XPuckIRComSensor {
   public:

      CXPuckIRComDefaultSensor();
      ~CXPuckIRComDefaultSensor() {}

      virtual void SetRobot(CComposableEntity& c_entity);
      virtual void Init(TConfigurationNode& t_tree);
      virtual void Update();
      virtual void Reset();
      virtual void Destroy();

   private:

      CXpuckRABEquippedEntity*  m_pcIRComEquippedEntity;
      CControllableEntity* m_pcControllableEntity;
      CRABMedium*          m_pcRangeAndBearingMedium;
      Real                 m_fDistanceNoiseStdDev;
      CRandom::CRNG*       m_pcRNG;
      CSpace&              m_cSpace;
      bool                 m_bShowRays;
   };
}
#endif

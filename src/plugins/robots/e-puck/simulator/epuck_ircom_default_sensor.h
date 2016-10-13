/**
 * @file <argos3/plugins/robots/e-puck/simulator/epuck_rab_default_sensor.h>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */
#ifndef EPUCK_IRCOM_DEFAULT_SENSOR_H
#define EPUCK_IRCOM_DEFAULT_SENSOR_H

namespace argos {
   class CEPuckIRComDefaultSensor;
   class CEpuckRABEquippedEntity;
   class CControllableEntity;
   class CRABMedium;
}

#include <argos3/core/simulator/sensor.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_ircom_sensor.h>

namespace argos {

   class CEPuckIRComDefaultSensor : public CSimulatedSensor,
                                    public CCI_EPuckIRComSensor {
   public:

      CEPuckIRComDefaultSensor();
      ~CEPuckIRComDefaultSensor() {}

      virtual void SetRobot(CComposableEntity& c_entity);
      virtual void Init(TConfigurationNode& t_tree);
      virtual void Update();
      virtual void Reset();
      virtual void Destroy();

   private:

      CEpuckRABEquippedEntity*  m_pcIRComEquippedEntity;
      CControllableEntity* m_pcControllableEntity;
      CRABMedium*          m_pcRangeAndBearingMedium;
      Real                 m_fDistanceNoiseStdDev;
      CRandom::CRNG*       m_pcRNG;
      CSpace&              m_cSpace;
      bool                 m_bShowRays;
   };
}
#endif

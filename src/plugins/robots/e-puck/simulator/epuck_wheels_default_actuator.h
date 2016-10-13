/**
 * @file <argos3/plugins/robots/e-puck/simulator/epuck_wheels_default_actuator.h>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */
#ifndef EPUCK_WHEELS_DEFAULT_ACTUATOR_H
#define EPUCK_WHEELS_DEFAULT_ACTUATOR_H

namespace argos {
   class CEPuckWheelsDefaultActuator;
}

#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_wheels_actuator.h>
#include <argos3/plugins/simulator/entities/wheeled_entity.h>
#include <argos3/core/simulator/actuator.h>
#include <argos3/core/utility/math/rng.h>

namespace argos {

   class CEPuckWheelsDefaultActuator : public CSimulatedActuator,
                                       public CCI_EPuckWheelsActuator {

   public:

      enum DIFFERENTIAL_STEERING {
         LEFT_WHEEL = 0,
         RIGHT_WHEEL = 1
      };

   public:

      CEPuckWheelsDefaultActuator();
      virtual ~CEPuckWheelsDefaultActuator() {}
      virtual void Init(TConfigurationNode& t_tree);
      virtual void SetRobot(CComposableEntity& c_entity);
      virtual void Update();
      virtual void Reset();

      virtual void SetLinearVelocity(Real f_left_velocity,
                                     Real f_right_velocity);

   private:

      virtual void AddGaussianNoise();

   private:

      CWheeledEntity* m_pcWheeledEntity;
      Real m_fCurrentVelocity[2];
      /** Random number generator */
      CRandom::CRNG* m_pcRNG;
      /** Noise parameters, at the moment noise is Gaussian */
      Real m_fNoiseStdDeviation;
   };
}

#endif

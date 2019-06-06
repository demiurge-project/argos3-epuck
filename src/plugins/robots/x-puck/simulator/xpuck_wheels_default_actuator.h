/**
 * @file <argos3/plugins/robots/x-puck/simulator/xpuck_wheels_default_actuator.h>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */
#ifndef XPUCK_WHEELS_DEFAULT_ACTUATOR_H
#define XPUCK_WHEELS_DEFAULT_ACTUATOR_H

namespace argos {
   class CXPuckWheelsDefaultActuator;
}

#include <argos3/plugins/robots/x-puck/control_interface/ci_xpuck_wheels_actuator.h>
#include <argos3/plugins/simulator/entities/wheeled_entity.h>
#include <argos3/core/simulator/actuator.h>
#include <argos3/core/utility/math/rng.h>

namespace argos {

   class CXPuckWheelsDefaultActuator : public CSimulatedActuator,
                                       public CCI_XPuckWheelsActuator {

   public:

      enum DIFFERENTIAL_STEERING {
         LEFT_WHEEL = 0,
         RIGHT_WHEEL = 1
      };

   public:

      CXPuckWheelsDefaultActuator();
      virtual ~CXPuckWheelsDefaultActuator() {}
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

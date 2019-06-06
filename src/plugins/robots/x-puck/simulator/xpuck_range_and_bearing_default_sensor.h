/**
 * @file <argos3/plugins/robots/x-puck/simulator/xpuck_rab_default_sensor.h>
 *
 * @author Gianpiero Francesca <gianpiero.francesca@ulb.ac.be>
 * @author Lorenzo Garattoni <lgaratto@ulb.ac.be>
 */
#ifndef XPUCK_RAB_DEFAULT_ACTUATOR_H
#define XPUCK_RAB_DEFAULT_ACTUATOR_H

#include <string>
#include <map>

namespace argos {
   class CRangeAndBearingMediumSensor;
   class CXpuckRABEquippedEntity;
   class CControllableEntity;
   class CRABMedium;
}

#include <argos3/core/simulator/sensor.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/plugins/robots/x-puck/simulator/xpuck_rab_equipped_entity.h>
#include <argos3/plugins/robots/x-puck/control_interface/ci_xpuck_range_and_bearing_sensor.h>

namespace argos {

   class CXPuckRABDefaultSensor : public CSimulatedSensor, public CCI_XPuckRangeAndBearingSensor {

   public:

      CXPuckRABDefaultSensor();
      virtual ~CXPuckRABDefaultSensor() {}
      virtual void SetRobot(CComposableEntity& c_entity);
      virtual void Init(TConfigurationNode& t_tree);
      virtual void
      Update();
      virtual void Reset();
      virtual void Destroy();

   private:

      CXpuckRABEquippedEntity*  m_pcRangeAndBearingEquippedEntity;
      CControllableEntity* m_pcControllableEntity;
      CRABMedium*          m_pcRangeAndBearingMedium;
      Real                 m_fDistanceNoiseStdDev;
      CRandom::CRNG*       m_pcRNG;
      CSpace&              m_cSpace;
      bool                 m_bShowRays;

      bool                 m_bRealRangeNoise;
      Real                 m_fLossProbability;
      UInt32               m_uMaxNumberOfMessages;
      std::vector<std::pair<Real,Real> > m_cMuValues,m_cSigmaValues;// The values of the conversion function for each distance.
      Real m_fExpA,m_fExpB; //The conversion between POWER (from the hw) and DISTANCE is done using the function DISTANCE(POWER)=exp(a+ b* POWER)

   };
}

#endif

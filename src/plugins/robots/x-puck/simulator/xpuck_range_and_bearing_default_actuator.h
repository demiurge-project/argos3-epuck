/**
 * @file <argos3/plugins/robots/x-puck/simulator/xpuck_range_and_bearing_default_actuator.h>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 *
 * Range and bearing default actuator implementation for the x-puck simulation
 */

#ifndef XPUCK_RAB_DEFAULT_ACTUATOR_H
#define XPUCK_RAB_DEFAULT_ACTUATOR_H

namespace argos {
   class CXPuckRABDefaultActuator;
}

#include <argos3/core/simulator/actuator.h>
#include <argos3/core/utility/datatypes/byte_array.h>
#include <argos3/plugins/robots/x-puck/simulator/xpuck_rab_equipped_entity.h>
#include <argos3/plugins/robots/x-puck/control_interface/ci_xpuck_range_and_bearing_actuator.h>

namespace argos {

   class CXPuckRABDefaultActuator : public CSimulatedActuator, public CCI_XPuckRangeAndBearingActuator {

   public:

      CXPuckRABDefaultActuator() {}
      virtual ~CXPuckRABDefaultActuator() {}


      virtual void SetRobot(CComposableEntity& c_entity);
      virtual void Update();
      virtual void Init(TConfigurationNode& t_tree);

   private:

      CXpuckRABEquippedEntity* m_pcRangeAndBearingEquippedEntity;

   };

}

#endif

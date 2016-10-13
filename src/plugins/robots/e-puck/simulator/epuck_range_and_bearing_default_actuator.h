/**
 * @file <argos3/plugins/robots/e-puck/simulator/epuck_range_and_bearing_default_actuator.h>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 *
 * Range and bearing default actuator implementation for the e-puck simulation
 */

#ifndef EPUCK_RAB_DEFAULT_ACTUATOR_H
#define EPUCK_RAB_DEFAULT_ACTUATOR_H

namespace argos {
   class CEPuckRABDefaultActuator;
}

#include <argos3/core/simulator/actuator.h>
#include <argos3/core/utility/datatypes/byte_array.h>
#include <argos3/plugins/robots/e-puck/simulator/epuck_rab_equipped_entity.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_range_and_bearing_actuator.h>

namespace argos {

   class CEPuckRABDefaultActuator : public CSimulatedActuator, public CCI_EPuckRangeAndBearingActuator {

   public:

      CEPuckRABDefaultActuator() {}
      virtual ~CEPuckRABDefaultActuator() {}


      virtual void SetRobot(CComposableEntity& c_entity);
      virtual void Update();
      virtual void Init(TConfigurationNode& t_tree);

   private:

      CEpuckRABEquippedEntity* m_pcRangeAndBearingEquippedEntity;

   };

}

#endif

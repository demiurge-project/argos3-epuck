/**
 * @file <argos3/plugins/robots/e-puck/simulator/epuck_base_leds_default_actuator.h>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#ifndef EPUCK_BASE_LEDS_DEFAULT_ACTUATOR_H
#define EPUCK_BASE_LEDS_DEFAULT_ACTUATOR_H

namespace argos {
   class CEPuckBaseLEDsDefaultActuator;
}

#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_base_leds_actuator.h>
#include <argos3/plugins/simulator/entities/led_equipped_entity.h>
#include <argos3/core/simulator/actuator.h>

namespace argos {

   class CEPuckBaseLEDsDefaultActuator : public CSimulatedActuator,
                                         public CCI_EPuckBaseLEDsActuator {

   public:

      CEPuckBaseLEDsDefaultActuator();

      virtual ~CEPuckBaseLEDsDefaultActuator();

      virtual void Init(TConfigurationNode& t_tree);
      virtual void SetRobot(CComposableEntity& c_entity);
      virtual void Update();
      virtual void Reset();

   private:

      CLEDEquippedEntity* m_pcLEDEquippedEntity;
      CLEDMedium*         m_pcLEDMedium;
   };
}

#endif

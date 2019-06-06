/**
 * @file <argos3/plugins/robots/x-puck/simulator/xpuck_base_leds_default_actuator.h>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#ifndef XPUCK_BASE_LEDS_DEFAULT_ACTUATOR_H
#define XPUCK_BASE_LEDS_DEFAULT_ACTUATOR_H

namespace argos {
   class CXPuckBaseLEDsDefaultActuator;
}

#include <argos3/plugins/robots/x-puck/control_interface/ci_xpuck_base_leds_actuator.h>
#include <argos3/plugins/simulator/entities/led_equipped_entity.h>
#include <argos3/core/simulator/actuator.h>

namespace argos {

   class CXPuckBaseLEDsDefaultActuator : public CSimulatedActuator,
                                         public CCI_XPuckBaseLEDsActuator {

   public:

      CXPuckBaseLEDsDefaultActuator();

      virtual ~CXPuckBaseLEDsDefaultActuator();

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

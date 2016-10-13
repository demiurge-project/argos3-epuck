/**
 * @file <argos3/plugins/robots/e-puck/simulator/epuck_rgb_leds_default_actuator.h>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */
#ifndef EPUCK_RGB_LEDS_DEFAULT_ACTUATOR_H
#define EPUCK_RGB_LEDS_DEFAULT_ACTUATOR_H

namespace argos {
   class CEPuckRGBLEDsDefaultActuator;
}

#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_rgb_leds_actuator.h>
#include <argos3/plugins/simulator/entities/led_equipped_entity.h>
#include <argos3/core/simulator/actuator.h>

namespace argos {

   class CEPuckRGBLEDsDefaultActuator : public CSimulatedActuator, public CCI_EPuckRGBLEDsActuator {

   public:

      CEPuckRGBLEDsDefaultActuator();
      virtual ~CEPuckRGBLEDsDefaultActuator();
      void Init(TConfigurationNode& t_tree);
      virtual void SetRobot(CComposableEntity& c_entity);
      virtual void Update();
      virtual void Reset();

   private:

      CLEDEquippedEntity* m_pcLEDEquippedEntity;
      CLEDMedium*         m_pcLEDMedium;
   };
}

#endif

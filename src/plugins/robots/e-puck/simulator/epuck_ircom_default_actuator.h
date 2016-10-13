/**
 * @file <argos3/plugins/robots/e-puck/simulator/epuck_ircom_default_actuator.h>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 *
 * ircom default actuator implementation for the e-puck simulation
 */
#ifndef EPUCK_IRCOM_DEFAULT_ACTUATOR_H
#define EPUCK_IRCOM_DEFAULT_ACTUATOR_H

namespace argos {
   class CEPuckIRComDefaultActuator;
}

#include <argos3/core/simulator/actuator.h>
#include <argos3/core/utility/datatypes/byte_array.h>
#include <argos3/plugins/robots/e-puck/simulator/epuck_rab_equipped_entity.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_ircom_actuator.h>

namespace argos {

   class CEPuckIRComDefaultActuator : public CSimulatedActuator, public CCI_EPuckIRComActuator {

   public:

      CEPuckIRComDefaultActuator() {}
      virtual ~CEPuckIRComDefaultActuator() {}


      virtual void SetRobot(CComposableEntity& c_entity);
      virtual void Update();
      virtual void Reset();
      virtual void Init(TConfigurationNode& t_tree);

      virtual void SetData(UInt8 un_data);
      virtual void SetEnabled(bool b_enabled);

   private:

      CEpuckRABEquippedEntity* m_pcRangeAndBearingEquippedEntity;
      CByteArray m_cData;

   };

}

#endif

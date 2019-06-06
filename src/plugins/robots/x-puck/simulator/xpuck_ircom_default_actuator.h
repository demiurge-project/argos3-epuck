/**
 * @file <argos3/plugins/robots/x-puck/simulator/xpuck_ircom_default_actuator.h>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 *
 * ircom default actuator implementation for the x-puck simulation
 */
#ifndef XPUCK_IRCOM_DEFAULT_ACTUATOR_H
#define XPUCK_IRCOM_DEFAULT_ACTUATOR_H

namespace argos {
   class CXPuckIRComDefaultActuator;
}

#include <argos3/core/simulator/actuator.h>
#include <argos3/core/utility/datatypes/byte_array.h>
#include <argos3/plugins/robots/x-puck/simulator/xpuck_rab_equipped_entity.h>
#include <argos3/plugins/robots/x-puck/control_interface/ci_xpuck_ircom_actuator.h>

namespace argos {

   class CXPuckIRComDefaultActuator : public CSimulatedActuator, public CCI_XPuckIRComActuator {

   public:

      CXPuckIRComDefaultActuator() {}
      virtual ~CXPuckIRComDefaultActuator() {}


      virtual void SetRobot(CComposableEntity& c_entity);
      virtual void Update();
      virtual void Reset();
      virtual void Init(TConfigurationNode& t_tree);

      virtual void SetData(UInt8 un_data);
      virtual void SetEnabled(bool b_enabled);

   private:

      CXpuckRABEquippedEntity* m_pcRangeAndBearingEquippedEntity;
      CByteArray m_cData;

   };

}

#endif

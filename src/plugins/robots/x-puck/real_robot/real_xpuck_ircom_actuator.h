/**
 * @file <argos3/plugins/robots/x-puck/real_robot/real_xpuck_ircom_actuator.h
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#ifndef REAL_XPUCK_IRCOM_ACTUATOR_H
#define REAL_XPUCK_IRCOM_ACTUATOR_H

namespace argos {
   class CRealXPuckIRComActuator;
}

#include <argos3/plugins/robots/x-puck/real_robot/real_xpuck_base.h>
#include <argos3/plugins/robots/x-puck/real_robot/real_xpuck_serial_actuator.h>
#include <argos3/plugins/robots/x-puck/control_interface/ci_xpuck_ircom_actuator.h>

namespace argos {

   class CRealXPuckIRComActuator : virtual public CCI_XPuckIRComActuator,
                                   virtual public CRealXPuckSerialActuator {

   public:

      virtual ~CRealXPuckIRComActuator() {}

      virtual void SetData(UInt8 un_data);
      virtual void SetEnabled(bool b_enabled);
   };
}
#endif

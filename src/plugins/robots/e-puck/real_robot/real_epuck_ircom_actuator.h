/**
 * @file <argos3/plugins/robots/e-puck/real_robot/real_epuck_ircom_actuator.h
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#ifndef REAL_EPUCK_IRCOM_ACTUATOR_H
#define REAL_EPUCK_IRCOM_ACTUATOR_H

namespace argos {
   class CRealEPuckIRComActuator;
}

#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_base.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_serial_actuator.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_ircom_actuator.h>

namespace argos {

   class CRealEPuckIRComActuator : virtual public CCI_EPuckIRComActuator,
                                   virtual public CRealEPuckSerialActuator {

   public:

      virtual ~CRealEPuckIRComActuator() {}

      virtual void SetData(UInt8 un_data);
      virtual void SetEnabled(bool b_enabled);
   };
}
#endif

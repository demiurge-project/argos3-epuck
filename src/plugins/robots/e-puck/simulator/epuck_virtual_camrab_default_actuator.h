/**
 * @file <argos3/plugins/robots/e-puck/simulator/epuck_virtual_camrab_default_actuator.h>
 *
 * @author Antoine Ligot - <aligot@ulb.ac.be>
 *
 * Virtual camrab default actuator implementation for the e-puck simulation.
 */

#ifndef EPUCK_VIRTUAL_CAMRAB_DEFAULT_ACTUATOR_H
#define EPUCK_VIRTUAL_CAMRAB_DEFAULT_ACTUATOR_H

namespace argos {
   class CEPuckVirtualCamrabDefaultActuator;
}

#include <argos3/core/simulator/actuator.h>
#include <argos3/core/utility/datatypes/byte_array.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/plugins/robots/e-puck/simulator/epuck_rab_equipped_entity.h>
#include <argos3/plugins/simulator/entities/led_equipped_entity.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_virtual_camrab_actuator.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_range_and_bearing_actuator.h>

namespace argos {

    class CEPuckVirtualCamrabDefaultActuator : public CSimulatedActuator, public CCI_EPuckVirtualCamrabActuator {

    public:

        CEPuckVirtualCamrabDefaultActuator() {}
        virtual ~CEPuckVirtualCamrabDefaultActuator() {}


        virtual void SetRobot(CComposableEntity& c_entity);
        virtual void Update();
        virtual void Init(TConfigurationNode& t_tree);

    private:
		/** Pointer to the range and bearing equipped entity */
        CEpuckRABEquippedEntity* m_pcRangeAndBearingEquippedEntity;
        
        /** Pointer to the led equipped entity */
        CLEDEquippedEntity* m_pcLEDEquippedEntity; 
    };
}

#endif

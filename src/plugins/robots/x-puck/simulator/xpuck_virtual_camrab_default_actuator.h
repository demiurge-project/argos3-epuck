/**
 * @file <argos3/plugins/robots/x-puck/simulator/xpuck_virtual_camrab_default_actuator.h>
 *
 * @author Antoine Ligot - <aligot@ulb.ac.be>
 *
 * Virtual camrab default actuator implementation for the x-puck simulation.
 */

#ifndef XPUCK_VIRTUAL_CAMRAB_DEFAULT_ACTUATOR_H
#define XPUCK_VIRTUAL_CAMRAB_DEFAULT_ACTUATOR_H

namespace argos {
   class CXPuckVirtualCamrabDefaultActuator;
}

#include <argos3/core/simulator/actuator.h>
#include <argos3/core/utility/datatypes/byte_array.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/plugins/robots/x-puck/simulator/xpuck_rab_equipped_entity.h>
#include <argos3/plugins/simulator/entities/led_equipped_entity.h>
#include <argos3/plugins/robots/x-puck/control_interface/ci_xpuck_virtual_camrab_actuator.h>
#include <argos3/plugins/robots/x-puck/control_interface/ci_xpuck_range_and_bearing_actuator.h>

namespace argos {

    class CXPuckVirtualCamrabDefaultActuator : public CSimulatedActuator, public CCI_XPuckVirtualCamrabActuator {

    public:

        CXPuckVirtualCamrabDefaultActuator() {}
        virtual ~CXPuckVirtualCamrabDefaultActuator() {}


        virtual void SetRobot(CComposableEntity& c_entity);
        virtual void Update();
        virtual void Init(TConfigurationNode& t_tree);

    private:
		/** Pointer to the range and bearing equipped entity */
        CXpuckRABEquippedEntity* m_pcRangeAndBearingEquippedEntity;
        
        /** Pointer to the led equipped entity */
        CLEDEquippedEntity* m_pcLEDEquippedEntity; 
    };
}

#endif

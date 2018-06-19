/**
 * @file <argos3/plugins/simulator/entities/battery_equipped_entity.h>
 *
 */

#ifndef EPUCK_Battery_EQUIPPED_ENTITY_H
#define EPUCK_Battery_EQUIPPED_ENTITY_H

#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/utility/math/vector3.h>

namespace argos {

class CEPuckBatteryEquippedEntity : public CEntity {
protected:
    Real m_fBatteryLevel;

public:

    ENABLE_VTABLE();

    CEPuckBatteryEquippedEntity(CComposableEntity* pc_parent);

    CEPuckBatteryEquippedEntity(CComposableEntity* pc_parent,
                           const std::string& str_id);

    virtual void Init(TConfigurationNode &t_tree){
        /*
           * Parse basic entity stuff
           */
        CEntity::Init(t_tree);
    }
    virtual std::string GetTypeDescription() const {
        return "epuck_battery";
    }

    Real BatteryLevel() const;
    void setBatteryLevel(const Real &fBatteryLevel);
};

}

#endif

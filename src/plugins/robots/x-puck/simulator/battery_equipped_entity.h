/**
 * @file <argos3/plugins/simulator/entities/battery_equipped_entity.h>
 *
 */

#ifndef Battery_EQUIPPED_ENTITY_H
#define Battery_EQUIPPED_ENTITY_H

#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/utility/math/vector3.h>

namespace argos {

class CBatteryEquippedEntity : public CEntity {
protected:
    Real m_fBatteryLevel;

public:

    ENABLE_VTABLE();

    CBatteryEquippedEntity(CComposableEntity* pc_parent);

    CBatteryEquippedEntity(CComposableEntity* pc_parent,
                           const std::string& str_id);

    virtual void Init(TConfigurationNode &t_tree){
        /*
           * Parse basic entity stuff
           */
        CEntity::Init(t_tree);
    }
    virtual std::string GetTypeDescription() const {
        return "battery";
    }

    Real BatteryLevel() const;
    void setBatteryLevel(const Real &fBatteryLevel);
};

}

#endif

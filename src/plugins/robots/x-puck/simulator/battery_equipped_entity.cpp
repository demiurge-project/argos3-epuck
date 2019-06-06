/**
 * @file <argos3/plugins/simulator/entities/battery_equipped_entity.cpp>
 *
 */

#include "battery_equipped_entity.h"
#include <argos3/core/simulator/space/space.h>

namespace argos {

/****************************************/
/****************************************/


Real CBatteryEquippedEntity::BatteryLevel() const
{
    return m_fBatteryLevel;
}

void CBatteryEquippedEntity::setBatteryLevel(const Real &fBatteryLevel)
{
    m_fBatteryLevel = fBatteryLevel;
}
CBatteryEquippedEntity::CBatteryEquippedEntity(CComposableEntity* pc_parent) :
    CEntity(pc_parent),
    m_fBatteryLevel(1){
}

/****************************************/
/****************************************/

CBatteryEquippedEntity::CBatteryEquippedEntity(CComposableEntity* pc_parent,
                                               const std::string& str_id) :
    CEntity(pc_parent, str_id),
    m_fBatteryLevel(1){
}

/****************************************/
/****************************************/

REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CBatteryEquippedEntity);

/****************************************/
/****************************************/

}

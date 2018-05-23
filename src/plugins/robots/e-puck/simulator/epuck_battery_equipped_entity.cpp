/**
 * @file <argos3/plugins/simulator/entities/battery_equipped_entity.cpp>
 *
 */

#include "epuck_battery_equipped_entity.h"
#include <argos3/core/simulator/space/space.h>

namespace argos {

/****************************************/
/****************************************/


Real CEPuckBatteryEquippedEntity::BatteryLevel() const
{
    return m_fBatteryLevel;
}

void CEPuckBatteryEquippedEntity::setBatteryLevel(const Real &fBatteryLevel)
{
    m_fBatteryLevel = fBatteryLevel;
}
CEPuckBatteryEquippedEntity::CEPuckBatteryEquippedEntity(CComposableEntity* pc_parent) :
    CEntity(pc_parent),
    m_fBatteryLevel(1){
}

/****************************************/
/****************************************/

CEPuckBatteryEquippedEntity::CEPuckBatteryEquippedEntity(CComposableEntity* pc_parent,
                                               const std::string& str_id) :
    CEntity(pc_parent, str_id),
    m_fBatteryLevel(1){
}

/****************************************/
/****************************************/

REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CEPuckBatteryEquippedEntity);

/****************************************/
/****************************************/

}

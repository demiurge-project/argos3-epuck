/**
 * @file <argos3/plugins/robots/x-puck/simulator/xpuck_rab_equipped_entity.h>
 *
 * @author Gianpiero Francesca <gianpiero.francesca@ulb.ac.be>
 * @author Lorenzo Garattoni <lgaratto@ulb.ac.be>
 */

#ifndef XPUCK_RAB_EQUIPPED_ENTITY_H
#define XPUCK_RAB_EQUIPPED_ENTITY_H

namespace argos {
class CXpuckRABEquippedEntity;
}

#include <argos3/core/utility/datatypes/byte_array.h>
#include <argos3/core/utility/datatypes/set.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/simulator/space/positional_indices/space_hash.h>
#include <argos3/core/simulator/space/positional_indices/grid.h>
#include <argos3/plugins/simulator/entities/rab_equipped_entity.h>
#include <argos3/plugins/robots/x-puck/control_interface/ci_xpuck_range_and_bearing_actuator.h>
namespace argos {

class CXpuckRABEquippedEntity : public CRABEquippedEntity {
public:

public:

    ENABLE_VTABLE();

    typedef std::vector<CXpuckRABEquippedEntity*> TVector;
    typedef CSet<CXpuckRABEquippedEntity*> TSet;
private:
    /** this is the data size */
    UInt8 m_uDataSize;
    CCI_XPuckRangeAndBearingActuator::SDataToSend m_cEmitterData;
    CEmbodiedEntity* m_pcEntityBody;
public:

    CXpuckRABEquippedEntity(CComposableEntity* pc_parent);

    CXpuckRABEquippedEntity(CComposableEntity* pc_parent,
                            const std::string& str_id,
                            size_t un_msg_size,
                            Real f_range,
                            SAnchor& s_anchor,
                            CEmbodiedEntity& c_entity_body,
                            const CVector3& c_position = CVector3(0,0,0),
                            const CQuaternion& c_orientation = CQuaternion().FromEulerAngles(CRadians::ZERO,CRadians::ZERO,CRadians::ZERO));


    virtual ~CXpuckRABEquippedEntity() {}

    virtual void Reset();

    //virtual void Update();

    inline size_t GetMsgSize() const {
        return m_uDataSize;
    }
    inline void SetDataSize(UInt8 uDataSize){
        m_uDataSize=uDataSize;
    }

    void SetDataToSend(const CCI_XPuckRangeAndBearingActuator::SDataToSend& c_data);

    void ClearData();

    inline Real GetRange() const {
        return m_fRange;
    }
    inline void SetRange(Real frange){
        m_fRange=frange;
    }
    inline CEmbodiedEntity& GetReference() {
        return *m_pcEntityBody;
    }

    virtual std::string GetTypeDescription() const {
        return "rab";
    }
    CCI_XPuckRangeAndBearingActuator::SEmitter& GetEmitterForAngle(const CRadians& t_angle);

};

/****************************************/
/****************************************/

class CXpuckRABEquippedEntitySpaceHashUpdater : public CSpaceHashUpdater<CXpuckRABEquippedEntity> {

public:

    virtual void operator()(CAbstractSpaceHash<CXpuckRABEquippedEntity>& c_space_hash,
                            CXpuckRABEquippedEntity& c_element);

private:

    SInt32 m_nCenterI, m_nCenterJ, m_nCenterK;

};

/****************************************/
/****************************************/

class CXpuckRABEquippedEntityGridCellUpdater : public CGrid<CXpuckRABEquippedEntity>::CCellOperation {

public:

    CXpuckRABEquippedEntityGridCellUpdater(CGrid<CXpuckRABEquippedEntity>& c_grid);

    virtual bool operator()(SInt32 n_i,
                            SInt32 n_j,
                            SInt32 n_k,
                            CGrid<CXpuckRABEquippedEntity>::SCell& s_cell);

    void SetEntity(CXpuckRABEquippedEntity& c_entity);

private:

    CGrid<CXpuckRABEquippedEntity>& m_cGrid;
    CXpuckRABEquippedEntity* m_pcEntity;
};

class CXpuckRABEquippedEntityGridEntityUpdater : public CGrid<CXpuckRABEquippedEntity>::COperation {

public:

    CXpuckRABEquippedEntityGridEntityUpdater(CGrid<CXpuckRABEquippedEntity>& c_grid);
    virtual bool operator()(CXpuckRABEquippedEntity& c_entity);

private:

    CGrid<CXpuckRABEquippedEntity>& m_cGrid;
    CXpuckRABEquippedEntityGridCellUpdater m_cCellUpdater;
};

/****************************************/
/****************************************/

}

#endif

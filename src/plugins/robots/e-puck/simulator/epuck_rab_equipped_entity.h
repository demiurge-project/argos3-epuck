/**
 * @file <argos3/plugins/robots/e-puck/simulator/epuck_rab_equipped_entity.h>
 *
 * @author Gianpiero Francesca <gianpiero.francesca@ulb.ac.be>
 * @author Lorenzo Garattoni <lgaratto@ulb.ac.be>
 */

#ifndef EPUCK_RAB_EQUIPPED_ENTITY_H
#define EPUCK_RAB_EQUIPPED_ENTITY_H

namespace argos {
class CEpuckRABEquippedEntity;
}

#include <argos3/core/utility/datatypes/byte_array.h>
#include <argos3/core/utility/datatypes/set.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/simulator/space/positional_indices/space_hash.h>
#include <argos3/core/simulator/space/positional_indices/grid.h>
#include <argos3/plugins/simulator/entities/rab_equipped_entity.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_range_and_bearing_actuator.h>
namespace argos {

class CEpuckRABEquippedEntity : public CRABEquippedEntity {
public:

public:

    ENABLE_VTABLE();

    typedef std::vector<CEpuckRABEquippedEntity*> TVector;
    typedef CSet<CEpuckRABEquippedEntity*> TSet;
private:
    /** this is the data size */
    UInt8 m_uDataSize;
    CCI_EPuckRangeAndBearingActuator::SDataToSend m_cEmitterData;
    CEmbodiedEntity* m_pcEntityBody;
public:

    CEpuckRABEquippedEntity(CComposableEntity* pc_parent);

    CEpuckRABEquippedEntity(CComposableEntity* pc_parent,
                            const std::string& str_id,
                            size_t un_msg_size,
                            Real f_range,
                            SAnchor& s_anchor,
                            CEmbodiedEntity& c_entity_body,
                            const CVector3& c_position = CVector3(0,0,0),
                            const CQuaternion& c_orientation = CQuaternion().FromEulerAngles(CRadians::ZERO,CRadians::ZERO,CRadians::ZERO));


    virtual ~CEpuckRABEquippedEntity() {}

    virtual void Reset();

    //virtual void Update();

    inline size_t GetMsgSize() const {
        return m_uDataSize;
    }
    inline void SetDataSize(UInt8 uDataSize){
        m_uDataSize=uDataSize;
    }

    void SetDataToSend(const CCI_EPuckRangeAndBearingActuator::SDataToSend& c_data);

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
    CCI_EPuckRangeAndBearingActuator::SEmitter& GetEmitterForAngle(const CRadians& t_angle);

};

/****************************************/
/****************************************/

class CEpuckRABEquippedEntitySpaceHashUpdater : public CSpaceHashUpdater<CEpuckRABEquippedEntity> {

public:

    virtual void operator()(CAbstractSpaceHash<CEpuckRABEquippedEntity>& c_space_hash,
                            CEpuckRABEquippedEntity& c_element);

private:

    SInt32 m_nCenterI, m_nCenterJ, m_nCenterK;

};

/****************************************/
/****************************************/

class CEpuckRABEquippedEntityGridCellUpdater : public CGrid<CEpuckRABEquippedEntity>::CCellOperation {

public:

    CEpuckRABEquippedEntityGridCellUpdater(CGrid<CEpuckRABEquippedEntity>& c_grid);

    virtual bool operator()(SInt32 n_i,
                            SInt32 n_j,
                            SInt32 n_k,
                            CGrid<CEpuckRABEquippedEntity>::SCell& s_cell);

    void SetEntity(CEpuckRABEquippedEntity& c_entity);

private:

    CGrid<CEpuckRABEquippedEntity>& m_cGrid;
    CEpuckRABEquippedEntity* m_pcEntity;
};

class CEpuckRABEquippedEntityGridEntityUpdater : public CGrid<CEpuckRABEquippedEntity>::COperation {

public:

    CEpuckRABEquippedEntityGridEntityUpdater(CGrid<CEpuckRABEquippedEntity>& c_grid);
    virtual bool operator()(CEpuckRABEquippedEntity& c_entity);

private:

    CGrid<CEpuckRABEquippedEntity>& m_cGrid;
    CEpuckRABEquippedEntityGridCellUpdater m_cCellUpdater;
};

/****************************************/
/****************************************/

}

#endif

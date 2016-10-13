/**
 * @file <argos3/plugins/robots/e-puck/simulator/epuck_rab_equipped_entity.cpp>
 *
 * @author Gianpiero Francesca <gianpiero.francesca@ulb.ac.be>
 * @author Lorenzo Garattoni <lgaratto@ulb.ac.be>
 */

#include "epuck_rab_equipped_entity.h"
#include <argos3/core/utility/string_utilities.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>

namespace argos {

/****************************************/
/****************************************/

CEpuckRABEquippedEntity::CEpuckRABEquippedEntity(CComposableEntity* pc_parent) :
    CRABEquippedEntity(pc_parent), m_pcEntityBody(NULL){
    Disable();
}

/****************************************/
/****************************************/

CEpuckRABEquippedEntity::CEpuckRABEquippedEntity(CComposableEntity* pc_parent,
                                                 const std::string& str_id,
                                                 size_t un_msg_size,
                                                 Real f_range,
                                                 SAnchor& s_anchor,
                                                 CEmbodiedEntity& c_entity_body,
                                                 const CVector3& c_pos_offset,
                                                 const CQuaternion& c_rot_offset) :
   CRABEquippedEntity( pc_parent,
                        str_id,
                        un_msg_size,
                        f_range,
                        s_anchor,
                        c_entity_body,
                        c_pos_offset,
                        c_rot_offset), m_pcEntityBody(&c_entity_body)
{
    m_cData.Resize(un_msg_size);
    Disable();
}

/****************************************/
/****************************************/

CCI_EPuckRangeAndBearingActuator::SEmitter& CEpuckRABEquippedEntity::GetEmitterForAngle(const CRadians& t_angle) {
    /**
       * The 12 (from 0 to 11) emitters are uniformely placed around the robot and each one
       * covers 30 degrees. The number 0 covers from -15°,15°, the number 1 from 15°,45°
       * and so on...
       */
    CRadians angle=t_angle+CRadians::PI_OVER_SIX/2.0f;
    angle=angle.UnsignedNormalize();
    //value in the range [0,360]
    size_t idx=(size_t)(angle/CRadians::PI_OVER_SIX);
    return m_cEmitterData.Emitter[idx];
}

/****************************************/
/****************************************/

void CEpuckRABEquippedEntity::Reset() {
    m_cEmitterData=CCI_EPuckRangeAndBearingActuator::SDataToSend();
}

/****************************************/
/****************************************/

void CEpuckRABEquippedEntity::SetDataToSend(const CCI_EPuckRangeAndBearingActuator::SDataToSend& c_data) {
    m_cEmitterData = c_data;

}

/****************************************/
/****************************************/

void CEpuckRABEquippedEntity::ClearData() {
    m_cEmitterData=CCI_EPuckRangeAndBearingActuator::SDataToSend();
}

/****************************************/
/****************************************/

void CEpuckRABEquippedEntitySpaceHashUpdater::operator()(CAbstractSpaceHash<CEpuckRABEquippedEntity>& c_space_hash,
                                                         CEpuckRABEquippedEntity& c_element) {
    /* Calculate the position of the center of the RAB equipped entity in the space hash */
    c_space_hash.SpaceToHashTable(m_nCenterI,
                                  m_nCenterJ,
                                  m_nCenterK,
                                  c_element.GetPosition());
    /* Update the cells in a sphere around it */
    SInt32 nRangeI = c_space_hash.SpaceToHashTable(c_element.GetRange(), 0);
    SInt32 nRangeJ;
    SInt32 nRangeK;
    for(SInt32 i = 0; i <= nRangeI; ++i) {
        nRangeJ =
                c_space_hash.SpaceToHashTable(
                    ::sqrt(
                        Square(c_element.GetRange()) -
                        Square(c_space_hash.HashTableToSpace(i, 0))
                        ),
                    1);
        for(SInt32 j = 0; j <= nRangeJ; ++j) {
            nRangeK =
                    c_space_hash.SpaceToHashTable(
                        ::sqrt(
                            Square(c_element.GetRange()) -
                            Square(c_space_hash.HashTableToSpace(j, 1))
                            ),
                        2);
            for(SInt32 k = 0; k <= nRangeK; ++k) {
                if(i > 0) {
                    /*
                   * i > 0
                   */
                    if(j > 0) {
                        /*
                      * i > 0
                      * j > 0
                      */
                        if(k > 0) {
                            /*
                         * i > 0
                         * j > 0
                         * k > 0
                         */
                            c_space_hash.UpdateCell(m_nCenterI + i, m_nCenterJ + j, m_nCenterK + k, c_element);
                            c_space_hash.UpdateCell(m_nCenterI + i, m_nCenterJ + j, m_nCenterK - k, c_element);
                            c_space_hash.UpdateCell(m_nCenterI + i, m_nCenterJ - j, m_nCenterK + k, c_element);
                            c_space_hash.UpdateCell(m_nCenterI + i, m_nCenterJ - j, m_nCenterK - k, c_element);
                            c_space_hash.UpdateCell(m_nCenterI - i, m_nCenterJ + j, m_nCenterK + k, c_element);
                            c_space_hash.UpdateCell(m_nCenterI - i, m_nCenterJ + j, m_nCenterK - k, c_element);
                            c_space_hash.UpdateCell(m_nCenterI - i, m_nCenterJ - j, m_nCenterK + k, c_element);
                            c_space_hash.UpdateCell(m_nCenterI - i, m_nCenterJ - j, m_nCenterK - k, c_element);
                        }
                        else {
                            /*
                         * i > 0
                         * j > 0
                         * k == 0
                         */
                            c_space_hash.UpdateCell(m_nCenterI + i, m_nCenterJ + j, m_nCenterK, c_element);
                            c_space_hash.UpdateCell(m_nCenterI + i, m_nCenterJ - j, m_nCenterK, c_element);
                            c_space_hash.UpdateCell(m_nCenterI - i, m_nCenterJ + j, m_nCenterK, c_element);
                            c_space_hash.UpdateCell(m_nCenterI - i, m_nCenterJ - j, m_nCenterK, c_element);
                        }
                    }
                    else {
                        /*
                      * i > 0
                      * j == 0
                      */
                        if(k > 0) {
                            /*
                         * i > 0
                         * j == 0
                         * k > 0
                         */
                            c_space_hash.UpdateCell(m_nCenterI + i, m_nCenterJ, m_nCenterK + k, c_element);
                            c_space_hash.UpdateCell(m_nCenterI + i, m_nCenterJ, m_nCenterK - k, c_element);
                            c_space_hash.UpdateCell(m_nCenterI - i, m_nCenterJ, m_nCenterK + k, c_element);
                            c_space_hash.UpdateCell(m_nCenterI - i, m_nCenterJ, m_nCenterK - k, c_element);
                        }
                        else {
                            /*
                         * i > 0
                         * j == 0
                         * k == 0
                         */
                            c_space_hash.UpdateCell(m_nCenterI + i, m_nCenterJ, m_nCenterK, c_element);
                            c_space_hash.UpdateCell(m_nCenterI - i, m_nCenterJ, m_nCenterK, c_element);
                        }
                    }
                }
                else {
                    /*
                   * i == 0
                   */
                    if(j > 0) {
                        /*
                      * i == 0
                      * j > 0
                      */
                        if(k > 0) {
                            /*
                         * i == 0
                         * j > 0
                         * k > 0
                         */
                            c_space_hash.UpdateCell(m_nCenterI, m_nCenterJ + j, m_nCenterK + k, c_element);
                            c_space_hash.UpdateCell(m_nCenterI, m_nCenterJ + j, m_nCenterK - k, c_element);
                            c_space_hash.UpdateCell(m_nCenterI, m_nCenterJ - j, m_nCenterK + k, c_element);
                            c_space_hash.UpdateCell(m_nCenterI, m_nCenterJ - j, m_nCenterK - k, c_element);
                        }
                        else {
                            /*
                         * i == 0
                         * j > 0
                         * k == 0
                         */
                            c_space_hash.UpdateCell(m_nCenterI, m_nCenterJ + j, m_nCenterK, c_element);
                            c_space_hash.UpdateCell(m_nCenterI, m_nCenterJ - j, m_nCenterK, c_element);
                        }
                    }
                    else {
                        /*
                      * i == 0
                      * j == 0
                      */
                        if(k > 0) {
                            /*
                         * i == 0
                         * j == 0
                         * k > 0
                         */
                            c_space_hash.UpdateCell(m_nCenterI, m_nCenterJ, m_nCenterK + k, c_element);
                            c_space_hash.UpdateCell(m_nCenterI, m_nCenterJ, m_nCenterK - k, c_element);
                        }
                        else {
                            /*
                         * i == 0
                         * j == 0
                         * k == 0
                         */
                            c_space_hash.UpdateCell(m_nCenterI, m_nCenterJ, m_nCenterK, c_element);
                        }
                    }
                }
            }
        }
    }
}

/****************************************/
/****************************************/

REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CEpuckRABEquippedEntity);

/****************************************/
/****************************************/

CEpuckRABEquippedEntityGridCellUpdater::CEpuckRABEquippedEntityGridCellUpdater(CGrid<CEpuckRABEquippedEntity>& c_grid) :
    m_cGrid(c_grid) {}

bool CEpuckRABEquippedEntityGridCellUpdater::operator()(SInt32 n_i,
                                                        SInt32 n_j,
                                                        SInt32 n_k,
                                                        CGrid<CEpuckRABEquippedEntity>::SCell& s_cell) {
    /* Update cell */
    m_cGrid.UpdateCell(n_i, n_j, n_k, *m_pcEntity);
    /* Continue with other cells */
    return true;
}

void CEpuckRABEquippedEntityGridCellUpdater::SetEntity(CEpuckRABEquippedEntity& c_entity) {
    m_pcEntity = &c_entity;
}

CEpuckRABEquippedEntityGridEntityUpdater::CEpuckRABEquippedEntityGridEntityUpdater(CGrid<CEpuckRABEquippedEntity>& c_grid) :
    m_cGrid(c_grid),
    m_cCellUpdater(c_grid) {}

bool CEpuckRABEquippedEntityGridEntityUpdater::operator()(CEpuckRABEquippedEntity& c_entity) {
    try {
        m_cCellUpdater.SetEntity(c_entity);
        m_cGrid.ForCellsInBoxRange(c_entity.GetPosition(),
                                   CVector3(c_entity.GetRange(),
                                            c_entity.GetRange(),
                                            c_entity.GetRange()),
                                   m_cCellUpdater);
        /* Continue with the other entities */
        return true;
    }
    catch(CARGoSException& ex) {
        THROW_ARGOSEXCEPTION_NESTED("While updating the RAB entity grid for RAB entity \"" << c_entity.GetContext() << c_entity.GetId() << "\"", ex);
    }
}

/****************************************/
/****************************************/

}

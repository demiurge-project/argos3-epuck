/**
 * @file <argos3/plugins/robots/e-puck/control_interface/ci_epuck_virtual_camrab_actuator.cpp>
 *
 * @author Antoine Ligot - <aligot@ulb.ac.be>
 */

#include "ci_epuck_virtual_camrab_actuator.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#include <argos3/core/utility/datatypes/byte_array.h>
#endif

namespace argos {

    /****************************************/
    /****************************************/
    
    const UInt32 CCI_EPuckVirtualCamrabActuator::DEFAULT_NUMBER_COLORS = 3;

    /****************************************/
    /****************************************/

    #ifdef ARGOS_WITH_LUA
    /*
    * The stack should have one array of size GetDataSize()
    * and values in the range [0,255]
    */
    int LuaCamrabSetData(lua_State* pt_lua_state) {
        if(lua_gettop(pt_lua_state) == 1) {
            /* Check parameters */
            luaL_checktype(pt_lua_state, 1, LUA_TTABLE);
            /* Get reference to actuator */
            CCI_EPuckVirtualCamrabActuator* pcAct = CLuaUtility::GetDeviceInstance<CCI_EPuckVirtualCamrabActuator>(pt_lua_state, "virtual_camrab");
            /* Check whether sizes match */
            if(CCI_EPuckVirtualCamrabActuator::MAX_BYTES_SENT != lua_rawlen(pt_lua_state, -1)) {
                return luaL_error(pt_lua_state, "robot.virtual_camrab.set_data(array) expects an array of %d numbers", CCI_EPuckVirtualCamrabActuator::MAX_BYTES_SENT);
            }
            /* Fill up a byte array, checking that all elements are numbers */
            CCI_EPuckVirtualCamrabActuator::TData tBuf;
            for(size_t i = 0; i < CCI_EPuckVirtualCamrabActuator::MAX_BYTES_SENT ; ++i) {
                lua_pushnumber(pt_lua_state, i+1);
                lua_gettable(pt_lua_state, -2);
                if(lua_type(pt_lua_state, -1) == LUA_TNUMBER) {
                    tBuf[i] = static_cast<UInt8>(lua_tonumber(pt_lua_state, -1));
                    lua_pop(pt_lua_state, 1);
                }
                else {
                    return luaL_error(pt_lua_state, "element #%d of the array is not a number", i+1);
                }
            }
            /* Perform action */
            pcAct->SetData(tBuf);
            return 0;
        }
        else {
            return luaL_error(pt_lua_state, "robot.virtual_camrab.set_data() expects one arguments");
        }
    }
    #endif

    /****************************************/
    /****************************************/

    #ifdef ARGOS_WITH_LUA
    void CCI_EPuckVirtualCamrabActuator::CreateLuaState(lua_State* pt_lua_state) {
        CLuaUtility::StartTable(pt_lua_state, "virtual_camrab");
        CLuaUtility::AddToTable(pt_lua_state, "_instance", this);
        CLuaUtility::AddToTable(pt_lua_state, "set_data", &LuaCamrabSetData);
        CLuaUtility::EndTable(pt_lua_state);
    }
    #endif

    /****************************************/
    /****************************************/

    CCI_EPuckVirtualCamrabActuator::CCI_EPuckVirtualCamrabActuator() {
        m_bLEDFlag = false;
        m_pcRandomNumberGenerator = CRandom::CreateRNG("argos");
        m_unNumberColors = CCI_EPuckVirtualCamrabActuator::DEFAULT_NUMBER_COLORS;
    }

    /****************************************/
    /****************************************/

    void CCI_EPuckVirtualCamrabActuator::Init(TConfigurationNode& t_tree) {
        try {
            /* get the number of color used to light the LEDs */
            GetNodeAttributeOrDefault(t_tree, "number_colors", m_unNumberColors, m_unNumberColors);
            if (m_unNumberColors > 9) {
                LOG << "[WARNING] Number of colors too large, back to maximum value = 9" << std::endl;
                m_unNumberColors = 9;
            }
            /* computes the number of bits needed to encode the colors */
            m_unNumberBitsToShift = std::ceil(log2(m_unNumberColors));
            
        } catch (CARGoSException& ex) {
            THROW_ARGOSEXCEPTION_NESTED("Error initializing virtual camrab actuator", ex);
        }
    }

    /****************************************/
    /****************************************/

    void CCI_EPuckVirtualCamrabActuator::SetData(const TData t_data) {
        m_sDesiredData.State = STATE_ALL_EMITTERS_SAME_DATA;
        for(size_t i = 0; i < 12; ++i) {
            m_sDesiredData.Emitter[i].Enabled = true;
            m_sDesiredData.Emitter[i].Data[0] = (t_data[0] << m_unNumberBitsToShift) + m_unCurrentColorId;
            m_sDesiredData.Emitter[i].Data[1] = t_data[1];
            m_sDesiredData.Emitter[i].Data[2] = t_data[2];
            m_sDesiredData.Emitter[i].Data[3] = t_data[3];
        }
        m_bLEDFlag = true;
    }

    /****************************************/
    /****************************************/

    void CCI_EPuckVirtualCamrabActuator::SetDataForEmitter(size_t un_idx, const TData t_data) {
        m_sDesiredData.State = STATE_EMITTERS_DIFFERENT;
        m_sDesiredData.Emitter[un_idx].Enabled = true;
        m_sDesiredData.Emitter[un_idx].Data[0] = (t_data[0] << m_unNumberBitsToShift) + m_unCurrentColorId;
        m_sDesiredData.Emitter[un_idx].Data[1] = t_data[1];
        m_sDesiredData.Emitter[un_idx].Data[2] = t_data[2];
        m_sDesiredData.Emitter[un_idx].Data[3] = t_data[3]; 
        m_bLEDFlag = true;
    }
    
    /****************************************/
    /****************************************/

    CCI_EPuckVirtualCamrabActuator::SDataToSend::SDataToSend() :
        State(STATE_ALL_EMITTERS_DISABLED) {
        for(size_t i = 0; i < 12; ++i) {
            Emitter[i].Enabled = false;
            ::memset(&(Emitter[i].Data), 0, sizeof(Emitter[i].Data));
        }
    }

    /****************************************/
    /****************************************/

    CCI_EPuckVirtualCamrabActuator::SDataToSend::SDataToSend(const CCI_EPuckVirtualCamrabActuator::SDataToSend& s_data) :
        State(s_data.State) {
        for(size_t i = 0; i < 12; ++i) {
            Emitter[i].Enabled = s_data.Emitter[i].Enabled;
            Emitter[i].Data[0] = s_data.Emitter[i].Data[0];
            Emitter[i].Data[1] = s_data.Emitter[i].Data[1];
            Emitter[i].Data[2] = s_data.Emitter[i].Data[2];
            Emitter[i].Data[3] = s_data.Emitter[i].Data[3];
        }
    }

    /****************************************/
    /****************************************/

    CCI_EPuckVirtualCamrabActuator::SDataToSend&
    CCI_EPuckVirtualCamrabActuator::SDataToSend::operator=(
           const CCI_EPuckVirtualCamrabActuator::SDataToSend& s_data) {
        if(this != &s_data) {
            State = s_data.State;
            for(size_t i = 0; i < 12; ++i) {
                Emitter[i].Enabled = s_data.Emitter[i].Enabled;
                Emitter[i].Data[0] = s_data.Emitter[i].Data[0];
                Emitter[i].Data[1] = s_data.Emitter[i].Data[1];
                Emitter[i].Data[2] = s_data.Emitter[i].Data[2];
                Emitter[i].Data[3] = s_data.Emitter[i].Data[3];
            }
        }
        return *this;
    }

    /****************************************/
    /****************************************/

    void CCI_EPuckVirtualCamrabActuator::Disable() {
        m_sDesiredData.State = STATE_ALL_EMITTERS_DISABLED;
        for(size_t i = 0; i < 12; ++i) {
            m_sDesiredData.Emitter[i].Enabled = false;
        }
        m_bLEDFlag = false;
    }

    /***************************************/
    /***************************************/

    void CCI_EPuckVirtualCamrabActuator::PickRandomColor() {
        CRange<UInt32> cColorRange(0, m_unNumberColors);
        m_unCurrentColorId = m_pcRandomNumberGenerator->Uniform(cColorRange);
    }

    /***************************************/
    /***************************************/

    CColor CCI_EPuckVirtualCamrabActuator::IntegerToColor(UInt8 color_id) {
        CColor cColor;
        if (color_id == 0) {
            cColor = CColor::RED;
        } else if (color_id == 1) {
            cColor = CColor::BLUE;
        } else if (color_id == 2) {
            cColor = CColor::GREEN;
        } else if (color_id == 3) {
            cColor = CColor::MAGENTA;
        } else if (color_id == 4) {
            cColor = CColor::CYAN;
        } else if (color_id == 5) {
            cColor = CColor::YELLOW;
        } else if (color_id == 6) {
            cColor = CColor::BROWN;
        } else if (color_id == 7) {
            cColor = CColor::ORANGE;
        } else if (color_id == 8) {
            cColor = CColor::PURPLE;
        }
        return cColor;
    }

    /***************************************/
    /***************************************/
    
}

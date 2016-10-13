/**
 * @file <argos3/plugins/robots/e-puck/control_interface/ci_range_and_bearing_actuator.h>
 *
 * @author Gianpiero Francesca <gianpiero.francesca@ulb.ac.be>
 * @author Lorenzo Garattoni <lgaratto@ulb.ac.be>
 */

#ifndef CCI_EPUCK_RANGE_AND_BEARING_ACTUATOR_H
#define CCI_EPUCK_RANGE_AND_BEARING_ACTUATOR_H

namespace argos {
class CCI_EPuckRangeAndBearingActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>

namespace argos {

class CCI_EPuckRangeAndBearingActuator : public CCI_Actuator {
public:
    /* the number of bytes authorized by the RAB board */
    static const UInt32 MAX_BYTES_SENT = 4;


    /* struct to store the data to send */
    typedef UInt8 TData[MAX_BYTES_SENT];

public:

    enum EEmitterState {
        STATE_ALL_EMITTERS_DISABLED = 0,
        STATE_ALL_EMITTERS_SAME_DATA,
        STATE_EMITTERS_DIFFERENT
    };
    struct SEmitter{
        bool Enabled;
        TData Data;
    };
    struct SDataToSend {
        EEmitterState State;
        SEmitter Emitter[12];

        SDataToSend();
        SDataToSend(const SDataToSend& s_data);
        SDataToSend& operator=(const SDataToSend& s_data);
    };


public:

    virtual ~CCI_EPuckRangeAndBearingActuator() {}
    virtual void Init(TConfigurationNode& t_tree);

    /**
       * Sets a message to be broadcast by all emitters.
       * This method also implicitly enables all emitters.
       */
    virtual void SetData(const TData t_data);

    /**
       * Sets a message to be broadcast by a specific emitter.
       * This method also implicitly enables the emitter.
       */
    virtual void SetDataForEmitter(size_t un_idx,
                                   const TData t_data);

    /**
       * Disables all emitters.
       */
    virtual void Disable();


    /**
       * Disables a specific emitter.
       */
    virtual void DisableEmitter(size_t un_idx);
    /**
       * Reset.
       */
    virtual void Reset(){
        Disable();
    }
protected:
    /** this is the maximum data size */
    UInt8 m_uDataSize;
    /** This structure stores what the controllers wants to send */
    SDataToSend m_sDesiredData;

#ifdef ARGOS_WITH_LUA
    virtual void CreateLuaState(lua_State* pt_lua_state);
#endif

};
}
#endif

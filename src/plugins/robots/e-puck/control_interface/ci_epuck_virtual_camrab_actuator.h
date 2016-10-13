/**
 * @file <argos3/plugins/robots/e-puck/control_interface/ci_epuck_virtual_camrab_actuator.h>
 *
 * @brief This file provides the definition of the epuck virtual camrab actuator.
 *
 * This file provides the definition of the epuck virtual camrab actuator.
 * The actuator combines the range and bearing and rgb leds actuator to enable 
 * better distance estimation.
 *
 * @author Antoine Ligot - <aligot@ulb.ac.be>
 */

#ifndef CCI_EPUCK_VIRTUAL_CAMRAB_ACTUATOR_H
#define CCI_EPUCK_VIRTUAL_CAMRAB_ACTUATOR_H

namespace argos {
   class CCI_EPuckVirtualCamrabActuator;
}

#include <argos3/core/utility/math/rng.h> 
#include <argos3/core/utility/datatypes/color.h>
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/control_interface/ci_actuator.h>

namespace argos {

class CCI_EPuckVirtualCamrabActuator : public CCI_Actuator {

    public:
        /** the number of bytes authorized by the RAB board */
        static const UInt32 MAX_BYTES_SENT = 4;

        /** the default number of color to light the LEDs */
        static const UInt32 DEFAULT_NUMBER_COLORS;

        /** struct to store the data to send */
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

    /**
     * Constructor
     */
    CCI_EPuckVirtualCamrabActuator();

    /**
     * Destructor
     */
    virtual ~CCI_EPuckVirtualCamrabActuator(){}
	
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
    virtual void SetDataForEmitter(size_t un_idx, const TData t_data);
    
    /** Disables all emitters. */
    virtual void Disable();
    
    /**
       * Reset.
       */
    virtual void Reset(){
        Disable();
    }
                                   
    /** 
     * Select a color randomly. This color will be used to light the LEDs.
     * Sets m_unCurrentColorId to random integers between 0 and m_unNumberColors included. 
     */              
    void PickRandomColor();

protected:    
    /** This structure stores what the controllers wants to send */
    SDataToSend m_sDesiredData;
    
    /** Number of different colors used to light the LEDs */
    UInt32 m_unNumberColors;
    
    /** 
     * Flag used to determine when to light the LEDs
     * When set to true, LEDs will light. When set to false, LEDs will be off. 
     */
    bool m_bLEDFlag;
    
	/** Random number generator used to select color */
	CRandom::CRNG* m_pcRandomNumberGenerator;
	
    /** 
     * Integer corresponding to the CColor used to light the LEDs.
     * Will be sent in the first byte of the range and bearing payload. 
     */
	UInt8 m_unCurrentColorId;
	
    /** 
     * Number of bits that the first byte of the range and bearing payload
     * will be shifted to include m_unCurrentColorId. Depends on m_uNumberColors.
     */
	UInt8 m_unNumberBitsToShift;
    
protected:

    /** Transform an UInt8 to the corresponding CColor */
	CColor IntegerToColor(UInt8 color_id);

	#ifdef ARGOS_WITH_LUA
    virtual void CreateLuaState(lua_State* pt_lua_state);
	#endif
};

}

#endif

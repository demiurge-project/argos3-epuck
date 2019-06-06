/**
 * @file <argos3/plugins/robots/x-puck/real_robot/real_xpuck_virtual_camrab_actuator.h>
 *
 * @brief This file provides the implementation of the xpuck virtual camrab actuator.
 *
 * The actuator lights up the rgb LEDs and sends an integer  via the range and bearing sensor.
 * The integer represents the color used for the LED.
 *
 * @author Antoine Ligot <aligot@ulb.ac.be>
 */

#ifndef REAL_XPUCK_VIRTUAL_CAMRAB_ACTUATOR_H
#define REAL_XPUCK_VIRTUAL_CAMRAB_ACTUATOR_H

namespace argos {
   class CRealXPuckVirtualCamrabActuator;
   class CRealXPuck;
}

#include <math.h>
#include <argos3/core/utility/math/rng.h> 
#include <argos3/plugins/robots/x-puck/real_robot/real_xpuck.h>
#include <argos3/plugins/robots/x-puck/control_interface/ci_xpuck_virtual_camrab_actuator.h>
#include <argos3/plugins/robots/x-puck/real_robot/real_xpuck_i2c_actuator.h>
#include <argos3/plugins/robots/x-puck/control_interface/ci_xpuck_rgb_leds_actuator.h>
#include <argos3/plugins/robots/x-puck/control_interface/ci_xpuck_range_and_bearing_actuator.h>

namespace argos {
   class CRealXPuckVirtualCamrabActuator : virtual public CCI_XPuckVirtualCamrabActuator,
                                             virtual public CRealXPuckI2CActuator {

	public:

		CRealXPuckVirtualCamrabActuator();
		virtual ~CRealXPuckVirtualCamrabActuator();

		virtual void Init(TConfigurationNode& t_node);
		virtual void SendData();

	private:
		
        void DataSenderThread();

        static void* StartThread(void* pt_arg);

        /** Lights up the LEDs with color m_cCurrentColor. */
        void TurnOnLEDs();

         /** Switches the LEDs off (lights them with CColor::BLACK) */
        void TurnOffLEDs();

	private:
        /** Pointer to the rgb led actuator */
        CCI_XPuckRGBLEDsActuator* m_pcRgbLedActuator;

        /** Pointer to the range and bearing actuator */
        CCI_XPuckRangeAndBearingActuator* m_pcRabActuator;

        /** Pointer to the xpuck implementation */
        CRealXPuck* m_pcRealXPuck;

        /** This structure stores what it is actually sent by the thread */
        SDataToSend m_sSentData;

        /** Flag that is used to tell if LEDs have to be lit or not */
        bool m_bLightLEDs;

        /** Handle to the data sender thread */
        pthread_t m_tDataSenderThread;

        /** This mutex protects the concurrent access to the data to send */
        pthread_mutex_t m_tDataMutex;

        /** Color to light the LEDs */
        CColor m_cCurrentColor;

};
}
#endif

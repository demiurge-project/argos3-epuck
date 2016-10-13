/**
 * @file <argos3/plugins/robots/e-puck/real_robot/real_epuck_virtual_camrab_actuator.h>
 *
 * @brief This file provides the implementation of the epuck virtual camrab actuator.
 *
 * The actuator lights up the rgb LEDs and sends an integer  via the range and bearing sensor.
 * The integer represents the color used for the LED.
 *
 * @author Antoine Ligot <aligot@ulb.ac.be>
 */

#ifndef REAL_EPUCK_VIRTUAL_CAMRAB_ACTUATOR_H
#define REAL_EPUCK_VIRTUAL_CAMRAB_ACTUATOR_H

namespace argos {
   class CRealEPuckVirtualCamrabActuator;
   class CRealEPuck;
}

#include <math.h>
#include <argos3/core/utility/math/rng.h> 
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_virtual_camrab_actuator.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_i2c_actuator.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_rgb_leds_actuator.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_range_and_bearing_actuator.h>

namespace argos {
   class CRealEPuckVirtualCamrabActuator : virtual public CCI_EPuckVirtualCamrabActuator,
                                             virtual public CRealEPuckI2CActuator {

	public:

		CRealEPuckVirtualCamrabActuator();
		virtual ~CRealEPuckVirtualCamrabActuator();

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
        CCI_EPuckRGBLEDsActuator* m_pcRgbLedActuator;

        /** Pointer to the range and bearing actuator */
        CCI_EPuckRangeAndBearingActuator* m_pcRabActuator;

        /** Pointer to the epuck implementation */
        CRealEPuck* m_pcRealEPuck;

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

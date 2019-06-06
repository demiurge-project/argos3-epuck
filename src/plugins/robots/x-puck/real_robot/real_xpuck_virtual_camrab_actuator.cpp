/**
 * @file <argos3/plugins/robots/x-puck/real_robot/real_xpuck_virtual_camrab_actuator.cpp>
 *
 * @author Antoine Ligot - <aligot@ulb.ac.be>
 */

#include "real_xpuck_virtual_camrab_actuator.h"
#include <argos3/core/utility/logging/argos_log.h>

#include <cstring>

namespace argos {

    /** time between two send in micro seconds (so 10 000 micro seconds = 10 milli seconds) */
    static UInt32 TIME_BETWEEN_TWO_SEND = 20000;

    /****************************************/
    /****************************************/
    
    static void VirtualCamrabActuatorCleanupThreadData(void* pt_data) {
        CRealXPuckI2CDevice::ThreadCleanup();
        pthread_mutex_t* ptMutex = reinterpret_cast<pthread_mutex_t*>(pt_data);
        pthread_mutex_unlock(ptMutex);

    }

    /****************************************/
    /****************************************/

    CRealXPuckVirtualCamrabActuator::CRealXPuckVirtualCamrabActuator() {
        m_cCurrentColor = CColor::RED;
        m_bLightLEDs = false;
        CCI_XPuckVirtualCamrabActuator();

    }

    /****************************************/
    /****************************************/

    CRealXPuckVirtualCamrabActuator::~CRealXPuckVirtualCamrabActuator() {
        ::pthread_cancel(m_tDataSenderThread);
    }

    /****************************************/
    /****************************************/
    
    void CRealXPuckVirtualCamrabActuator::Init(TConfigurationNode& t_node){
        CCI_XPuckVirtualCamrabActuator::Init(t_node);
        
        try {
            m_pcRealXPuck = &CRealXPuck::GetInstance();
            m_pcRgbLedActuator = m_pcRealXPuck->GetController().GetActuator<CCI_XPuckRGBLEDsActuator>("xpuck_rgb_leds");
            m_pcRabActuator = m_pcRealXPuck->GetController().GetActuator<CCI_XPuckRangeAndBearingActuator>("xpuck_range_and_bearing");
        
            /*
              * Start threads and related data
              */
            ::pthread_mutex_init(&m_tDataMutex, NULL);
            ::pthread_create(&m_tDataSenderThread, NULL, &StartThread, this);
            
        } catch (CARGoSException ex) {
            LOGERR<<"Error while initializing the virtual Camrab actuator! \n";
            LOGERR<<"Make sure the rgb leds and the range-and-bearing actuators are \n ";
            LOGERR<<"intialized correctly as the Camrab actuator uses both.\n";
        }
    }

    /****************************************/
    /****************************************/
   
    void CRealXPuckVirtualCamrabActuator::SendData() {
        pthread_mutex_lock(&m_tDataMutex);
        m_sSentData = m_sDesiredData;
        m_bLightLEDs = m_bLEDFlag;
        m_cCurrentColor = IntegerToColor(m_unCurrentColorId);
        pthread_mutex_unlock(&m_tDataMutex);

    }

    /****************************************/
    /****************************************/

    void CRealXPuckVirtualCamrabActuator::DataSenderThread() {

        /* Register the function CleanupThreadData to be executed upon thread cancellation */
        pthread_cleanup_push(VirtualCamrabActuatorCleanupThreadData, &m_tDataMutex);
        /* Actual thread logic */
        while(true) {
            
            pthread_testcancel();
            pthread_mutex_lock(&m_tDataMutex);
            switch(m_sSentData.State) {
                case STATE_ALL_EMITTERS_DISABLED: {
                    m_pcRabActuator->Disable();
                    break;
                }

                case STATE_ALL_EMITTERS_SAME_DATA: {
                    m_pcRabActuator->SetData(m_sSentData.Emitter[0].Data);
                    break;
                }

                case STATE_EMITTERS_DIFFERENT: {
                    for(size_t i = 0; i < 12; ++i) {
                        if(m_sSentData.Emitter[i].Enabled) {
                            m_pcRabActuator->SetDataForEmitter(i, m_sSentData.Emitter[i].Data);
                        }
                    }
            
                    break;
                }

                default:
                    THROW_ARGOSEXCEPTION("[BUG] CRealXPuckVirtualCamrabActuator::DataSenderThread() is in an unknown state");
                }
            
            if (m_bLightLEDs == true) {
                TurnOnLEDs();
            } else {
                TurnOffLEDs();
            }
            
            pthread_mutex_unlock(&m_tDataMutex);
            /*
              * Sleep for a while to let the other devices use the I2C bus
              * The value is set in microseconds:
              * 10.000 = 10 millisec => 10 times per control step
              */
            ::usleep(TIME_BETWEEN_TWO_SEND);
        }
        pthread_cleanup_pop(1);
    }

    /****************************************/
    /****************************************/

    void* CRealXPuckVirtualCamrabActuator::StartThread(void* pt_arg) {
        reinterpret_cast<CRealXPuckVirtualCamrabActuator*>(pt_arg)->DataSenderThread();
        return NULL;
    }

    /***************************************/
    /***************************************/

    void CRealXPuckVirtualCamrabActuator::TurnOnLEDs() {
        m_pcRgbLedActuator->SetColor(0, m_cCurrentColor);
        m_pcRgbLedActuator->SetColor(1, m_cCurrentColor);
        m_pcRgbLedActuator->SetColor(2, m_cCurrentColor);
    }

    /***************************************/
    /***************************************/

    void CRealXPuckVirtualCamrabActuator::TurnOffLEDs() {
        m_pcRgbLedActuator->SetColor(0, CColor::BLACK);
        m_pcRgbLedActuator->SetColor(1, CColor::BLACK);
        m_pcRgbLedActuator->SetColor(2, CColor::BLACK);
    }
    
    /***************************************/
    /***************************************/

}

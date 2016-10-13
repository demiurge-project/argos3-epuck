/**
 * @file <argos3/plugins/robots/e-puck/real_robot/real_epuck_range_and_bearing_actuator.cpp>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 < */

#include "real_epuck_range_and_bearing_actuator.h"
#include <argos3/core/utility/logging/argos_log.h>

#include <cstring>

namespace argos {

/****************************************/
/****************************************/

static const std::string LOW_LEVEL_CONFIGURATION_FILE = "/home/root/ARGoS/low_level_configuration/range_and_bearing_sensor.xml";
/* time between two send in micro seconds (so 10 000 micro seconds = 10 milli seconds) */
static UInt32 TIME_BETWEEN_TWO_SEND = 20000;

/****************************************/
/****************************************/

enum ERABRegisters {
    RAB_REGISTER_SET_RANGE = 12,
    RAB_REGISTER_SEND_DATA_BYTE_4 = 13,
    RAB_REGISTER_SEND_DATA_BYTE_1 = 14,
    RAB_REGISTER_SEND_DATA = 15,
    RAB_REGISTER_SEND_DATA_BYTE_3 = 19,
    RAB_REGISTER_SEND_DATA_BYTE_2 = 20,
    RAB_REGISTER_SET_DATA_SIZE = 21,
    RAB_REGISTER_DEVICE = 0x20
};

/****************************************/
/****************************************/

static void RABActuatorCleanupThreadData(void* pt_data) {

    CRealEPuckI2CDevice::ThreadCleanup();
    pthread_mutex_t* ptMutex = reinterpret_cast<pthread_mutex_t*>(pt_data);
    pthread_mutex_unlock(ptMutex);

}

/****************************************/
/****************************************/

CRealEPuckRangeAndBearingActuator::CRealEPuckRangeAndBearingActuator() {

    m_tDeviceStream = OpenDevice(RAB_REGISTER_DEVICE);

}

/****************************************/
/****************************************/

CRealEPuckRangeAndBearingActuator::~CRealEPuckRangeAndBearingActuator() {

    /* Cancel thread */
    ::pthread_cancel(m_tPacketSenderThread);
    /* Disable all emitters */
    Disable();
    SendData();
    /* Close device stream */
    CloseDevice(m_tDeviceStream);

}

/****************************************/
/****************************************/

void CRealEPuckRangeAndBearingActuator::Init(TConfigurationNode& t_node) {
    CCI_EPuckRangeAndBearingActuator::Init(t_node);

    try {
        /*
          * Parse XML
          */
        UInt8 unRange = 115;
        /*To understand what e_randb_set_range means, check the epuck rab's manual*/
        GetNodeAttributeOrDefault(t_node, "e_randb_set_range", unRange, unRange);
        GetNodeAttributeOrDefault(t_node, "delay", TIME_BETWEEN_TWO_SEND, TIME_BETWEEN_TWO_SEND);
        /*
          * Perform RAB configuration
          */
        /* Set transmission range */
        WriteRegister(m_tDeviceStream,
                      RAB_REGISTER_SET_RANGE,
                      unRange);
        LOG << "[INFO] Real Epuck RAB actuator data_size=" << m_uDataSize << std::endl;
        /* Set data size */
        WriteRegister(m_tDeviceStream,
                      RAB_REGISTER_SET_DATA_SIZE,
                      m_uDataSize * 8);
        /*
          * Start threads and related data
          */
        ::pthread_mutex_init(&m_tDataMutex, NULL);
        ::pthread_create(&m_tPacketSenderThread, NULL, &StartThread, this);
    } catch (CARGoSException& ex) {
        THROW_ARGOSEXCEPTION_NESTED("Error initializing range-and-bearing actuator", ex);
    }

}

/****************************************/
/****************************************/

void CRealEPuckRangeAndBearingActuator::SendData() {

    pthread_mutex_lock(&m_tDataMutex);
    m_sSentData = m_sDesiredData;
    pthread_mutex_unlock(&m_tDataMutex);

}


/****************************************/
/****************************************/

void CRealEPuckRangeAndBearingActuator::PacketSenderThread() {

    /* Register the function CleanupThreadData to be executed upon thread cancellation */
    pthread_cleanup_push(RABActuatorCleanupThreadData, &m_tDataMutex);
    /* Actual thread logic */
    while(true) {

        pthread_testcancel();
        pthread_mutex_lock(&m_tDataMutex);
        switch(m_sSentData.State) {
        case STATE_ALL_EMITTERS_DISABLED: {
            /* Nothing to send */
            break;
        }

        case STATE_ALL_EMITTERS_SAME_DATA: {
            if(m_uDataSize > 3) {
                WriteRegister(m_tDeviceStream,
                              RAB_REGISTER_SEND_DATA_BYTE_4,
                              m_sSentData.Emitter[0].Data[3]);
            }
            if(m_uDataSize > 2) {
                WriteRegister(m_tDeviceStream,
                              RAB_REGISTER_SEND_DATA_BYTE_3,
                              m_sSentData.Emitter[0].Data[2]);
            }
            if(m_uDataSize > 1) {
                WriteRegister(m_tDeviceStream,
                              RAB_REGISTER_SEND_DATA_BYTE_2,
                              m_sSentData.Emitter[0].Data[1]);
            }
            WriteRegister(m_tDeviceStream,
                          RAB_REGISTER_SEND_DATA_BYTE_1,
                          m_sSentData.Emitter[0].Data[0]);
            break;
        }

        case STATE_EMITTERS_DIFFERENT: {
            for(size_t i = 0; i < 12; ++i) {
                if(m_sSentData.Emitter[i].Enabled) {
                    /* Store data to send for emitter i */
                    if(m_uDataSize > 3)
                        WriteRegister(m_tDeviceStream,
                                      RAB_REGISTER_SEND_DATA_BYTE_4,
                                      m_sSentData.Emitter[i].Data[3]);
                    if(m_uDataSize > 2)
                        WriteRegister(m_tDeviceStream,
                                      RAB_REGISTER_SEND_DATA_BYTE_3,
                                      m_sSentData.Emitter[i].Data[2]);
                    if(m_uDataSize > 1)
                        WriteRegister(m_tDeviceStream,
                                      RAB_REGISTER_SEND_DATA_BYTE_2,
                                      m_sSentData.Emitter[i].Data[1]);
                    WriteRegister(m_tDeviceStream, i, m_sSentData.Emitter[i].Data[0]);
                }
                /* Actually send data */
                WriteRegister(m_tDeviceStream, RAB_REGISTER_SEND_DATA, 0);
            }
            break;
        }

        default:
            THROW_ARGOSEXCEPTION("[BUG] CRealEPuckRangeAndBearingActuator::PacketSenderThread() is in an unknown state");
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

void* CRealEPuckRangeAndBearingActuator::StartThread(void* pt_arg) {
    reinterpret_cast<CRealEPuckRangeAndBearingActuator*>(pt_arg)->PacketSenderThread();
    return NULL;
}

/****************************************/
/****************************************/



}

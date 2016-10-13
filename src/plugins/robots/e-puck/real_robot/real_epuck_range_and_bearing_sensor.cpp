/**
 * @file <argos3/plugins/robots/e-puck/real_robot/real_epuck_range_and_bearing_sensor.cpp>
 *
 * @author Gianpiero Francesca - <gianpiero.francesca@ulb.ac.be>
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#include "real_epuck_range_and_bearing_sensor.h"

#include <argos3/core/utility/math/angles.h>
#include <argos3/core/utility/logging/argos_log.h>

#include <unistd.h>

namespace argos {

/****************************************/
/****************************************/

static const std::string LOW_LEVEL_CONFIGURATION_FILE = "/home/root/ARGoS/low_level_configuration/range_and_bearing_sensor.xml";
/* time between two receive in micro seconds (so 10 000 micro seconds = 10 milli seconds) */
static UInt32 TIME_BETWEEN_TWO_RECEIVE = 20000;

/****************************************/
/****************************************/

enum ERABCalculation {
    RAB_CALCULATION_ON_RAB_BOARD = 0,
    RAB_CALCULATION_ON_LINUX_BOARD = 1
};

enum ERABRegisters {
    RAB_REGISTER_DATA_RECEIVED = 0,
    RAB_REGISTER_DATA_BYTE_2 = 1,
    RAB_REGISTER_DATA_BYTE_1 = 2,
    RAB_REGISTER_BEARING_BYTE_1 = 3,
    RAB_REGISTER_BEARING_BYTE_2 = 4,
    RAB_REGISTER_RANGE_BYTE_1 = 5,
    RAB_REGISTER_RANGE_BYTE_2 = 6,
    RAB_REGISTER_DATA_BYTE_4 = 10,
    RAB_REGISTER_DATA_BYTE_3 = 11,
    RAB_REGISTER_STORE_LIGHT_COND = 16,
    RAB_REGISTER_SET_CALCULATION = 17,
    RAB_REGISTER_SET_DATA_SIZE = 21,
    RAB_REGISTER_DEVICE = 0x20
};

/****************************************/
/****************************************/

static void RABSensorCleanupThreadData(void* pt_data) {

    CRealEPuckI2CDevice::ThreadCleanup();
    pthread_mutex_t* ptMutex = reinterpret_cast<pthread_mutex_t*>(pt_data);
    pthread_mutex_unlock(ptMutex);

}

/****************************************/
/****************************************/

static Real CalculateCalibratedRange(Real f_gain,
                                     Real f_a,
                                     Real f_b,
                                     UInt16 un_raw_range) {
    return f_gain * Max(Exp(f_a + f_b * un_raw_range), 0.0f);
}

/****************************************/
/****************************************/

CRealEPuckRangeAndBearingSensor::CRealEPuckRangeAndBearingSensor() :
    m_fExpA(9.06422181283387f),
    m_fExpB(-0.00565074879677167f),
    m_expK(0.0867438559),//Default values
    m_bCalibrated(false) {

    m_tDeviceStream = OpenDevice(RAB_REGISTER_DEVICE);

}

/****************************************/
/****************************************/

CRealEPuckRangeAndBearingSensor::~CRealEPuckRangeAndBearingSensor() {

    /* Cancel thread */
    ::pthread_cancel(m_tMessageFetcherThread);
    /* Erase packets */
    while(! m_tQueuedPackets.empty()) {
        delete m_tQueuedPackets.back();
        m_tQueuedPackets.pop_back();
    }
    while(! m_tPackets.empty()) {
        delete m_tPackets.back();
        m_tPackets.pop_back();
    }
    /* Close device stream */
    CloseDevice(m_tDeviceStream);

}

/****************************************/
/****************************************/

void CRealEPuckRangeAndBearingSensor::Init(TConfigurationNode& t_node) {
    CCI_EPuckRangeAndBearingSensor::Init(t_node);

    try {
        /*
          * Read XML configuration
          * 1. calibrated
          * 2. low level config
          */
        GetNodeAttributeOrDefault(t_node,
                                  "calibrated",
                                  m_bCalibrated,
                                  m_bCalibrated);
        if(m_bCalibrated) {
            std::string sLowLevelConfigurationFile = LOW_LEVEL_CONFIGURATION_FILE;
            GetNodeAttributeOrDefault(t_node,
                                      "low_level_config",
                                      sLowLevelConfigurationFile,
                                      sLowLevelConfigurationFile);
            Calibrate(sLowLevelConfigurationFile);
        }
        else {
            LOGERR << "[WARNING] Using range and bearing sensor without calibration." << std::endl;
        }
        GetNodeAttributeOrDefault(t_node, "delay", TIME_BETWEEN_TWO_RECEIVE, TIME_BETWEEN_TWO_RECEIVE);

        /*
          * Perform RAB configuration
          */
        /* Perform calculation on the RAB board (and not the Linux board) */
        WriteRegister(m_tDeviceStream,
                      RAB_REGISTER_SET_CALCULATION,
                      RAB_CALCULATION_ON_RAB_BOARD);
        /* This sleep avoid writing too fast to the bus */
        ::usleep(TIME_BETWEEN_TWO_RECEIVE);
        /* Store light conditions (gives lower noise on readings) */
        WriteRegister(m_tDeviceStream,
                      RAB_REGISTER_STORE_LIGHT_COND,
                      0);
        /* This sleep avoid writing too fast to the bus */
        ::usleep(TIME_BETWEEN_TWO_RECEIVE);
        LOG << "[INFO] Real Epuck RAB sensor data_size=" << m_uDataSize << std::endl;
        /* Set data size */
        WriteRegister(m_tDeviceStream,
                      RAB_REGISTER_SET_DATA_SIZE,
                      m_uDataSize * 8);
        /*
          * Start threads and related data
          */
        ::pthread_mutex_init(&m_tPacketQueueMutex, NULL);
        ::pthread_create(&m_tMessageFetcherThread, NULL, &StartThread, this);
    } catch (CARGoSException& ex) {
        THROW_ARGOSEXCEPTION_NESTED("Error initializing range-and-bearing sensor", ex);
    }

}

/****************************************/
/****************************************/

void CRealEPuckRangeAndBearingSensor::ReceiveData() {

    pthread_mutex_lock(&m_tPacketQueueMutex);
    while(! m_tQueuedPackets.empty()) {
        m_tPackets.push_back(m_tQueuedPackets.front());
        m_tQueuedPackets.pop_front();
    }
    pthread_mutex_unlock(&m_tPacketQueueMutex);

}

/****************************************/
/****************************************/

void CRealEPuckRangeAndBearingSensor::Calibrate(const std::string& str_conf_file) {
    try {
            ticpp::Document tConfiguration(str_conf_file);
            tConfiguration.LoadFile();
            TConfigurationNode tGroundNode = GetNode(*tConfiguration.FirstChildElement(), "range_and_bearing_sensor");
            TConfigurationNode tKNode = GetNode(tGroundNode, "k_value");

            std::string sKText = tKNode.GetText();

            m_expK = FromString<Real>(sKText);

         }
         catch (std::exception& ex) {
            THROW_ARGOSEXCEPTION_NESTED("Unable to read range and bearing sensor k value from file", ex);
         }
}

/****************************************/
/****************************************/

void CRealEPuckRangeAndBearingSensor::MessageFetcherThread() {
    /* Register the function CleanupThreadData to be executed upon thread cancellation */
    pthread_cleanup_push(RABSensorCleanupThreadData, &m_tPacketQueueMutex);
    /* Actual thread logic */
    while(true) {
        pthread_testcancel();

        /* Proceed only if data has been received */
        if(ReadRegister(m_tDeviceStream,
                        RAB_REGISTER_DATA_RECEIVED,
                        1)) {
            pthread_testcancel();
            /* Fill in the packet data */
            CCI_EPuckRangeAndBearingSensor::SReceivedPacket* psPacket =
                    new CCI_EPuckRangeAndBearingSensor::SReceivedPacket();
            if(m_uDataSize > 3) {
                psPacket->Data[3] = ReadRegister(m_tDeviceStream, RAB_REGISTER_DATA_BYTE_4, 1);
            }
            if(m_uDataSize > 2) {
                psPacket->Data[2] = ReadRegister(m_tDeviceStream, RAB_REGISTER_DATA_BYTE_3, 1);
            }
            if(m_uDataSize > 1) {
                psPacket->Data[1] = ReadRegister(m_tDeviceStream, RAB_REGISTER_DATA_BYTE_2, 1);
            }
            psPacket->Data[0] = ReadRegister(m_tDeviceStream, RAB_REGISTER_DATA_BYTE_1, 1);


            /* Fill in the packet bearing */
            UInt8 unRawBearing1 = ReadRegister(m_tDeviceStream, RAB_REGISTER_BEARING_BYTE_1, 1);
            UInt8 unRawBearing2 = ReadRegister(m_tDeviceStream, RAB_REGISTER_BEARING_BYTE_2, 1);         
			psPacket->Bearing = CRadians(static_cast<Real>((unRawBearing1 << 8) + unRawBearing2) * 0.0001f);
            /* Fill in the packet range */
            UInt8 unRawRange1 = ReadRegister(m_tDeviceStream, RAB_REGISTER_RANGE_BYTE_1, 1);
            UInt8 unRawRange2 = ReadRegister(m_tDeviceStream, RAB_REGISTER_RANGE_BYTE_2, 1);
            if(m_bCalibrated) {
                // TODO
                psPacket->Range = CalculateCalibratedRange(
                            m_expK, m_fExpA, m_fExpB,
                            (unRawRange1 << 8) + unRawRange2);
            }
            else {
				psPacket->Range = (unRawRange1 << 8) + unRawRange2;
            }

            /* Add packet to the queue */
            pthread_mutex_lock(&m_tPacketQueueMutex);
            m_tQueuedPackets.push_back(psPacket);
            pthread_mutex_unlock(&m_tPacketQueueMutex);
        }
        /*
          * Sleep for a while to let the other devices use the I2C bus
          * The value is set in microseconds:
          * 10.000 = 10 millisec => 10 times per control step
          */
        ::usleep(TIME_BETWEEN_TWO_RECEIVE);

    }
    pthread_cleanup_pop(1);
}

/****************************************/
/****************************************/

void* CRealEPuckRangeAndBearingSensor::StartThread(void* pt_arg) {
    reinterpret_cast<CRealEPuckRangeAndBearingSensor*>(pt_arg)->MessageFetcherThread();
    return NULL;
}

/****************************************/
/****************************************/

}

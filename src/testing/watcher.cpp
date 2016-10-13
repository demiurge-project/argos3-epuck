#include <stdlib.h>

#include "watcher.h"
#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <argos3/core/utility/logging/argos_log.h>

static std::ofstream file;

#define LOG_FILE_INIT(FILENAME) if (file) file.open (FILENAME);
#define LOG_FILE(SMTHG) if (file.is_open()) file << SMTHG;
#define LOG_FILE_CLOSE() if (file.is_open()) file.close();

using namespace argos;

/****************************************/
/****************************************/

CWatcherController::CWatcherController() :
    m_uRobotID(0),
    m_nDelay(10),
    m_nRange(115),
    m_nDataSize(2),
    m_nDistance(0),
    m_nRun(0),
    filename(""),
    m_sLeftWheelSpeed(0),
    m_sRightWheelSpeed(0),
    m_nControlStep(0),
    m_pcWheelsActuator(NULL),
    m_pcLEDsActuator(NULL),
    m_pcRGBLEDsActuator(NULL),
    m_pcRABActuator(NULL),
    m_pcIRComActuator(NULL),
    m_pcProximitySensor(NULL),
    m_pcLightSensor(NULL),
    m_pcGroundSensor(NULL),
    m_pcRABSensor(NULL),
    m_pcIRComSensor(NULL),
    m_pcCamera(NULL){
}

/****************************************/
/****************************************/

CWatcherController::~CWatcherController() {
    if (filename != "")
        LOG_FILE_CLOSE();
}

/****************************************/
/****************************************/

void CWatcherController::ParseParams(TConfigurationNode& t_node) {
    try {
        /* Left wheel speed */
        GetNodeAttributeOrDefault(t_node, "LWS", m_sLeftWheelSpeed, m_sLeftWheelSpeed);
        /* Right wheel speed */
        GetNodeAttributeOrDefault(t_node, "RWS", m_sRightWheelSpeed, m_sRightWheelSpeed);
        LOG << "[PARAMS] LWS: " << m_sLeftWheelSpeed << " | RWS: " << m_sRightWheelSpeed << std::endl;
        GetNodeAttributeOrDefault(t_node, "log_file", filename, filename);
        GetNodeAttributeOrDefault(t_node, "delay", m_nDelay, m_nDelay);
        GetNodeAttributeOrDefault(t_node, "range", m_nRange, m_nRange);
        GetNodeAttributeOrDefault(t_node, "data_size", m_nDataSize, m_nDataSize);
        GetNodeAttributeOrDefault(t_node, "distance", m_nDistance, m_nDistance);
        GetNodeAttributeOrDefault(t_node, "run", m_nRun, m_nRun);
        if (filename != "") {
            LOG << "[INFO]\tfilename: " << filename << std::endl;
            LOG_FILE_INIT(filename.c_str());
            LOG_FILE("my_id\tmy_ts\tother_id\tother_ts\trange\tbearing\tdelay\trange\tdatasize\tdistance\trun\n");
            LOG << "[PARAMS] log file: " << filename << std::endl;
        }
    } catch (CARGoSException& ex) {
        THROW_ARGOSEXCEPTION_NESTED("Error parsing <params>", ex);
    }
}

/****************************************/
/****************************************/

void CWatcherController::Init(TConfigurationNode& t_node) {
    /* parse the xml tree <params> */
    ParseParams(t_node);
    m_uRobotID = atoi(GetId().substr(5, 6).c_str());
    /* actuators */
    try {
        m_pcWheelsActuator = GetActuator<CCI_EPuckWheelsActuator>("epuck_wheels");
    } catch (CARGoSException ex) {}
    try {
        m_pcLEDsActuator = GetActuator<CCI_EPuckBaseLEDsActuator>("epuck_base_leds");
    } catch (CARGoSException ex) {}
    try {
        m_pcRGBLEDsActuator = GetActuator<CCI_EPuckRGBLEDsActuator>("epuck_rgb_leds");
    } catch (CARGoSException ex) {}
    try {
        m_pcRABActuator = GetActuator<CCI_EPuckRangeAndBearingActuator>("epuck_range_and_bearing");
    } catch (CARGoSException ex) {}
    try {
        m_pcIRComActuator = GetActuator<CCI_EPuckIRComActuator>("epuck_ircom");
    } catch (CARGoSException ex) {}
    /* sensors */
    try {
        m_pcProximitySensor = GetSensor<CCI_EPuckProximitySensor>("epuck_proximity");
    } catch (CARGoSException ex) {}
    try {
        m_pcLightSensor = GetSensor<CCI_EPuckLightSensor>("epuck_light");
    } catch (CARGoSException ex) {}
    try {
        m_pcGroundSensor = GetSensor<CCI_EPuckGroundSensor>("epuck_ground");
    } catch (CARGoSException ex) {}
    try {
        m_pcRABSensor = GetSensor<CCI_EPuckRangeAndBearingSensor>("epuck_range_and_bearing");
    } catch (CARGoSException ex) {}
    try {
        m_pcIRComSensor = GetSensor<CCI_EPuckIRComSensor>("epuck_ircom");
    } catch (CARGoSException ex) {}
    try {
        m_pcCamera = GetSensor<CCI_EPuckOmnidirectionalCameraSensor>("epuck_omnidirectional_camera");
    } catch (CARGoSException ex) {}
}

/****************************************/
/****************************************/

void CWatcherController::ControlStep() {
    /* increment the control step count */
    ++m_nControlStep;
    if (filename == "")
        printf("[INFO]\t\trobot: %i, cs: %i\n", m_uRobotID, m_nControlStep);
    if (m_pcWheelsActuator != NULL) {
        /* make the robot turn clockwise on itself */
        m_pcWheelsActuator->SetLinearVelocity(m_sLeftWheelSpeed, m_sRightWheelSpeed);
    }
    if (m_pcLEDsActuator != NULL) {
        if(m_pcCamera ==NULL){
            /* puts one of the 8 base LED on */
            m_pcLEDsActuator->SwitchLED(m_nControlStep % 8, true);
            // /* puts the previous base LED off */
            m_pcLEDsActuator->SwitchLED((m_nControlStep - 1) % 8, false);
        }
    }
    if (m_pcRGBLEDsActuator != NULL) {
        /* puts one of the 3 RGB LED blue */
        m_pcRGBLEDsActuator->SetColor(m_nControlStep % 3, CColor::BLUE);
        /* puts the previous RGB LED black */
        m_pcRGBLEDsActuator->SetColor((m_nControlStep - 1) % 3, CColor::BLACK);
    }
    if (m_pcRABActuator != NULL) {
        UInt8 data[4];
        /* sends the control id with the RAB */
        data[0] = m_uRobotID;
        /* sends in the same time the control step */
        data[1] = m_nControlStep;
        data[2] = 3;
        data[3] = 4;
        m_pcRABActuator->SetData(data);
    }
    if (m_pcIRComActuator != NULL) {
        /* send the current id of the robot with IRCom */
        m_pcIRComActuator->SetData(m_uRobotID);
    }
    if (m_pcIRComSensor != NULL) {
        printf("[IRCom]\t\t");
        const CCI_EPuckIRComSensor::TPackets& packets = m_pcIRComSensor->GetPackets();
        for (size_t i = 0; i < packets.size(); ++i) {
            printf("(%f, %.2f, %i, %i) ",
                   ToDegrees(packets[i]->Bearing).GetValue(),
                   packets[i]->Range,
                   packets[i]->Sensor,
                   packets[i]->Data);
        }
        m_pcIRComSensor->ClearPackets();
        printf("\n");
    }
    if (m_pcRABSensor != NULL) {
        const CCI_EPuckRangeAndBearingSensor::TPackets& packets = m_pcRABSensor->GetPackets();
        if (filename == "")
            printf("[RAB]\t\t");
        for (size_t i = 0; i < packets.size(); ++i)
            if (packets[i]->Data[0] != m_uRobotID) {
                if (filename != "") {
                    char rab_msg[50];
                    sprintf(rab_msg, "%i\t%i\t%i\t%i\t%i\t%.2f\t%.2f\t%i\t%i\t%i\t%i\t%i\n",
                            m_uRobotID,
                            m_nControlStep,
                            packets[i]->Data[0],
                            packets[i]->Data[1],
                            packets[i]->Data[2],
                            packets[i]->Range,
                            ToDegrees(packets[i]->Bearing).GetValue(),
                            m_nDelay,
                            m_nRange,
                            m_nDataSize,
                            m_nDistance,
                            m_nRun);
                    LOG << "[RAB]\t" << rab_msg << std::endl;
                    LOG_FILE(rab_msg);
                } else {
                    printf("(%f, %f, %i, %i, %i, %i) ",
                           packets[i]->Range,
                           ToDegrees(packets[i]->Bearing).GetValue(),
                           packets[i]->Data[0],
                            packets[i]->Data[1],
                            packets[i]->Data[2],
                            packets[i]->Data[3]);
                }
            }
        if (filename == "")
            printf("\n");
        m_pcRABSensor->ClearPackets();
    }
    if (m_pcProximitySensor != NULL) {
        /* prints the proximity sensors readings */
        printf("[PROXIMITY]\t");
        for(size_t i = 0; i < 8; ++i)
            printf("(%6.2f - %6.2f) ", m_pcProximitySensor->GetReading(i).Value,
                   ToDegrees(m_pcProximitySensor->GetReading(i).Angle).GetValue());
        printf("\n");
    }
    if (m_pcLightSensor != NULL) {
        /* prints the light sensors readings */
        printf("[LIGHT]\t\t");
        for(size_t i = 0; i < 8; ++i)
            printf("(%6.2f, %6.2f) ", m_pcLightSensor->GetReading(i).Value,
                   ToDegrees(m_pcLightSensor->GetReading(i).Angle).GetValue());
        printf("\n");
    }
    if (m_pcGroundSensor != NULL) {
        /* prints the ground sensors readings */
        printf("[GROUND]\t%.2f\t%.2f\t%.2f\n",
               m_pcGroundSensor->GetReadings().Left,
               m_pcGroundSensor->GetReadings().Center,
               m_pcGroundSensor->GetReadings().Right);
        printf("\n\n");
    }

    if (m_pcCamera != NULL) {
        /* prints the blobs */
        const CCI_EPuckOmnidirectionalCameraSensor::SReadings* m_sCameraReadings = &m_pcCamera->GetReadings();
        std::vector<CCI_EPuckOmnidirectionalCameraSensor::SBlob*> allBlobs = m_sCameraReadings->BlobList;
        LOG << "CAMERA CYCLES " << m_sCameraReadings->Counter << "" << std::endl;
        LOG << "Found " << allBlobs.size() << " blobs." << std::endl;
        if(allBlobs.size() == 0){
            LOG<< "NO BLOB FOUND" << std::endl;
            m_pcLEDsActuator->SwitchLEDs(false);
        }else{
            UInt32 max_area=0;
            UInt32 led_on=0;
            for (std::vector<CCI_EPuckOmnidirectionalCameraSensor::SBlob*>::iterator it = allBlobs.begin(); it != allBlobs.end(); ++it) {
                if(max_area<(*it)->Area){
                    led_on=(*it)->Angle.UnsignedNormalize().GetValue() * 8.0f/CRadians::TWO_PI.GetValue();
                    max_area=(*it)->Area;
                }
                LOG<< "Color: " << (*it)->Color << std::endl;
                LOG<< "Distance: " << (*it)->Distance << std::endl;
                LOG<< "Area: " << (*it)->Area<< std::endl;
                LOG<< "Angle: " << (*it)->Angle.UnsignedNormalize().GetValue() << std::endl;
                LOG<< "---------------------------------------------------------------" << std::endl;
            }
            m_pcLEDsActuator->SwitchLEDs(false);
            m_pcLEDsActuator->SwitchLED(led_on, true);
        }
    }
}

/****************************************/
/****************************************/

REGISTER_CONTROLLER(CWatcherController, "watcher_controller");

/**
 * @file <argos3/plugins/robots/e-puck/real_robot/real_epuck.cpp>
 *
 * This class provides the real e-puck robot implementation. In
 * particular there is the implementation of the sensors and actuators
 * creation and the serial data communication.
 * @author Gianpiero Francesca <gianpiero.francesca@ulb.ac.be>
 * @author Lorenzo Garattoni <lgaratto@ulb.ac.be>
 */
#include "real_epuck.h"

#include <argos3/core/utility/configuration/argos_exception.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

namespace argos {

const std::string DEVICE = "/dev/ttyS0";
const speed_t BAUD_RATE = B115200;

/****************************************/
/****************************************/


bool CRealEPuck::bGoHome() const
{
    return m_bGoHome;
}

void CRealEPuck::setBGoHome(bool bGoHome)
{
    m_bGoHome = bGoHome;
}
CRealEPuck& CRealEPuck::GetInstance() {
    static std::auto_ptr<CRealEPuck> pcRealEPuckInstance(new CRealEPuck);
    return *(pcRealEPuckInstance.get());
}

/****************************************/
/****************************************/

CRealEPuck::CRealEPuck() :
    m_bGoHome(false),
    m_bTimeLimitSet(false),
    m_unControlStepDuration(0),
    m_unTimeBeforeControlStep(0),
    m_unTimeAfterControlStep(0),
    m_nPortFileDescriptor(-1),
    m_cRandomRange(0, 10000),
    m_eBoardState(WAITING)
{
    ::memset(&m_sActuatorState, 0, sizeof(BaseActuatorState));
}

/****************************************/
/****************************************/

CRealEPuck::~CRealEPuck() {

    /* termination requested by os (kill) */
    signal(SIGTERM, SIG_IGN);
    /* user wishes to interrupt the process */
    signal(SIGINT, SIG_IGN);
    /* immediate termination */
    signal(SIGKILL,SIG_IGN);
    signal(SIGSEGV,SIG_IGN);

    ResetPic30();
    switch(m_eBoardState) {
    case WAITING:
    case INITIALIZATION:
    case RECEIVE:
        break;
    case WAKE_UP:
    {
        LOGERR << "[WARNING] Interruption while waking up, " << m_eBoardState << std::endl;
        SendSensorConfiguration();
        ReceiveSensorData();
        break;
    }
    case SENDING_CONFIGURATION:
    {
        LOGERR << "[WARNING] Interruption while sending configuration, " << m_eBoardState << std::endl;
        ReceiveSensorData();
        break;
    }
    case SEND:
    {
        LOGERR << "[WARNING] Interruption while sending data, " << m_eBoardState << std::endl;
        ReceiveSensorData();
        // ReceiveDataFromPic<BaseSensorState>(m_sSensorState);
        break;
    }
    }
    SendActuatorData();
    DestroySerial();
    if (CRandom::ExistsCategory("argos")) {
        CRandom::RemoveCategory("argos");
    }

    LOG << "[INFO] Destroying controller" << std::endl;
    delete m_pcController;
    LOG << "[INFO] Destruction completed" << std::endl;

}

/****************************************/
/****************************************/

void CRealEPuck::Init(const std::string& str_config_file_name,
                      const std::string& str_controller_id) {
    RegisterSystemSignalHandlers();
    InitSerial();
    CRealEPuckI2CDevice::Init();
    WakeUpPic();
    SendSensorConfiguration();
    InitController(str_config_file_name, str_controller_id);
}

/****************************************/
/****************************************/

void CRealEPuck::SendSensorConfiguration() {
    switch (m_eBoardState){
    case WAITING:
    case INITIALIZATION:
    case SENDING_CONFIGURATION:
    case SEND:
    case RECEIVE:
    {
        LOGERR << "[WARNING] Out-of-sync operation: SendSensorConfiguration(), " << m_eBoardState << std::endl;
        break;
    }
    case WAKE_UP:
    {
        m_eBoardState = SENDING_CONFIGURATION;
        SendDataToPic<BaseSensorConfiguration>(m_sSensorConfiguration);
        break;
    }
    }
}

/****************************************/
/****************************************/

void CRealEPuck::SendActuatorData() {
    switch (m_eBoardState) {
    case SENDING_CONFIGURATION:
    case WAITING:
    case INITIALIZATION:
    case WAKE_UP:
    case SEND:
    {
        LOGERR << "[WARNING] Out-of-sync operation: SendActuatorData(), " << m_eBoardState << std::endl;
        break;
    }
    case RECEIVE:
    {
        /* Send data to the I2C bus */
        for(size_t i = 0; i < m_vecI2CActuators.size(); ++i) {
            m_vecI2CActuators[i]->SendData();
        }
        /* Send data to the serial bus */
        SendDataToPic<BaseActuatorState>(m_sActuatorState);
        /* Set new state */
        m_eBoardState = SEND;
        break;
    }
    }
}

/****************************************/
/****************************************/

/*
    *   Reads sensors data via serial communication and stores them in
    *    the shared BaseSensorState structure.  Not used for i2c sensors
    *    (ground and cam sensors for the time being)
    */
void CRealEPuck::ReceiveSensorData() {
    switch (m_eBoardState) {
    case WAITING:
    case INITIALIZATION:
    case WAKE_UP:
    case RECEIVE:
    {
        LOGERR << "[WARNING] Out-of-sync operation: ReceiveSensorData(), "  << m_eBoardState << std::endl;
        break;
    }
    case SENDING_CONFIGURATION:
    case SEND:
    {
        /* Receive data from the I2C bus */
        for(size_t i = 0; i < m_vecI2CSensors.size(); ++i) {
            m_vecI2CSensors[i]->ReceiveData();
        }
        /* Receive data from the serial bus */
        ReceiveDataFromPic<BaseSensorState>(m_sSensorState);
        m_eBoardState = RECEIVE;
        break;
    }
    }
}

/****************************************/
/****************************************/

void CRealEPuck::UpdateValues() {
    /*Update camera*/
    if(m_pcOmnidirectionalCameraSensor!=NULL){
        m_pcOmnidirectionalCameraSensor->Update();
    }
    for(size_t i = 0; i < m_vecSerialSensors.size(); ++i) {
        m_vecSerialSensors[i]->UpdateValues();
    }
}

/****************************************/
/****************************************/

/*
    *   Creates all the actuators defined by the user in the xml
    *   configuration file.
    */
CCI_Actuator* CRealEPuck::InsertActuator(const std::string& str_actuator_name) {
    if (str_actuator_name == "epuck_wheels") {
        CRealEPuckWheelsActuator* pcWheelsActuator = CreateActuator<CRealEPuckWheelsActuator>(str_actuator_name);
        pcWheelsActuator->SetState(m_sActuatorState);
        return pcWheelsActuator;
    } else if (str_actuator_name == "epuck_base_leds") {
        CRealEPuckBaseLEDsActuator* pcLEDsActuator = CreateActuator<CRealEPuckBaseLEDsActuator>(str_actuator_name);
        pcLEDsActuator->SetState(m_sActuatorState);
        return pcLEDsActuator;
    } else if (str_actuator_name == "epuck_rgb_leds") {
        CRealEPuckRGBLEDsActuator* pcRGBLEDsActuator = CreateActuator<CRealEPuckRGBLEDsActuator>(str_actuator_name);
        m_vecI2CActuators.push_back(pcRGBLEDsActuator);
        return pcRGBLEDsActuator;
    } else if (str_actuator_name == "epuck_range_and_bearing") {
        CRealEPuckRangeAndBearingActuator* pcRABActuator = CreateActuator<CRealEPuckRangeAndBearingActuator>(str_actuator_name);
        m_vecI2CActuators.push_back(pcRABActuator);
        return pcRABActuator;
    } else if (str_actuator_name == "epuck_virtual_camrab") {     //@antoine
        CRealEPuckVirtualCamrabActuator* pcVirtualCamrabActuator = CreateActuator<CRealEPuckVirtualCamrabActuator>(str_actuator_name);
        m_vecI2CActuators.push_back(pcVirtualCamrabActuator);
        return pcVirtualCamrabActuator;
    } else if (str_actuator_name == "epuck_ircom") {
        CRealEPuckIRComActuator* pcIRComActuator = CreateActuator<CRealEPuckIRComActuator>(str_actuator_name);
        pcIRComActuator->SetState(m_sActuatorState);
        return pcIRComActuator;
    } else {
        THROW_ARGOSEXCEPTION("Unknown actuator \"" << str_actuator_name << "\"");
    }
}

/****************************************/
/****************************************/

/*
    *   Creates all the sensors defined by the user in the xml
    *   configuration file.
    */
CCI_Sensor* CRealEPuck::InsertSensor(const std::string& str_sensor_name) {
    if (str_sensor_name == "epuck_proximity") {
        CRealEPuckProximitySensor* pcProximitySensor = CreateSensor<CRealEPuckProximitySensor>(str_sensor_name);
        pcProximitySensor->SetState(m_sSensorState);
        m_vecSerialSensors.push_back(pcProximitySensor);
        return pcProximitySensor;
    } else if (str_sensor_name == "epuck_light") {
        CRealEPuckLightSensor* pcLightSensor = CreateSensor<CRealEPuckLightSensor>(str_sensor_name);
        pcLightSensor->SetState(m_sSensorState);
        m_vecSerialSensors.push_back(pcLightSensor);
        return pcLightSensor;
    } else if (str_sensor_name == "epuck_ground") {
        CRealEPuckGroundSensor* pcGroundSensor = CreateSensor<CRealEPuckGroundSensor>(str_sensor_name);
        m_vecI2CSensors.push_back(pcGroundSensor);
        return pcGroundSensor;
    } else if (str_sensor_name == "epuck_accelerometer") {
        CRealEPuckAccelerometerSensor* pcAccelerometerSensor = CreateSensor<CRealEPuckAccelerometerSensor>(str_sensor_name);
        pcAccelerometerSensor->SetState(m_sSensorState);
        m_vecSerialSensors.push_back(pcAccelerometerSensor);
        return pcAccelerometerSensor;
    } else if (str_sensor_name == "epuck_range_and_bearing") {
        CRealEPuckRangeAndBearingSensor* pcRABSensor = CreateSensor<CRealEPuckRangeAndBearingSensor>(str_sensor_name);
        m_vecI2CSensors.push_back(pcRABSensor);
        return pcRABSensor;
    } else if (str_sensor_name == "epuck_ircom") {
        CRealEPuckIRComSensor* pcIRComSensor = CreateSensor<CRealEPuckIRComSensor>(str_sensor_name);
        pcIRComSensor->SetState(m_sSensorState);
        m_vecSerialSensors.push_back(pcIRComSensor);
        return pcIRComSensor;
    } else if (str_sensor_name == "epuck_micro") {
        CRealEPuckMicroSensor* pcMicroSensor = CreateSensor<CRealEPuckMicroSensor>(str_sensor_name);
        pcMicroSensor->SetState(m_sSensorState);
        m_vecSerialSensors.push_back(pcMicroSensor);
        return pcMicroSensor;
    } else if (str_sensor_name == "epuck_omnidirectional_camera") {
        m_pcOmnidirectionalCameraSensor = CreateSensor<CRealEPuckOmnidirectionalCameraSensor>(str_sensor_name);
        return m_pcOmnidirectionalCameraSensor;    
    } else if (str_sensor_name == "epuck_virtual_camrab") {
        CRealEPuckVirtualCamrabSensor* pcVirtualCamrabSensor = CreateSensor<CRealEPuckVirtualCamrabSensor>(str_sensor_name);
        m_vecI2CSensors.push_back(pcVirtualCamrabSensor);
        return pcVirtualCamrabSensor;
    }if (str_sensor_name == "epuck_battery") {
        CRealEPuckBatterySensor* pcBatterySensor = CreateSensor<CRealEPuckBatterySensor>(str_sensor_name);
        pcBatterySensor->SetState(m_sSensorState);
        m_vecSerialSensors.push_back(pcBatterySensor);
        return pcBatterySensor;
    } else {
        THROW_ARGOSEXCEPTION("Unknown sensor \"" << str_sensor_name << "\"");
    }
}

/****************************************/
/****************************************/

bool CRealEPuck::IsExperimentFinished() {
    if (m_sActuatorState.Reset == 'R') {
        /* if reset button was pressed, control step is over */
        LOG << "[INFO] Reset button pressed" << std::endl;
        return true;
    }
    if(m_bTimeLimitSet) {
        /* if we have a time limited experience, must check if it's the end */
        if (m_unExperimentStepsLeft <= 0) {
            /* no time left in this experiment */
            LOG << "[INFO] Experiment end" << std::endl;
            /* calculate the time ellapsed from the start of the
             * experiment til the end (now)
             */
            UInt64 unTimeEllapsed = GetTime() - m_unStartTimeExperiment;
            /* if the time ellapsed (in ms) is greater than the expected time
             * warn the user. Need to convert the ExpDuration from s to ms.
             */
            if (unTimeEllapsed > m_unExpDuration * 1000llu) {
                LOGERR << "[WARNING] Experiment length is greater than expected: ["
                       << unTimeEllapsed
                       << " ms], instead of: ["
                       << m_unExpDuration * 1000llu
                       << " ms]." << std::endl;
            }
            return true;
        }
    }
    return false;
}

/****************************************/
/****************************************/

/* this function ensure that the control step is synchronized with the
    * ticks rythm imposed by the xml configuration file
    */
void CRealEPuck::SyncControlStep() {
    /* if it's the start of the experiment (no sync ran yet) */
    if (m_unTimeBeforeControlStep == 0) {
        /* set the time before control step */
        m_unTimeBeforeControlStep = GetTime();
        /* set the time of the start of the experiment */
        m_unStartTimeExperiment = m_unTimeBeforeControlStep;
    } else {

        /* have to decrease the time left for the experience by the
          * step duration if there is a time limit
          */
        if (m_bTimeLimitSet) {
            --m_unExperimentStepsLeft;
        }

        /* take the current time */
        m_unTimeAfterControlStep = GetTime();

        /*
          * calculate the time between the last call of this function and
          * now to get the duration of the current step.
          */
        UInt64 unCurrentStepDuration =
                m_unTimeAfterControlStep - m_unTimeBeforeControlStep;

        /* If the time elapsed between the control step start and end
          * is shorter than the target control step duration, wait for
          * the necessary time to complete the control step duration
          * time
          */
        if (unCurrentStepDuration < m_unControlStepDuration) {
            /* calculate the time left to be synchronized with the ticks */
            UInt64 unWaitTime =
                    m_unControlStepDuration - unCurrentStepDuration;
            /* wait the correct amount of time to be synchronized */
            ::usleep(1000 * unWaitTime);
        } else if (unCurrentStepDuration > m_unControlStepDuration) {
            /*
             * we are not in time, we are over the normal step duration
             */
            LOGERR << "[WARNING] Too much time in the control step. "
                   << unCurrentStepDuration
                   << " taken and the control step interval is "
                   << m_unControlStepDuration
                   << std::endl;
        }
        /* else, no sync needed, we are just in time */
        m_unTimeBeforeControlStep = GetTime();
    }
}

/****************************************/
/****************************************/

void CRealEPuck::InitSerial() {
    switch (m_eBoardState){
    case INITIALIZATION:
    case WAKE_UP:
    case SENDING_CONFIGURATION:
    case RECEIVE:
    case SEND:
    {
        LOGERR << "[WARNING] Out-of-sync operation: InitSerial(), " << m_eBoardState << std::endl;
        break;
    }
    case WAITING:
    {
        m_eBoardState = INITIALIZATION;
        /*Try to open serial link*/

        /* open the serial file descriptor */
        m_nPortFileDescriptor = ::open(DEVICE.c_str(), O_RDWR | O_EXCL);

        if (m_nPortFileDescriptor < 0) {
            THROW_ARGOSEXCEPTION("Unable to open serial port '"
                                 << DEVICE << "': "
                                 << strerror(errno));
        }

        /* get parameters of the I/O file descriptor */
        if (::tcgetattr(m_nPortFileDescriptor, &tAttr) < 0) {
            THROW_ARGOSEXCEPTION("Call to tcgetattr() failed: "
                                 << strerror(errno));
        }

        /* change some parameters of the I/O file descriptor attributes */
        tAttr.c_iflag = 0;
        tAttr.c_oflag = 0;
        tAttr.c_cflag = CLOCAL | CREAD | CS8;
        tAttr.c_lflag = 0;
        tAttr.c_cc[VTIME] = 0; // timeout in tenths of a second
        tAttr.c_cc[VMIN] = 1; // Only wait for a single char

        /* set the input baud rate stored into tAttr to BAUD_RATE */
        ::cfsetispeed(&tAttr, BAUD_RATE);
        /* set the output baud rate stored into tAttr to BAUD_RATE */
        ::cfsetospeed(&tAttr, BAUD_RATE);

        if ((::tcsetattr(m_nPortFileDescriptor, TCSAFLUSH, &tAttr) < 0)) {
            THROW_ARGOSEXCEPTION("Call to tcsetattr() failed: " << strerror(errno));
        }

        /* make sure to not reset the pic */
        m_sActuatorState.Reset ='N';
        break;
    }
    }
}

/****************************************/
/****************************************/

void CRealEPuck::WakeUpPic() {
    switch (m_eBoardState){
    case WAITING:
    case WAKE_UP:
    case SENDING_CONFIGURATION:
    case SEND:
    case RECEIVE:
    {
        LOGERR << "[WARNING] Out-of-sync operation: WakeUpPic(), "  << m_eBoardState << std::endl;
        break;
    }
    case INITIALIZATION:
    {
        m_eBoardState = WAKE_UP;
        SendDataToPic<UInt8>(WAKE_UP_PIC_SYMBOL);
        break;
    }
    }
}

/****************************************/
/****************************************/

void CRealEPuck::DestroySerial() {
    if (m_nPortFileDescriptor > 0) {
        /* reset the old terminal settings */
        ::tcsetattr(m_nPortFileDescriptor, TCSAFLUSH, &tAttr);

        ::close(m_nPortFileDescriptor);
        LOG << "[INFO] Serial communication closed"
            << std::endl;
    } else {
        LOG << "[INFO] Trying to close serial but file descriptor not set"
            << std::endl;
    }
}

/****************************************/
/****************************************/

/*
    * Parse the configuration file of the robot;
    * a xml file containing sensors/actuators/etc.
    */
void CRealEPuck::InitController(const std::string& str_config_file_name,
                                const std::string& str_controller_id) {
    try {
        /* creation of the xml document with the path */
        m_tXMLConfiguration = ticpp::Document(str_config_file_name);
        /* loading the file */
        m_tXMLConfiguration.LoadFile();
    } catch (CARGoSException& ex) {
        THROW_ARGOSEXCEPTION("Error loading xml file ["
                             << str_config_file_name << "]");
    }

    TConfigurationNode tConfigurationRoot;
    try {
        /* get the root node of the document (<argos-configuration> in xml) */
        tConfigurationRoot = *m_tXMLConfiguration.FirstChildElement();
    } catch (CARGoSException& ex) {
        THROW_ARGOSEXCEPTION("Error parsing the xml file, need a root node <argos-configuration>");
    }

    TConfigurationNode tFrameworkNode;
    try {
        /* get the controller node inside the root node (<framework>) */
        tFrameworkNode = GetNode(tConfigurationRoot, "framework");
    } catch (CARGoSException& ex) {
        THROW_ARGOSEXCEPTION("Error parsing the xml file, need a node <framework> "
                             << "inside the root node <argos-configuration>");
    }

    TConfigurationNode tExperiment;
    try {
        /* get the experiment node inside the framework node (<experiment>) */
        tExperiment = GetNode(tFrameworkNode, "experiment");
    } catch (CARGoSException& ex) {
        THROW_ARGOSEXCEPTION("Error parsing the xml file, need a node <experiment> "
                             << "inside the node <framework>");
    }

    TConfigurationNode tControllersNode;
    try {
        /* get the controller node inside the root node (<controllers> in xml)*/
        tControllersNode = GetNode(tConfigurationRoot, "controllers");
    } catch (CARGoSException& ex) {
        THROW_ARGOSEXCEPTION("Error parsing the xml file, need a node <controllers> "
                             << "inside the root node <argos-configuration>");
    }

    try {
        /*
          * extract if it exist the param "length" (in seconds) in the
          * node <experiment> and set the variable m_nExpDuration with
          * it
          */
        GetNodeAttributeOrDefault(tExperiment,
                                  "length",
                                  m_unExpDuration,
                                  m_unExpDuration);
        if(m_unExpDuration > 0) {
            m_bTimeLimitSet = true;
            LOG << "[INFO] Experiment duration "
                << m_unExpDuration << " sec" << std::endl;;
        } else {
            m_bTimeLimitSet = false;
            LOG << "[INFO] Experiment has no duration limit"
                << std::endl;;
        }

        /*
          * 1000 ms (= 1s) divided by the number of ticks per second
          * give us the step duration
          */
        UInt32 unTicksPerSecond;
        GetNodeAttribute(tExperiment,
                         "ticks_per_second",
                         unTicksPerSecond);
        m_unControlStepDuration = 1000 / unTicksPerSecond;
        if(m_unControlStepDuration <= 0) {
            THROW_ARGOSEXCEPTION("The ticks per second cannot be 0 or a negative value");
        }
        LOG << "[INFO] Control step duration "
            << m_unControlStepDuration
            << " ms" << std::endl;;

        /* calculate the number of steps we will do in this experiment */
        m_unExperimentStepsLeft = m_unExpDuration * unTicksPerSecond;

        LOG << "[INFO] Number of steps to be done: "
            << m_unExperimentStepsLeft << std::endl;
        /*
          * Create Random category argos
          */
        UInt32 unRandomSeed = 0;
        GetNodeAttributeOrDefault<UInt32>(tExperiment,
                                          "random_seed",
                                          unRandomSeed,
                                          unRandomSeed);
        /* If random seed is 0 or is not specified, init with the
            current timeval */
        if (unRandomSeed == 0) {
            unRandomSeed = static_cast<UInt32>(GetTime());
        }

        /* create category (already exist by default)
          * with a custom seed */
        CRandom::CreateCategory("argos", unRandomSeed);

        /* when the custom category is created, get the RNG */
        m_pcRNG = CRandom::CreateRNG("argos");

        LOG << "[INFO] Using random seed = "
            << unRandomSeed << std::endl;
    }
    catch(CARGoSException& ex) {
        THROW_ARGOSEXCEPTION_NESTED("Error parsing attributes of the node <experiment>", ex);
    }

    try {
        /* Create the controller */
        TConfigurationNode tControllerNode = CreateController(str_controller_id, tControllersNode);
        LoadActuators(tControllerNode);
        LoadSensors(tControllerNode);
        InitController(tControllerNode);
    } catch (CARGoSException& ex) {
        THROW_ARGOSEXCEPTION_NESTED("Error creating the controller with the config file" , ex);
    }
}

/****************************************/
/****************************************/

TConfigurationNode CRealEPuck::CreateController(const std::string& str_controller_id,
                                                TConfigurationNode& c_controllers_subtree) {
    // Look for a controller with id equal to that stored in
    // str_controller_id
    std::string strControllerLabel; // to store the name of the
    // controller with the wanted
    // id
    TConfigurationNode tControllerTree; // to store the XML tree
    // of the controller with
    // the wanted id

    std::string strId; // to search for the controller with
    // the wanted id

    // Cycle through the controllers
    TConfigurationNodeIterator tControllerIterator;
    tControllerIterator =
            tControllerIterator.begin(&c_controllers_subtree);

    // Stop when the end is reached or when the wanted id has been found
    while(tControllerIterator != tControllerIterator.end()
          && strId != str_controller_id) {
        try {
            // Get the current node id
            GetNodeAttribute(*tControllerIterator, "id", strId);
            if (strId == str_controller_id) {
                /* controller found with the good id */
                /* store the controller's name */
                strControllerLabel = tControllerIterator->Value();
                // store the controller's tree
                tControllerTree = *tControllerIterator;
                /* no need to search more */
                break;
            } else {
                // Not found, try the next controller
                ++tControllerIterator;
            }
        } catch (CARGoSException& ex) {
            THROW_ARGOSEXCEPTION("Invalid controller found, attribute id mandatory in <"
                                 << tControllerIterator->Value() << ">");
        }
    }

    if (strControllerLabel.empty())
        THROW_ARGOSEXCEPTION("A controller with id \""
                             << str_controller_id
                             << "\" has not been found");

    // Create the controller (need to be here, so we can add sensors/actuators to it directly)
    m_pcController = ControllerMaker(strControllerLabel);
    /* Return the controller */
    return tControllerTree;
}


void CRealEPuck::InitController(TConfigurationNode& c_controller_tree) {
    if (m_pcController != NULL) {
        // Get the parameter tree
        TConfigurationNode tParameters;
        try {
            /* try to find the node <params> in the controller */
            tParameters = GetNode(c_controller_tree, "params");
        } catch (CARGoSException& ex) {
            /* if not found, create and empty one */
            ticpp::Element element("params");
            tParameters = element;
        }
        /* Set the controller id */
        SetId(GetController());

        /* Init the controller with the <params> node */
        GetController().Init(tParameters);
    } else {
        THROW_ARGOSEXCEPTION("No controller, impossible to init controller");
    }
}

/****************************************/
/****************************************/

void CRealEPuck::LoadActuators(TConfigurationNode& c_controller_tree) {
    if (m_pcController != NULL) {
        // used to store the name of the actuator
        std::string strActuatorName;
        // Cycle through the tags in the "actuators" subtree
        TConfigurationNode& tActuators = GetNode(c_controller_tree, "actuators");
        TConfigurationNodeIterator tActuatorIterator;
        for (tActuatorIterator = tActuatorIterator.begin(&tActuators);
             tActuatorIterator != tActuatorIterator.end();
             ++tActuatorIterator) {
            // get the name of the actuator
            strActuatorName = tActuatorIterator->Value();
            // create the actuator
            LOG << "[INFO] Inserting actuator \"" << strActuatorName << "\"..." << std::endl;
            CCI_Actuator* pcActuator = InsertActuator(strActuatorName);
            /* init the sensor with its node */
            pcActuator->Init(*tActuatorIterator);
            LOG << "[INFO] Actuator \""
                << strActuatorName
                << "\" created."
                << std::endl;
        }
    } else {
        THROW_ARGOSEXCEPTION("No controller, impossible to load actuators");
    }
}

/****************************************/
/****************************************/

void CRealEPuck::LoadSensors(TConfigurationNode& c_controller_tree) {
    if (m_pcController != NULL) {
        // used to store the name of the sensor
        std::string strSensorName;
        // Cycle through the the tags in the "sensors" subtree
        TConfigurationNode& tSensors = GetNode(c_controller_tree, "sensors");
        TConfigurationNodeIterator tSensorIterator;
        for(tSensorIterator = tSensorIterator.begin(&tSensors);
            tSensorIterator != tSensorIterator.end();
            ++tSensorIterator) {
            // get the name of the sensor
            strSensorName = tSensorIterator->Value();
            LOG << "[INFO] Inserting sensor \"" << strSensorName << "\"..." << std::endl;
            // create the sensor
            CCI_Sensor* pcSensor = InsertSensor(strSensorName);
            /* init the sensor with its node */
            pcSensor->Init(*tSensorIterator);
            LOG << "[INFO] Sensor \""
                << strSensorName
                << "\" created."
                << std::endl;
        }
    } else {
        THROW_ARGOSEXCEPTION("No controller, impossible to load actuators");
    }
}

/****************************************/
/****************************************/

void CRealEPuck::RegisterSystemSignalHandlers() {
    /* termination requested by os (kill) */
    signal(SIGTERM, SignalHandler);
    /* user wishes to interrupt the process */
    signal(SIGINT, SignalHandler);
    /* immediate termination */
    signal(SIGKILL,SignalHandler);
    signal(SIGSEGV,SignalHandler);
    /* TODO handle segfault */
}

/****************************************/
/****************************************/

void CRealEPuck::SignalHandler(SInt32 n_signal) {
    if(!GetInstance().bGoHome()){
        GetInstance().setBGoHome(true);
    }
    static bool bSigHandlerRunning = false;
    if (bSigHandlerRunning) {
        LOG << "Stop pressing CTRL-C. Something nasty happend. To close the controller:\n1-Type CTRL-\\.\n 2- Reset the PIC." << std::endl;
    }
    bSigHandlerRunning = true;
    LOG << "[INFO] Controller termination due to signal: " << n_signal << std::endl;

}

/****************************************/
/****************************************/

void CRealEPuck::ResetPic30() {
    memset(&m_sActuatorState, 0, sizeof (BaseActuatorState));
    m_sActuatorState.Reset = 'R';
}

/****************************************/
/****************************************/

UInt64 CRealEPuck::GetTime() {
    timeval tTimeVal;
    gettimeofday(&tTimeVal, NULL);
    UInt64 unSeconds = tTimeVal.tv_sec;
    UInt64 unMicroSeconds = tTimeVal.tv_usec;
    UInt64 unResult = unSeconds * 1000ull + unMicroSeconds / 1000ull;
    return unResult;
}

/****************************************/
/****************************************/

void CRealEPuck::SetId(CCI_Controller& c_controller) {
    // set the robot id to the hostname of this robot
    char pchBuffer[30];
    if (::gethostname(pchBuffer, 30) == 0) {
        LOG << "[INFO] Setting robot id to its hostname '"
            << pchBuffer << "'."
            << std::endl;
        c_controller.SetId(std::string(pchBuffer));
    } else {
        LOGERR << "[WARNING] Failed to get hostname."
               << "Setting default robot id 'e-puck'."
               << std::endl;
        c_controller.SetId("e-puck");
    }
}

/****************************************/
/****************************************/
}

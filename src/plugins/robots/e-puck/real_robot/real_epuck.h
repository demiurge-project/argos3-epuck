/**
 * @file <argos3/plugins/robots/e-puck/real_robot/real_epuck.h>
 *
 * This class provides the real e-puck robot implementation. In
 * particular there is the implementation of the sensors and actuators
 * creation and the serial data communication.
 * @author Gianpiero Francesca <gianpiero.francesca@ulb.ac.be>
 * @author Lorenzo Garattoni <lgaratto@ulb.ac.be>
 */

#ifndef REAL_EPUCK_H
#define REAL_EPUCK_H

namespace argos {
   class CRealEPuck;
}

#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_base.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_proximity_sensor.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_battery_sensor.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_light_sensor.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_ground_sensor.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_accelerometer_sensor.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_range_and_bearing_sensor.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_ircom_sensor.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_micro_sensor.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_wheels_actuator.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_base_leds_actuator.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_rgb_leds_actuator.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_range_and_bearing_actuator.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_ircom_actuator.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_omnidirectional_camera_sensor.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_virtual_camrab_sensor.h>   //@antoine
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_virtual_camrab_actuator.h>   //@antoine
#include <argos3/core/control_interface/ci_controller.h>
#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/configuration/argos_exception.h>
#include <argos3/core/utility/math/rng.h>

#include <cstddef>
#include <iostream>
#include <termios.h>
#include <string>
#include <signal.h>
#include <unistd.h>

namespace argos {

   /****************************************/
   /****************************************/

   enum EBoardState {
      WAITING = 0,
      INITIALIZATION,
      WAKE_UP,
      SENDING_CONFIGURATION,
      RECEIVE,
      SEND
   };

   enum EPICSymbols {
      WAKE_UP_PIC_SYMBOL = 1,
      RESET_PIC_SYMBOL = 2
   };

   /****************************************/
   /****************************************/

   class CRealEPuck {

   protected:

      CRealEPuck();

      CRealEPuck(const CRealEPuck& c_epuck) {}

      CRealEPuck& operator=(const CRealEPuck& c_epuck) { return *this; }

   public:
      bool m_bGoHome;
       /**
       * Returns the instance to the CRealEPuck class Since CRealEPuck
       * is a singleton, the instance is created at the first call of
       * this function and returned thereafter.
       *
       * @return The instance to the CRealEPuck class.
       */
      static CRealEPuck& GetInstance();

      /**
       * Class destructor, ensure free/clean/reset of all stuff in e-puck
       */
      virtual ~CRealEPuck();

      /**
       * Returns the system time on the robot in ms.
       */
      static UInt64 GetTime();

      /**
       * Initializes the e-puck state (sensors/actuator and params)
       *
       * @param str_config_file_name path to the xml config file
       *
       * @param str_controller_id the controller id to select in the
       * xml file if there are severals.
       */
      virtual void Init(const std::string& str_config_file_name,
                const std::string& str_controller_id);

     /**
      * Send data to the pic
      *
      * @param data the data to send
      */
      void SendToPic(size_t data);

     /**
      * Make a check with the pic. Consist of receiving a boolean from the pic.
      *
      * @return true if the check is ok, false if not.
      */
      bool CheckFromPic();

     /**
      * Send the sensor configuration to the real robot sensors
      */
      void SendSensorConfiguration();

     /**
      * Sends actuator data to the pic30 via serial communication.
      */
      void SendActuatorData();

     /**
      * Receive the data
      */
      void ReceiveSensorData();

      /**
       * Called at the end of every ReceiveData. Allows the sensors to
       * copy their specific data in a private structure. Only for
       * i2c sensors this function allows the directed data reading.
       */
      virtual void UpdateValues();

      /**
       * Insert a new actuator
       *
       * @param str_actuator_type the type of the actuator
       * ("epuck_wheels" for example)
       */
      virtual CCI_Actuator* InsertActuator(const std::string& str_actuator_type);

      /**
       * Insert a new Sensor
       *
       * @param str_sensor_type the type of the sensor ("epuck_light"
       * for example)
       */
      virtual CCI_Sensor* InsertSensor(const std::string& str_sensor_type);

      /**
       * Get the controller of this e-puck
       *
       * @return the controller
       */
      inline CCI_Controller& GetController() {
         return *m_pcController;
      }

      /**
       * Check if the experiment is finished, either because of the
       * time limit or a press on the reset button
       *
       * @return true if the experiment is finished, false otherwise
       */
      bool IsExperimentFinished();

      /**
       * Ensure that a step last long enough relative to the time of a
       * step specified in the xml file. Do nothing (appart warning in
       * the log) if the control step was too long.
       */
      void SyncControlStep();

      /**
       * Returns the root of the XML configuration tree.
       */
      inline TConfigurationNode& GetConfigurationRoot() {
          return *m_tXMLConfiguration.FirstChildElement();
      }

      bool bGoHome() const;
      void setBGoHome(bool bGoHome);

   protected:

      /**
       * Create an actuator
       *
       * @param strActuatorName actuator to create (example:
       * "epuck_wheels")
       *
       * @return an instance of the actuator relative to the
       * [strActuatorName] param
       */
      template<class T> T* CreateActuator(const std::string& strActuatorName) {
         T* pc_specificAct;
         try {
            pc_specificAct = GetController().GetActuator<T>(strActuatorName);
            return pc_specificAct;
         } catch(CARGoSException& ex) {
            pc_specificAct = new T;
            GetController().AddActuator(strActuatorName, pc_specificAct);
            return pc_specificAct;
         }
      }

      /**
       * Create a sensor
       *
       * @param strSensorName sensor to create (example:
       * "epuck_proximity")
       *
       * @return an instance of the sensor relative to the
       * [strSensorName] param
       */
      template<class T> T* CreateSensor(const std::string& strSensorName) {
         T* pc_specificSens;
         try {
            pc_specificSens = dynamic_cast<T*>(GetController().GetSensor<T>(strSensorName));
            return pc_specificSens;
         } catch(CARGoSException& ex) {
            pc_specificSens = new T;
            GetController().AddSensor(strSensorName, pc_specificSens);
            return pc_specificSens;
         }
      }

   private:

      /**
       * Initialize the serial port
       */
      void InitSerial();

      /**
       * Send a special character to wake up the pic
       */
      void WakeUpPic();

      /**
       * Destroy the serial port
       */
      void DestroySerial();

      /**private
       * Parse the xml config file and init the controller according
       * to it
       */
      void InitController(const std::string& str_config_file_name,
                          const std::string& str_controller_id);

      /**
       * Creates the requested controller
       *
       * @param str_controller_id the id of the controller to load
       *
       * @param tControllerTree xml subtree of the controller
       * (<CONTROLLER_LABEL id=CONTROLLER_ID> ... </CONTROLLER_LABEL>)
       */
      TConfigurationNode CreateController(const std::string& str_controller_id,
                                          TConfigurationNode& tControllerTree);

      /**
       * Initialize the controller with the <param> node from the xml
       * config file
       *
       * @param tControllerTree xml subtree of the controller
       * (<CONTROLLER_LABEL id=CONTROLLER_ID> ... </CONTROLLER_LABEL>)
       */
      void InitController(TConfigurationNode& tControllerTree);

      /**
       * Creates, initialize and attach all actuators to the controller
       *
       * @param tControllerTree xml subtree of the controller
       * (<CONTROLLER_LABEL id=CONTROLLER_ID> ... </CONTROLLER_LABEL>)
       */
      void LoadActuators(TConfigurationNode& tControllerTree);

      /**
       * Creates, initialize and attach alprivatel sensors to the controller
       *
       * @param tControllerTree xml subtree of the controller
       * (<CONTROLLER_LABEL id=CONTROLLER_ID> ... </CONTROLLER_LABEL>)
       */
      void LoadSensors(TConfigurationNode& tControllerTree);

      /**
       * Rehandle system signals to ensure that the robot will stop
       * correctly.
       */
      void RegisterSystemSignalHandlers();

      /**
       * Signal handler for terminating the controller gracefully.
       */
      static void SignalHandler(SInt32 n_signal);

      /**
       * Reset the pic 30
       */
      void ResetPic30();

      /**
       * Send data to the pic
       *
       * @param data the data to send to the pic
       */
      template<typename T> void SendDataToPic(const T& t_data) {
         /* the size of the data send to the pic */
         ssize_t nWritten;
         /* the size of the data to write */
         size_t unLeftToWrite = sizeof(T);
         /* current position in the data */
         UInt8* pnCurrentPos = (UInt8*) &t_data;
         do {
            nWritten = ::write(m_nPortFileDescriptor,
                               pnCurrentPos,
                               unLeftToWrite);
            if (nWritten < 0) {
               THROW_ARGOSEXCEPTION("Error writing data to the pic");
            } else {
               unLeftToWrite -= nWritten;
               pnCurrentPos += nWritten;
            }
         } while (unLeftToWrite != 0);
        }

      /**
       * Receive data from the pic
       *
       * @param t_data the variable where to store the data received
       */
      template<typename T> void ReceiveDataFromPic(T& t_data) {
        /* the size of the data read on the pic */
         ssize_t nRead;
         /* the size of the data left to read */
         size_t unLeftToRead = sizeof(T);
         /* current position in the data */
         UInt8* pnCurrentPos = (UInt8*) &t_data;
         do {
            /* try to read the content of data to the pic */
            nRead = ::read(m_nPortFileDescriptor,
                           pnCurrentPos,
                           unLeftToRead);
            /* if there was an error during reading */
            if (nRead < 0) {
               THROW_ARGOSEXCEPTION("Error reading data from the pic");
            } else {
               unLeftToRead -= nRead;
               pnCurrentPos += nRead;
            }
         } while (unLeftToRead != 0);
      }

      /**
       * Sets the robot's id.
       */
      static void SetId(CCI_Controller& c_controller);

   private:

      /**
       * The parsed XML tree of the configuration file.
       */
      ticpp::Document m_tXMLConfiguration;

      /**
       * The controller id.
       */
      std::string m_strControllerId;

      /**
       * The e-puck controller.
       */
      CCI_Controller* m_pcController;

      /**
       * The number of steps left in this experiment.
       */
      UInt64 m_unExperimentStepsLeft;

      /**
       * The length of the experiment (in seconds).
       */
      UInt64 m_unExpDuration;

      /**
       * Flag to check if there is a time limit set in the xml file.
       * Used to avoid not necessary calculation with the time.
       */
      bool m_bTimeLimitSet;

      /**
       * Length of one control step (in ms).
       */
      UInt64 m_unControlStepDuration;

      /**
       * Start time of the experiment
       */
      UInt64 m_unStartTimeExperiment;

      /**
       * Time at the start of the control step (unix time stamp in ms).
       */
      UInt64 m_unTimeBeforeControlStep;

      /**
       * Time at the end of the control step (unix time stamp in ms).
       */
      UInt64 m_unTimeAfterControlStep;

      /**
       * A structure
       */
      struct termios m_tAttr;

      /**
       * Descriptor of serial port. Initialized to -1.
       */
      SInt32 m_nPortFileDescriptor;

      /**
       * List of sensors that communicate to the PIC30 through serial communication.
       */
      std::vector<CRealEPuckSerialSensor*> m_vecSerialSensors;

      /**
       * List of sensors that communicate to the rest of the robot through I2C communication.
       */
      std::vector<CRealEPuckI2CSensor*> m_vecI2CSensors;

      /**
       * Omnidirectional camera sensor
       */
      CRealEPuckOmnidirectionalCameraSensor* m_pcOmnidirectionalCameraSensor;

      /**
       * List of actuators that communicate to the rest of the robot through I2C communication.
       */
      std::vector<CRealEPuckI2CActuator*> m_vecI2CActuators;

      /**
       * Structs that contain sensors and actuators values. It'll be
       * exchanged via serial port
       */
      BaseSensorConfiguration m_sSensorConfiguration;

      /**
       * Structs that contains the actuator state.
       */
      BaseActuatorState m_sActuatorState;

      /**
       * Struct that contains the sensor state.
       */
      BaseSensorState m_sSensorState;

      /**
       * The random number generator
       */
      CRandom::CRNG* m_pcRNG;

      /**
       * The range of the random numbers to be generated
       */
      CRange<UInt32> m_cRandomRange;

      /**
       * Serial related
       */
      struct termios tAttr;

      /**
       * Current state of the board-side code.
       */
      EBoardState m_eBoardState;
   };

   /****************************************/
   /****************************************/

}
#endif

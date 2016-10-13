/* 
 Class Client. 

 Function: It can send the state of the sensors to a server, and receive commands as well from the server to set the state of the actuators.
 
 Details: 
 First (after creating an instance of this class), we need to call the init(ip, port, epuckID) method to initialize the connection with the server.
 Then, we can send and receive data from the server, by calling the send&recv() method. 
 This last one should be placed at the end of the loop step() of the robot, in order to the commands received from the server to be effective.
 
 @note: This class works well with the epuck-console (that has been implemented in Java by myself).

 @author: Brian Delhaisse - <briandelhaisse@gmail.com>
 */

#ifndef CLIENT_H
#define CLIENT_H

#include <stdlib.h>  
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <time.h>

#include <math.h>

#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/utility/datatypes/color.h>

#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_wheels_actuator.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_base_leds_actuator.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_rgb_leds_actuator.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_range_and_bearing_actuator.h>

#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_proximity_sensor.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_light_sensor.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_ground_sensor.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_range_and_bearing_sensor.h>


namespace argos {

   class Client {
      
   public:
      /** Default Constructor */
      Client();
      /** Constructor with all the actuators */
      Client(CCI_EPuckWheelsActuator* wheelsActuator, CCI_EPuckBaseLEDsActuator* lEDsActuator, CCI_EPuckRGBLEDsActuator* rGBLEDsActuator,
	 CCI_EPuckRangeAndBearingActuator* rABActuator);
      /** Constructor with all the sensors */
      Client(CCI_EPuckProximitySensor* proximitySensor, CCI_EPuckLightSensor* lightSensor, CCI_EPuckGroundSensor* groundSensor,
	 CCI_EPuckRangeAndBearingSensor* rABSensor);
      /** Constructor with all the actuators and sensors */
      Client(CCI_EPuckWheelsActuator* wheelsActuator, CCI_EPuckBaseLEDsActuator* lEDsActuator, CCI_EPuckRGBLEDsActuator* rGBLEDsActuator,
	 CCI_EPuckRangeAndBearingActuator* rABActuator, CCI_EPuckProximitySensor* proximitySensor, 
	 CCI_EPuckLightSensor* lightSensor,CCI_EPuckGroundSensor* groundSensor, CCI_EPuckRangeAndBearingSensor* rABSensor);

      /** Destructor */
      ~Client();

      // ----------------- Initialization ----------------- //

      /** Initialization of the connection with the server.
       @param: ip - IP of the server.
       @param: port - port of the server.
       @param: epuckId - id of the epuck.
      */
      void init(std::string ip, int port, int epuckId);

      // ----------------- Send and Receive ----------------- //

      /** Send the state of the sensors to the server and receive the state of the actuators from the server */
      void sendAndRcv();

      /** Send a message to the server */
      void createMsg(std::string msg);

      // ----------------- Setters for the Actuators ----------------- //

      /** Set all the actuators */
      void setAllActuators(CCI_EPuckWheelsActuator* wheelsActuator, CCI_EPuckBaseLEDsActuator* lEDsActuator, CCI_EPuckRGBLEDsActuator* rGBLEDsActuator,
	 CCI_EPuckRangeAndBearingActuator* rABActuator);

      /** Set the wheel actuators */
      void setWheelActuators(CCI_EPuckWheelsActuator* wheelsActuator);

      /** Set the Basic led actuators */
      void setLedActuators(CCI_EPuckBaseLEDsActuator* lEDsActuator);

      /** Set the RGB leds actuator */
      void setRGBLedActuators(CCI_EPuckRGBLEDsActuator* rGBLEDsActuator);

      /** Set the RAB actuators */
      void setRABActuators(CCI_EPuckRangeAndBearingActuator* rABActuator);

      // ----------------- Setters for the Sensors ----------------- //

      /** Set all the sensors */ 
      void setAllSensors(CCI_EPuckProximitySensor* proximitySensor, CCI_EPuckLightSensor* lightSensor, CCI_EPuckGroundSensor* groundSensor,
	 CCI_EPuckRangeAndBearingSensor* rABSensor);

      /** Set the proximity sensors */
      void setProximitySensors(CCI_EPuckProximitySensor* proximitySensor);

      /** Set the light sensors */
      void setLightSensors(CCI_EPuckLightSensor* lightSensor);

      /** Set the ground sensors */
      void setGroundSensors(CCI_EPuckGroundSensor* groundSensor);

      /** Set the RAB sensors */
      void setRABSensors(CCI_EPuckRangeAndBearingSensor* rABSensor);

   private:
      /** Variables useful for the internet connection */
      int clientSocket, numbytes;
      char buffer[256];
      bool connected;
      std::string msgWritten;

      /* Actuators */
      CCI_EPuckWheelsActuator* m_pcWheelsActuator;
      CCI_EPuckBaseLEDsActuator* m_pcLEDsActuator;
      CCI_EPuckRGBLEDsActuator* m_pcRGBLEDsActuator;
      CCI_EPuckRangeAndBearingActuator* m_pcRABActuator;

      /* Sensors */
      CCI_EPuckProximitySensor* m_pcProximitySensor;
      CCI_EPuckLightSensor* m_pcLightSensor;
      CCI_EPuckGroundSensor* m_pcGroundSensor;
      CCI_EPuckRangeAndBearingSensor* m_pcRABSensor;

      /** Send the state of the sensors to the server. This method calls the method msgToSend(). */
      void send();

      /** Receive the state of the actuators from the server. This method calls the method parse(). */
      void rcv();

      /** Create the message to send to the server. The message contains the values read by the sensors. */
      std::string msgToSend();

      /** Round a real value to 2digits after the comma/point and convert it to a string.
	@param: val - real value to round.
	@return: value - string corresponding to the real number.
       */
      std::string round(Real val);

      /** Parse the message received by the server, and set the actuators.
	@param: msg - string to parse.
       */
      void parse(std::string msg);
   
      /** Convert integer to string (this method exists in C++11). 
        @param: a - integer to convert.
	@return: value - string of the converted integer.
       */
      std::string to_string(int a);
      
      /** Convert argos::Real value to string (this method exists in C++11). 
        @param: a - real value to convert.
	@return: value - string of the converted real number.
       */
      std::string to_string(Real a);
   };

};

#endif

#include "client.h"

#include <argos3/core/utility/logging/argos_log.h>

using namespace argos;

Client::Client() :
   connected(false),
   msgWritten(""),
   m_pcWheelsActuator(NULL),
   m_pcLEDsActuator(NULL),
   m_pcRGBLEDsActuator(NULL),
   m_pcRABActuator(NULL),
   m_pcProximitySensor(NULL),
   m_pcLightSensor(NULL),
   m_pcGroundSensor(NULL),
   m_pcRABSensor(NULL) {
}

Client::Client(CCI_EPuckWheelsActuator* wheelsActuator, CCI_EPuckBaseLEDsActuator* lEDsActuator, CCI_EPuckRGBLEDsActuator* rGBLEDsActuator,
	 CCI_EPuckRangeAndBearingActuator* rABActuator) :
   connected(false),
   msgWritten(""),
   m_pcWheelsActuator(wheelsActuator),
   m_pcLEDsActuator(lEDsActuator),
   m_pcRGBLEDsActuator(rGBLEDsActuator),
   m_pcRABActuator(rABActuator) {
}

Client::Client(CCI_EPuckProximitySensor* proximitySensor, CCI_EPuckLightSensor* lightSensor, CCI_EPuckGroundSensor* groundSensor,
	 CCI_EPuckRangeAndBearingSensor* rABSensor) :
   connected(false),
   msgWritten(""),
   m_pcProximitySensor(proximitySensor),
   m_pcLightSensor(lightSensor),
   m_pcGroundSensor(groundSensor),
   m_pcRABSensor(rABSensor) {
}

Client::Client(CCI_EPuckWheelsActuator* wheelsActuator, CCI_EPuckBaseLEDsActuator* lEDsActuator, CCI_EPuckRGBLEDsActuator* rGBLEDsActuator,
	 CCI_EPuckRangeAndBearingActuator* rABActuator, CCI_EPuckProximitySensor* proximitySensor, 
	 CCI_EPuckLightSensor* lightSensor,CCI_EPuckGroundSensor* groundSensor, CCI_EPuckRangeAndBearingSensor* rABSensor) :
   connected(false),
   msgWritten(""),
   m_pcWheelsActuator(wheelsActuator),
   m_pcLEDsActuator(lEDsActuator),
   m_pcRGBLEDsActuator(rGBLEDsActuator),
   m_pcRABActuator(rABActuator),
   m_pcProximitySensor(proximitySensor),
   m_pcLightSensor(lightSensor),
   m_pcGroundSensor(groundSensor),
   m_pcRABSensor(rABSensor) {
}

Client::~Client() {
   LOG << "End of the connection with the server." << std::endl;
}

// ----------------- Initialization ----------------- //

/** Initialization of the socket, and connection with the server. */
void Client::init(std::string ip, int port, int epuckId) {
   //Creation of the socket (TCP/IP)
   clientSocket = socket(PF_INET, SOCK_STREAM, 0);
   if(clientSocket == -1)
      LOG << "Client: error during the creation of the socket." << std::endl;

   //Configuration of the client's socket
   struct sockaddr_in csin;
   csin.sin_family = AF_INET;		  //AF_INET = Address Family for internet.
   csin.sin_addr.s_addr = inet_addr(ip.c_str());  //set the IP of the server.
   csin.sin_port = htons(port);		  //set the port of the server.
   memset(&(csin.sin_zero), '\0', 8);

   //Connection with the server
   int connection = connect(clientSocket, (struct sockaddr *)&csin, sizeof(struct sockaddr));
   if(connection==-1)
      LOG << "Client: error during the connection with the server." << std::endl;
   else {
      LOG << "Client: connected to the server!" << std::endl;
      connected = true;
   }

   if(connected) {
      char rep[12];
      std::string id = to_string(epuckId);
      id += "\n"; //This is mandatory or the server (Java) will block and everything will be freezed.
      strncpy(rep, id.c_str(), sizeof(rep));
      rep[sizeof(rep) - 1] = '\0';
      if(::send(clientSocket, &rep, sizeof(rep), 0) == -1)
	 LOG << "Client : Error during the sending of the epuck id." << std::endl;
   }
}

// ----------------- Send and Receive ----------------- //

void Client::sendAndRcv() {
   send();
   rcv();
}

void Client::createMsg(std::string msg) {
   msgWritten = "Msg=["+msg+"]";
}


// ----------------- Setters for the Actuators/Sensors ----------------- //

void Client::setAllActuators(CCI_EPuckWheelsActuator* wheelsActuator, CCI_EPuckBaseLEDsActuator* lEDsActuator, CCI_EPuckRGBLEDsActuator* rGBLEDsActuator,
	 CCI_EPuckRangeAndBearingActuator* rABActuator) {
   m_pcWheelsActuator = wheelsActuator;
   m_pcLEDsActuator = lEDsActuator;
   m_pcRGBLEDsActuator = rGBLEDsActuator;
   m_pcRABActuator = rABActuator;
}

void Client::setWheelActuators(CCI_EPuckWheelsActuator* wheelsActuator) {
   m_pcWheelsActuator = wheelsActuator;
}

void Client::setLedActuators(CCI_EPuckBaseLEDsActuator* lEDsActuator) {
   m_pcLEDsActuator = lEDsActuator;
}

void Client::setRGBLedActuators(CCI_EPuckRGBLEDsActuator* rGBLEDsActuator) {
   m_pcRGBLEDsActuator = rGBLEDsActuator;
}

void Client::setRABActuators(CCI_EPuckRangeAndBearingActuator* rABActuator) {
   m_pcRABActuator = rABActuator;
}

void Client::setAllSensors(CCI_EPuckProximitySensor* proximitySensor, CCI_EPuckLightSensor* lightSensor, CCI_EPuckGroundSensor* groundSensor,
	 CCI_EPuckRangeAndBearingSensor* rABSensor) {
   m_pcProximitySensor = proximitySensor;
   m_pcLightSensor = lightSensor;
   m_pcGroundSensor = groundSensor;
   m_pcRABSensor = rABSensor;
}

void Client::setProximitySensors(CCI_EPuckProximitySensor* proximitySensor) {
   m_pcProximitySensor = proximitySensor;
}

void Client::setLightSensors(CCI_EPuckLightSensor* lightSensor) {
   m_pcLightSensor = lightSensor;
}

void Client::setGroundSensors(CCI_EPuckGroundSensor* groundSensor) {
   m_pcGroundSensor = groundSensor;
}

void Client::setRABSensors(CCI_EPuckRangeAndBearingSensor* rABSensor) {
   m_pcRABSensor = rABSensor;
}

// ----------------- Methods ----------------- //

/** Send the data to the server. [DONE] */
void Client::send() {
   if(connected) {
      std::string msg = msgToSend();
      msg += "\n"; //This is mandatory or the server (Java) will block and everything will be freezed.

      /*char rep[1024];      
      strncpy(rep, msg.c_str(), sizeof(rep));
      rep[sizeof(rep) - 1] = '\0';
      if(::send(clientSocket, &rep, sizeof(rep), 0) == -1)
	 LOG << "Client : Error during the sending." << std::endl;
      */

      char *rep = new char[msg.length() + 1];
      strcpy(rep, msg.c_str());
      if(::send(clientSocket, rep, msg.length()+1, 0) == -1)
	 LOG << "Client : Error during the sending." << std::endl;
      delete [] rep;
   }
}

/** Receive the data from the server. [DONE] */
void Client::rcv() {
   if(connected) {
      numbytes = recv(clientSocket, buffer, 255, 0);
      if(numbytes == -1) {
          LOG << "Client: error during the reception of the message." << std::endl;
      } else {
         buffer[numbytes] = '\0';
      }
      std::string str(buffer);
      //printf("msg: %s \n", str.c_str());
      parse(str);
   }
}

/** Message to send to the server. Create a message containing the values read by the sensors. [DONE] */
std::string Client::msgToSend() {
   std::string msg="";

   // Proximity Sensors. [DONE]
   if(m_pcProximitySensor != NULL) {
      msg += "P=[";
      for(size_t i = 0; i < 8; ++i)
	 msg += round(m_pcProximitySensor->GetReading(i).Value) + ";";
      msg[msg.length()-1] = ']';
   }
   
   // Light Sensors. [DONE]
   if(m_pcLightSensor != NULL) {
      msg += "L=[";
      for(size_t i = 0; i < 8; ++i)
	    msg += round(m_pcLightSensor->GetReading(i).Value) + ";";
      msg[msg.length()-1] = ']';
   }

   // Ground Sensors. [DONE]
   if(m_pcGroundSensor != NULL) {
      msg += "G=[" + round(m_pcGroundSensor->GetReadings().Left) + ";" + round(m_pcGroundSensor->GetReadings().Center)
	 + ";" + round(m_pcGroundSensor->GetReadings().Right) + "]";
   }

   // Range and Bearing Sensors.
   if(m_pcRABSensor != NULL) {
      const CCI_EPuckRangeAndBearingSensor::TPackets& p = m_pcRABSensor->GetPackets();
      size_t size = p.size();
      if(size != 0) {
	 msg += "RAB=[";
	 for (size_t i = 0; i < size; ++i) {
	    msg += to_string(p[i]->Data[0]) + "/" + to_string(p[i]->Data[1]) + "/" + to_string(p[i]->Data[2]) + "/" + to_string(p[i]->Data[3]) 
	       + "/" + round(ToDegrees(p[i]->Bearing).GetValue()) + "/" + round(p[i]->Range) + ";";
	 }
	 msg[msg.length()-1] = ']';
      }
      m_pcRABSensor->ClearPackets();
   }

   msg += msgWritten;
   msgWritten = "";
   
   //printf("msg: %s \n", msg.c_str());
   return msg;
}

/** Given a real value, it will keep only 2digits after the comma/point. [DONE] */
std::string Client::round(Real val) {
   val = floorf(val * 100 + 0.5) / 100;
   std::string value = to_string(val);
   //size_t pos=value.find(",");
   size_t pos = value.find(".");
   value = value.substr(0, pos+2+1);
   return value;
}

/** Parse the msg sent by the server. */
void Client::parse(std::string msg) {
   //printf("MSG: %s", msg.c_str());
   if(msg!="NULL\n") {
      size_t pos=0, pos1=0, posfinal=0;
      
      // Wheels Actuator. [DONE]
      if(m_pcWheelsActuator != NULL) {
	 pos = msg.find("W=[");
	 if(pos != std::string::npos) {
	    pos = pos+3;
	    pos1 = msg.find(";",pos);
	    Real vl = atof((msg.substr(pos,pos1-pos)).c_str());
	    pos = msg.find("]",pos1);
	    Real vr = atof((msg.substr(pos1+1,pos-pos1)).c_str());
	    m_pcWheelsActuator->SetLinearVelocity(vl,vr);
	 }
      }

      // Leds Actuator. [DONE]
      if(m_pcLEDsActuator != NULL) {
	 pos=0;
	 int led=0;
	 do {
	    pos = msg.find("L",pos);
	    if(pos != std::string::npos) {
	       pos = pos+1;
	       unsigned int id = atoi((msg.substr(pos,1)).c_str());
	       pos = pos+3;
	       pos1 = msg.find("]",pos);
	       led = atoi((msg.substr(pos,pos1-pos)).c_str());
	       m_pcLEDsActuator->SwitchLED(id, led==1);
	       pos = pos1+1;
   	    } else {
	       pos = msg.length();
	    }
	 } while(pos != msg.length());
      }

      // RGB Leds Actuator. [DONE]
      if(m_pcRGBLEDsActuator != NULL) {
	 pos = 0;
	 do {
	    pos = msg.find("RGB",pos);
	    if(pos != std::string::npos) {
	       pos = pos+3;
	       unsigned int id = atoi((msg.substr(pos,1)).c_str());
	       pos = pos+3;
	       UInt8 color[3];
	       for(int j=0; j<3; j++) {
		  if(j<2)
		     pos1 = msg.find(";",pos);
		  else
		     pos1 = msg.find("]",pos);
		  color[j] = atoi((msg.substr(pos,pos1-pos)).c_str());
		  pos = pos1+1;
	       }
	       CColor col(color[0], color[1], color[2]);
	       m_pcRGBLEDsActuator->SetColor(id, col);
	    } else {
	       pos = msg.length();
	    }
	 } while(pos != msg.length());
      }

      // Range and Bearing Actuator. [DONE]
      if(m_pcRABActuator != NULL) {
	 pos=msg.find("RAB=[");
	 if(pos != std::string::npos) {
	    pos = pos+5;
	    posfinal = msg.find("]",pos);
	    UInt8 data[4];
	    for(UInt8 i=0; i<sizeof(data); i++) {
	       pos1 = msg.find(";",pos);
	       if(pos1 > posfinal)
		  pos1 = posfinal;
	       data[i] = atoi((msg.substr(pos,pos1-pos)).c_str());
	       if(pos1 == posfinal)
		  break;
	       pos = pos1+1;
	    }
	    m_pcRABActuator->SetData(data);
	 }
      }
   }
}

std::string Client::to_string(int a) {  
   std::ostringstream strs;
   strs << a;
   std::string str = strs.str();
   return str;
}

std::string Client::to_string(Real a) {
   std::ostringstream strs;
   strs << a;
   std::string str = strs.str();
   return str;
}


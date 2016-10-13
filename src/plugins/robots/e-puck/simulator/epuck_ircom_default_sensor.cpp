/**
 * @file <argos3/plugins/robots/e-puck/simulator/epuck_ircom_default_sensor.cpp>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */
#include "epuck_ircom_default_sensor.h"
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/plugins/robots/e-puck/simulator/epuck_rab_equipped_entity.h>
#include <argos3/plugins/simulator/media/rab_medium.h>

namespace argos {

   /****************************************/
   /****************************************/

   CRange<CRadians> INCLINATION_RANGE_IRCOM(CRadians(0), CRadians(ARGOS_PI));

   /****************************************/
   /****************************************/

   CEPuckIRComDefaultSensor::CEPuckIRComDefaultSensor() :
      m_pcIRComEquippedEntity(NULL),
      m_fDistanceNoiseStdDev(0.0f),
      m_pcRNG(NULL),
      m_cSpace(CSimulator::GetInstance().GetSpace()),
      m_bShowRays(false) {
   }

   /****************************************/
   /****************************************/

   void CEPuckIRComDefaultSensor::SetRobot(CComposableEntity& c_entity) {
      /* Assign RAB equipped entity to this sensor */
      m_pcIRComEquippedEntity = &c_entity.GetComponent<CEpuckRABEquippedEntity>("rab[rab_1]");
      /* Enable the RAB equipped entity */
      m_pcIRComEquippedEntity->Enable();
      /* Get reference to controllable entity */
      m_pcControllableEntity = &c_entity.GetComponent<CControllableEntity>("controller");
   }

   /****************************************/
   /****************************************/

   void CEPuckIRComDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         /* Parent class init */
         CCI_EPuckIRComSensor::Init(t_tree);
         /* Show rays? */
         GetNodeAttributeOrDefault(t_tree, "show_rays", m_bShowRays, m_bShowRays);
         /* Parse noise */
         GetNodeAttributeOrDefault(t_tree, "noise_std_dev", m_fDistanceNoiseStdDev, m_fDistanceNoiseStdDev);
         if(m_fDistanceNoiseStdDev > 0.0f) {
            m_pcRNG = CRandom::CreateRNG("argos");
         }
         /* Get RAB medium from id specified in the XML */
         std::string strMedium;
         GetNodeAttribute(t_tree, "medium", strMedium);
         m_pcRangeAndBearingMedium = &(CSimulator::GetInstance().GetMedium<CRABMedium>(strMedium));
         /* Assign RAB entity to the medium */
         m_pcRangeAndBearingMedium->AddEntity(*m_pcIRComEquippedEntity);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing the range and bearing medium sensor", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CEPuckIRComDefaultSensor::Update() {
      /** TODO: there's a more efficient way to implement this */
      /* Delete old readings */
      ClearPackets();
      /* Get list of communicating RABs */
      const CSet<CRABEquippedEntity*>& setRABs =
         m_pcRangeAndBearingMedium->GetRABsCommunicatingWith(*m_pcIRComEquippedEntity);
      /* Buffer for calculating the message--robot distance */
      CVector3 cVectorRobotToMessage;
      /* Buffer for the received packet */
      CCI_EPuckIRComSensor::SPacket* psPacket;
      /* Buffer for vertical bearing*/
      CRadians cVerticalBearing;
      /* Go through communicating RABs and create packets */
      for(CSet<CRABEquippedEntity*>::iterator it = setRABs.begin(); it != setRABs.end(); ++it) {
         /* allocating memory */
         psPacket = new CCI_EPuckIRComSensor::SPacket();
         /* Create a reference to the RAB entity to process */
         CEpuckRABEquippedEntity* cIRComSendingEntity = static_cast<CEpuckRABEquippedEntity*>(*it);
         /* Add ray if requested */
         if(m_bShowRays) {
            m_pcControllableEntity->AddCheckedRay(false,
                                                  CRay3(cIRComSendingEntity->GetPosition(),
                                                        m_pcIRComEquippedEntity->GetPosition()));
         }
         /* Calculate vector to entity */
         cVectorRobotToMessage = cIRComSendingEntity->GetPosition();
         cVectorRobotToMessage -= m_pcIRComEquippedEntity->GetPosition();
         /* If noise was setup, add it */
         if(m_fDistanceNoiseStdDev > 0.0f) {
            cVectorRobotToMessage += CVector3(
               m_pcRNG->Gaussian(m_fDistanceNoiseStdDev),
               m_pcRNG->Uniform(INCLINATION_RANGE_IRCOM),
               m_pcRNG->Uniform(CRadians::UNSIGNED_RANGE));
         }
         /*
          * Set range and bearing from cVectorRobotToMessage
          * First, we must rotate the cVectorRobotToMessage so that
          * it is local to the robot coordinate system. To do this,
          * it enough to rotate cVectorRobotToMessage by the inverse
          * of the robot orientation.
          */
         cVectorRobotToMessage.Rotate(m_pcIRComEquippedEntity->GetOrientation().Inverse());
         cVectorRobotToMessage.ToSphericalCoords(psPacket->Range,
                                                 cVerticalBearing,
                                                 psPacket->Bearing);
         /* Convert range to cm */
         psPacket->Range *= 100.0f;
         /* Normalize horizontal bearing between [-pi,pi] */
         psPacket->Bearing.SignedNormalize();
         /* Set message data */
         psPacket->Data =cIRComSendingEntity->GetData()[0];
        /* Add message to the list */
         m_tPackets.push_back(psPacket);
      }
   }

   /****************************************/
   /****************************************/

   void CEPuckIRComDefaultSensor::Reset() {
      ClearPackets();
   }

   /****************************************/
   /****************************************/

   void CEPuckIRComDefaultSensor::Destroy() {
      ClearPackets();
      m_pcRangeAndBearingMedium->RemoveEntity(*m_pcIRComEquippedEntity);
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CEPuckIRComDefaultSensor,
                   "epuck_ircom", "medium",
                   "Lorenzo Garattoni [lgaratto@ulb.ac.be]",
                   "1.0",
                   "The ircom sensor of the e-puck.",
                   "This sensor allows epucks to perform situated communication, i.e., a form of\n"
                   "wireless communication whereby the receiver also knows the location of the\n"
                   "sender with respect to its own frame of reference.\n"
                   "This implementation of the ircom sensor is associated to the\n"
                   "ircom medium. To be able to use this sensor, you must add a\n"
                   "ircom medium to the <media> section.\n"
                   "This sensor allows a epucks to receive messages. To send messages, you need the\n"
                   "ircom actuator (\"epuck_ircom\" in the actuator section).\n"
                   "For use, refer to [ci_epuck_ircom_sensor.h].\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "   <controllers>\n"
                   "      ...\n"
                   "      <my_controller>\n"
                   "         ...\n"
                   "         <sensors>\n"
                   "            ...\n"
                   "            <epuck_ircom implementation=\"medium\" medium=\"rab\"/>\n"
                   "            ...\n"
                   "         <sensors/>\n"
                   "         ...\n"
                   "      <my_controller/>\n"
                   "      ...\n"
                   "   <controllers>\n\n"
                   "The attribute [medium] refers to the [id] of the [range_and_bearing] medium defined\n"
                   "in the [media] xml section\n\n"
                   "   <media>\n"
                   "      ...\n"
                   "      <range_and_bearing id=\"rab\"/>\n"
                   "      ...\n"
                   "   </media>\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n"
                   "It is possible to draw the rays shot by the rab sensor in the OpenGL\n"
                   "visualization. This can be useful for sensor debugging but also to understand\n"
                   "what's wrong in your controller. In OpenGL, the rays are drawn in cyan when\n"
                   "they are not obstructed and in purple when they are. In case a ray is\n"
                   "obstructed, a black dot is drawn where the intersection occurred.\n"
                   "To turn this functionality on, add the attribute \"show_rays\".\n\n"
                   "   <controllers>\n"
                   "      ...\n"
                   "      <my_controller>\n"
                   "         ...\n"
                   "         <sensors>\n"
                   "            ...\n"
                   "            <epuck_ircom implementation=\"medium\" medium=\"rab\" show_rays=\"true\"/>\n"
                   "            ...\n"
                   "         <sensors/>\n"
                   "         ...\n"
                   "      <my_controller/>\n"
                   "      ...\n"
                   "   <controllers>\n\n"
                   "It is possible to specify noise to the sensors in order to match the characteristics\n"
                   "of the real e-puck. This can be done with the attribute: 'noise_std_dev',\n"
                   "which indicates the standard deviation of a gaussian noise applied to the\n"
                   "sensor.\n\n"
                   "   <controllers>\n"
                   "      ...\n"
                   "      <my_controller>\n"
                   "         ...\n"
                   "         <sensors>\n"
                   "            ...\n"
                   "            <epuck_ircom implementation=\"medium\" medium=\"rab\" noise_std_dev=\"1\"/>\n"
                   "            ...\n"
                   "         <sensors/>\n"
                   "         ...\n"
                   "      <my_controller/>\n"
                   "      ...\n"
                   "   <controllers>"
                   , "Usable");
}


/**
 * @file <argos3/plugins/robots/e-puck/simulator/epuck_ground_default_sensor.cpp>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/floor_entity.h>
#include <argos3/plugins/simulator/entities/ground_sensor_equipped_entity.h>

#include "epuck_ground_rotzonly_sensor.h"

namespace argos {

   /****************************************/
   /****************************************/

   static CRange<Real> UNIT(0.0f, 1.0f);

   /****************************************/
   /****************************************/

   CEPuckGroundRotZOnlySensor::CEPuckGroundRotZOnlySensor() :
      m_pcEmbodiedEntity(NULL),
      m_pcFloorEntity(NULL),
      m_pcGroundSensorEntity(NULL),
      m_pcRNG(NULL),
      m_bAddNoise(false),
      m_cSpace(CSimulator::GetInstance().GetSpace()) {}

   /****************************************/
   /****************************************/

   void CEPuckGroundRotZOnlySensor::SetRobot(CComposableEntity& c_entity) {
      m_pcEmbodiedEntity = &(c_entity.GetComponent<CEmbodiedEntity>("body"));
      m_pcGroundSensorEntity = &(c_entity.GetComponent<CGroundSensorEquippedEntity>("ground_sensors"));
      m_pcGroundSensorEntity->Enable();
      m_pcFloorEntity = &m_cSpace.GetFloorEntity();
   }

   /****************************************/
   /****************************************/

   void CEPuckGroundRotZOnlySensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_EPuckGroundSensor::Init(t_tree);
         /* Parse noise level */
         Real fNoiseLevel = 0.0f;
         GetNodeAttributeOrDefault(t_tree, "noise_level", fNoiseLevel, fNoiseLevel);
         if(fNoiseLevel < 0.0f) {
            THROW_ARGOSEXCEPTION("Can't specify a negative value for the noise level of the ground sensor");
         }
         else if(fNoiseLevel > 0.0f) {
            m_bAddNoise = true;
            m_cNoiseRange.Set(-fNoiseLevel, fNoiseLevel);
            m_pcRNG = CRandom::CreateRNG("argos");
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in rotzonly ground sensor", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CEPuckGroundRotZOnlySensor::Update() {
      /*
       * We make the assumption that the robot is rotated only wrt to Z
       */
      /* Get robot position and orientation */
      const CVector3& cEntityPos = m_pcEmbodiedEntity->GetOriginAnchor().Position;
      const CQuaternion& cEntityRot = m_pcEmbodiedEntity->GetOriginAnchor().Orientation;
      CRadians cRotZ, cRotY, cRotX;
      cEntityRot.ToEulerAngles(cRotZ, cRotY, cRotX);
      /* Set robot center */
      CVector2 cCenterPos(cEntityPos.GetX(), cEntityPos.GetY());
      /* Position of sensor on the ground after rototranslation */
      CVector2 cSensorPos;
      /* Go through the sensors */
      for(UInt32 i = 0; i < 3; ++i) {
         /* Calculate sensor position on the ground */
         cSensorPos = m_pcGroundSensorEntity->GetSensor(i).Offset;
         cSensorPos.Rotate(cRotZ);
         cSensorPos += cCenterPos;
         /* Get the color */
         const CColor& cColor = m_pcFloorEntity->GetColorAtPoint(cSensorPos.GetX(),
                                                                 cSensorPos.GetY());
         /* Set the reading */
         m_sReadings[2-i] = cColor.ToGrayScale() / 255.0f;
         /* Apply noise to the sensor */
         if(m_bAddNoise) {
            m_sReadings[2-i] += m_pcRNG->Uniform(m_cNoiseRange);
         }
         /* Clamp the reading between 0 and 1 */
         UNIT.TruncValue(m_sReadings[2-i]);
      }
   }

   /****************************************/
   /****************************************/

   void CEPuckGroundRotZOnlySensor::Reset() {
      for(UInt32 i = 0; i < 3; ++i) {
         m_sReadings[i] = 0.0f;
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CEPuckGroundRotZOnlySensor,
                   "epuck_ground", "rot_z_only",
                   "Lorenzo Garattoni [lgaratto@ulb.ac.be]",
                   "1.0",
                   "The e-puck ground sensor",
                   "This sensor accesses a set of ground sensors. The sensors all return a value in [0-1].\n"
                   "The 3 sensors return values between [0-1], where 1 means white and 0 black.\n"
                   "For usage, refer to [ci_epuck_ground_sensor.h]\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "   <controllers>\n"
                   "      ...\n"
                   "      <my_controller>\n"
                   "         ...\n"
                   "         <sensors>\n"
                   "            ...\n"
                   "            <epuck_ground implementation=\"rot_z_only\"/>\n"
                   "            ...\n"
                   "         <sensors/>\n"
                   "         ...\n"
                   "      <my_controller/>\n"
                   "      ...\n"
                   "   <controllers>\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n"
                   "It is possible to add uniform noise to the sensors, thus matching the\n"
                   "characteristics of the real robot better. This can be done with the attribute\n"
                   "\"noise_level\", whose allowed range is in [-1,1] and is added to the calculated\n"
                   "reading. The final sensor reading is always normalized in the [0-1] range.\n\n"
                   "   <controllers>\n"
                   "      ...\n"
                   "      <my_controller>\n"
                   "         ...\n"
                   "         <sensors>\n"
                   "            ...\n"
                   "            <epuck_ground implementation=\"rot_z_only\" noise_level=\"0.1\"/>\n"
                   "            ...\n"
                   "         <sensors/>\n"
                   "         ...\n"
                   "      <my_controller/>\n"
                   "      ...\n"
                   "   <controllers>"
                   ,"Usable");
}

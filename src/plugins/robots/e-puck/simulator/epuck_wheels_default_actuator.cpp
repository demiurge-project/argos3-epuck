/**
 * @file <argos3/plugins/robots/e-puck/simulator/epuck_wheels_default_actuator.cpp>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#include "epuck_wheels_default_actuator.h"
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/simulator/entity/composable_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CEPuckWheelsDefaultActuator::CEPuckWheelsDefaultActuator() :
      m_pcWheeledEntity(NULL),
      m_pcRNG(NULL),
      m_fNoiseStdDeviation(0.0f) {
      m_fCurrentVelocity[LEFT_WHEEL] = 0.0f;
      m_fCurrentVelocity[RIGHT_WHEEL] = 0.0f;
   }

   /****************************************/
   /****************************************/

   void CEPuckWheelsDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         CCI_EPuckWheelsActuator::Init(t_tree);
         GetNodeAttributeOrDefault<Real>(t_tree, "noise_std_dev", m_fNoiseStdDeviation, 0.0f);
         if(m_fNoiseStdDeviation > 0.0f) {
            m_pcRNG = CRandom::CreateRNG("argos");
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in e-puck wheels actuator.", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CEPuckWheelsDefaultActuator::SetRobot(CComposableEntity& c_entity) {
      try {
         m_pcWheeledEntity = &(c_entity.GetComponent<CWheeledEntity>("wheels"));
         if(m_pcWheeledEntity->GetNumWheels() != 2) {
            THROW_ARGOSEXCEPTION("The e-puck wheels actuator can be associated only to a robot with 2 wheels");
         }
         m_pcWheeledEntity->Enable();
      } catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error setting e-puck wheels actuator to entity \""
                                     << c_entity.GetId() << "\"", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CEPuckWheelsDefaultActuator::Update() {
      m_pcWheeledEntity->SetVelocities(m_fCurrentVelocity);
   }

   /****************************************/
   /****************************************/

   void CEPuckWheelsDefaultActuator::Reset() {
      m_fCurrentVelocity[LEFT_WHEEL]  = 0.0f;
      m_fCurrentVelocity[RIGHT_WHEEL] = 0.0f;
   }

   /****************************************/
   /****************************************/

   void CEPuckWheelsDefaultActuator::SetLinearVelocity(Real f_left_velocity,
                                                       Real f_right_velocity) {
      /* Convert speeds in m/s */
      m_fCurrentVelocity[LEFT_WHEEL] = f_left_velocity * 0.01f;
      m_fCurrentVelocity[RIGHT_WHEEL] = f_right_velocity * 0.01f;
      /* Apply noise */
      if(m_fNoiseStdDeviation > 0.0f)
         AddGaussianNoise();
   }

   /****************************************/
   /****************************************/

   void CEPuckWheelsDefaultActuator::AddGaussianNoise() {
      m_fCurrentVelocity[LEFT_WHEEL]  +=
         m_fCurrentVelocity[LEFT_WHEEL] * m_pcRNG->Gaussian(m_fNoiseStdDeviation);
      m_fCurrentVelocity[RIGHT_WHEEL] +=
         m_fCurrentVelocity[RIGHT_WHEEL] * m_pcRNG->Gaussian(m_fNoiseStdDeviation);
   }

   /****************************************/
   /****************************************/

   REGISTER_ACTUATOR(CEPuckWheelsDefaultActuator,
                     "epuck_wheels", "default",
                     "Lorenzo Garattoni [lgaratto@ulb.ac.be]",
                     "1.0",
                     "The epuck wheels actuator.",
                     "This actuator controls the two wheels of the e-puck.\n"
                     "For usage, refer to [ci_epuck_wheels_actuator.h]\n\n"
                     "REQUIRED XML CONFIGURATION\n\n"
                     "   <controllers>\n"
                     "      ...\n"
                     "      <my_controller>\n"
                     "         ...\n"
                     "         <actuators>\n"
                     "            ...\n"
                     "            <epuck_wheels implementation=\"default\"/>\n"
                     "            ...\n"
                     "         <actuators/>\n"
                     "         ...\n"
                     "      <my_controller/>\n"
                     "      ...\n"
                     "   <controllers>\n\n"
                     "OPTIONAL XML CONFIGURATION\n\n"
                     "It is possible to specify noisy speed in order to match the characteristics\n"
                     "of the real e-puck. This can be done with the attribute: 'noise_std_dev',\n"
                     "which indicates the standard deviation of a gaussian noise applied to the\n"
                     "desired velocity of the steering.\n\n"
                     "   <controllers>\n"
                     "      ...\n"
                     "      <my_controller>\n"
                     "         ...\n"
                     "         <actuators>\n"
                     "            ...\n"
                     "            <epuck_wheels implementation=\"default\" noise_std_dev=\"1\"/>\n"
                     "            ...\n"
                     "         <actuators/>\n"
                     "         ...\n"
                     "      <my_controller/>\n"
                     "      ...\n"
                     "   <controllers>"
                     ,"Usable");
}

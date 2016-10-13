/**
 * @file <argos3/plugins/robots/e-puck/simulator/epuck_virtual_camrab_default_actuator.cpp>
 *
 * @author Antoine Ligot - <aligot@ulb.ac.be>
 */

#include "epuck_virtual_camrab_default_actuator.h"
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

    /****************************************/
    /****************************************/

    void CEPuckVirtualCamrabDefaultActuator::SetRobot(CComposableEntity& c_entity) {
        m_pcRangeAndBearingEquippedEntity = &c_entity.GetComponent<CEpuckRABEquippedEntity>("rab[rab_0]");
        m_pcRangeAndBearingEquippedEntity->Enable();
        
        m_pcLEDEquippedEntity = &(c_entity.GetComponent<CLEDEquippedEntity>("leds"));
        m_pcLEDEquippedEntity->Enable();
    }

    /****************************************/
    /****************************************/

    void CEPuckVirtualCamrabDefaultActuator::Init(TConfigurationNode& t_tree) {
        CCI_EPuckVirtualCamrabActuator::Init(t_tree);
    }

    /****************************************/
    /****************************************/

    void CEPuckVirtualCamrabDefaultActuator::Update() {
        CCI_EPuckRangeAndBearingActuator::SDataToSend dataToSend;
        dataToSend.State = (CCI_EPuckRangeAndBearingActuator::EEmitterState)(m_sDesiredData.State); 
        for(size_t i = 0; i < 12; ++i) {
            dataToSend.Emitter[i].Enabled = m_sDesiredData.Emitter[i].Enabled;
            dataToSend.Emitter[i].Data[0] = m_sDesiredData.Emitter[i].Data[0];
            dataToSend.Emitter[i].Data[1] = m_sDesiredData.Emitter[i].Data[1];
            dataToSend.Emitter[i].Data[2] = m_sDesiredData.Emitter[i].Data[2];
            dataToSend.Emitter[i].Data[3] = m_sDesiredData.Emitter[i].Data[3];
        }
        m_pcRangeAndBearingEquippedEntity->SetDataToSend(dataToSend);

        
        CColor currentColor = IntegerToColor(m_unCurrentColorId);
        
        if (m_bLEDFlag) {
            m_pcLEDEquippedEntity->SetLEDColor(8,  currentColor);
            m_pcLEDEquippedEntity->SetLEDColor(9,  currentColor);
            m_pcLEDEquippedEntity->SetLEDColor(10, currentColor);
        }
    }	

    /****************************************/
    /****************************************/

REGISTER_ACTUATOR(CEPuckVirtualCamrabDefaultActuator,
                  "epuck_virtual_camrab", "default",
                  "Antoine Ligot - [aligot@ulb.ac.be]"
                  ,
                  "1.0",
                  "The camrab actuator of the e-puck.",
                  "This actuator allows epucks to perform improved situated communication. \n"
                  "By combining both range-and-bearing and omnidirectional camera sensors, the distance \n"
                  "estimation of the range-and-bearing is enhanced. \n"
                  "This actuator represents an interface that allows the user to send messages with \n"
                  "the range-and-bearing actuator and light the leds with the rgb leds actuator. \n"
                  "Since the actuator sends an integer that represents the color used to light the leds, \n"
                  "some bits of the first byte of the RAB byte are no longer available to the user \n"
                  "-- WARNING: \n"
                  "To send messages, the range-and-bearing, rgb leds and the virtual camrab actuator are needed. \n"
                  "To receive messages, you need the range-and-bearing, omnidirectional camera and \n"
                  "the virtual camrab sensor.\n"
                  "For usage, refer to [ci_epuck_virtual_camrab_actuator.h]\n\n"
                  "REQUIRED XML CONFIGURATION\n\n"
                  "   <controllers>\n"
                  "      ...\n"
                  "      <my_controller>\n"
                  "         ...\n"
                  "         <actuators>\n"
                  "            ...\n"
                  "            <epuck_range_and_bearing implementation=\"medium\" medium=\"rab\" range=\"0.8\" data_size=\"2\"/>\n"
                  "            <epuck_rgb_leds implementation=\"default\" medium=\"leds\"/>\n"
                  "            <epuck_virtual_camrab implementation=\"default\" number_colors=\"4\" data_size=\"2\"/> \n"
                  "            ...\n"
                  "         <actuators/>\n"
                  "         ...\n"
                  "      <my_controller/>\n"
                  "      ...\n"
                  "   <controllers>\n\n"
                  "The attribute [number_colors] is the number of different colors used to light the leds. Needs to \n"
                  "coincide with the [number_colors] declaration of the virtucal camrab sensor. The default value\n"
                  "is 3 and it is in the range[1,9]. \n"
                  "The attribute [data_size] is the number of bytes sent and it is in the range [1,4]. "
                  "The attributes [medium] refer to the [id] of the [range_and_bearing] and the [led] medium defined\n"
                  "in the [media] xml section.\n"
                  "The attribute [range] defines the communication range of the rab in meters.\n"
                  "   <media>\n"
                  "      ...\n"
                  "      <range_and_bearing id=\"rab\"/>\n"
                  "      <led id=\"leds\" grid_size=\"1,1,1\"/>\n"
                  "      ...\n"
                  "   </media>"
                  , "Usable");

}

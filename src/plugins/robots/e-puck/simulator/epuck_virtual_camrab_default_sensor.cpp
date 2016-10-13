/**
 * @file <argos3/plugins/robots/e-puck/simulator/epuck_virtual_camrab_default_sensor.cpp>
 *
 * @author Antoine Ligot - <aligot@ulb.ac.be>
 */

#include "epuck_virtual_camrab_default_sensor.h"

namespace argos{

    /****************************************/
    /****************************************/

    void CEPuckVirtualCamrabDefaultSensor::SetRobot(CComposableEntity& c_entity) {
        m_pcRangeAndBearingEquippedEntity = &c_entity.GetComponent<CEpuckRABEquippedEntity>("rab[rab_0]");
        m_pcRangeAndBearingEquippedEntity->Enable();
        
        /* Get reference to controllable entity, used to get reference to the sensors */
        m_pcControllableEntity = &c_entity.GetComponent<CControllableEntity>("controller");
        
        m_pcRabSensor = m_pcControllableEntity->GetController().GetSensor<CCI_EPuckRangeAndBearingSensor>("epuck_range_and_bearing");
        m_pcCamera = m_pcControllableEntity->GetController().GetSensor<CCI_EPuckOmnidirectionalCameraSensor>("epuck_omnidirectional_camera");
        m_pcCamera->Enable();
    }

    /****************************************/
    /****************************************/

    void CEPuckVirtualCamrabDefaultSensor::Init(TConfigurationNode& t_tree) {
        CCI_EPuckVirtualCamrabSensor::Init(t_tree);
    }

    /****************************************/
    /****************************************/

    void CEPuckVirtualCamrabDefaultSensor::Reset() {
        ClearReadings();
    }

    /****************************************/
    /****************************************/

    void CEPuckVirtualCamrabDefaultSensor::Destroy() {
        ClearReadings();
    }

    /****************************************/
    /****************************************/

    void CEPuckVirtualCamrabDefaultSensor::Update() {
        std::vector<CCI_EPuckRangeAndBearingSensor::SReceivedPacket*> vecRabPackets = m_pcRabSensor->GetPackets();
        
        const CCI_EPuckOmnidirectionalCameraSensor::SReadings* sCameraReadings = &m_pcCamera->GetReadings();
        std::vector<CCI_EPuckOmnidirectionalCameraSensor::SBlob*> vecAllBlobs = sCameraReadings->BlobList;
        
        std::vector<CCI_EPuckOmnidirectionalCameraSensor::SBlob*> vecFilteredBlobs = FilterBlobs(vecAllBlobs);

        InferRobots(vecFilteredBlobs, vecRabPackets);
    }

    /***************************************/
    /***************************************/

    void CEPuckVirtualCamrabDefaultSensor::InferRobots(std::vector<CCI_EPuckOmnidirectionalCameraSensor::SBlob*> vec_blobs, std::vector<CCI_EPuckRangeAndBearingSensor::SReceivedPacket*> vec_packets) {
        if ((!vec_blobs.empty()) && (!vec_packets.empty())) {
            UInt8 unCurrentColor;
            Real fCurrentDiff;
            std::vector<CCI_EPuckOmnidirectionalCameraSensor::SBlob*> vecBlobsWithSameColor;
            for (std::vector<CCI_EPuckRangeAndBearingSensor::SReceivedPacket*>::iterator itRAB = vec_packets.begin(); itRAB != vec_packets.end(); ++itRAB) {
                /* Get color from RAB message */
                unCurrentColor = (*itRAB)->Data[0] & m_unColorMask;
                /* Filter blobs corresponding to the color in RAB message */
                vecBlobsWithSameColor = GetBlobsWithColor(vec_blobs, unCurrentColor);
                Real fMinDiff = 1000;
                CCI_EPuckOmnidirectionalCameraSensor::SBlob* psElected = NULL;
                for (std::vector<CCI_EPuckOmnidirectionalCameraSensor::SBlob*>::iterator itBlob = vecBlobsWithSameColor.begin(); itBlob != vecBlobsWithSameColor.end(); ++itBlob) {
                    fCurrentDiff = std::abs( RadiansToDegrees((*itBlob)->Angle) - RadiansToDegrees((*itRAB)->Bearing) );
                    if (fCurrentDiff < fMinDiff) {
                        psElected = (*itBlob);
                        fMinDiff = fCurrentDiff;
                    }
                    CreateReading(psElected, *itRAB);
                    vec_blobs.erase(std::remove(vec_blobs.begin(), vec_blobs.end(), psElected), vec_blobs.end());
                }
            }
        } else {   
            /* If no blobs are perceived at all, just collect the RAB data. */
            if (!vec_packets.empty()) {
                for (std::vector<CCI_EPuckRangeAndBearingSensor::SReceivedPacket*>::iterator itRAB = vec_packets.begin(); itRAB != vec_packets.end(); ++itRAB) {
                    CreateReading(*itRAB);
                }
            }
        
        }
    }

    /****************************************/
    /****************************************/

    void CEPuckVirtualCamrabDefaultSensor::CreateReading(CCI_EPuckOmnidirectionalCameraSensor::SBlob* ps_blob, CCI_EPuckRangeAndBearingSensor::SReceivedPacket* ps_RAB_packet) {
        SReading* sNewReading = new SReading();
        sNewReading->Color = ps_blob->Color;
        sNewReading->Distance = Fusion(ps_blob->Distance, ps_RAB_packet->Range);
        sNewReading->Angle = RadiansToDegrees(ps_blob->Angle);
        sNewReading->Data[0] = ps_RAB_packet->Data[0] >> m_unNumberBitsToShift;
        sNewReading->Data[1] = ps_RAB_packet->Data[1];
        sNewReading->Data[2] = ps_RAB_packet->Data[2];
        sNewReading->Data[3] = ps_RAB_packet->Data[3];
        sNewReading->Fusion = true;
        m_tReadings.push_back(sNewReading);
    }

    /****************************************/
    /****************************************/

    void CEPuckVirtualCamrabDefaultSensor::CreateReading(CCI_EPuckRangeAndBearingSensor::SReceivedPacket* ps_RAB_packet) {
        SReading* sNewReading = new SReading();
        sNewReading->Color = IntegerToColor(ps_RAB_packet->Data[0] & 3);
        sNewReading->Distance = 0.0;
        sNewReading->Angle = RadiansToDegrees(ps_RAB_packet->Bearing);
        sNewReading->Data[0] = ps_RAB_packet->Data[0] >> m_unNumberBitsToShift;
        sNewReading->Data[1] = ps_RAB_packet->Data[1];
        sNewReading->Data[2] = ps_RAB_packet->Data[2];
        sNewReading->Data[3] = ps_RAB_packet->Data[3];
        sNewReading->Fusion = false;
        m_tReadings.push_back(sNewReading);
    }

    /****************************************/
    /****************************************/

REGISTER_SENSOR(CEPuckVirtualCamrabDefaultSensor,
                "epuck_virtual_camrab", "default",
                "Antoine Ligot - [aligot@ulb.ac.be]",
                "1.0",
                "The virtual camrab sensor of the e-puck.",
                "This sensor allows epucks to perform improved situated communication.\n"
                "By combining both range-and-bearing and omnidirectional camera sensors, the distance \n"
                "estimation of the range-and-bearing is enhanced. \n"
                "This sensor represents an interface that combines range-and-bearing and omnidirectional \n"
                "readings by matching received packets and detected blobs. \n"
                "-- WARNING: \n"
                "To receive messages, the virtual camrab sensor is required, as well as the range-and-bearing \n"
                "sensor and the omnidirectional camera sensor. \n"
                "To send messages, you need the virtual camrab actuator along side with the \n"
                "range-and-bearing and the rgb leds actuators.\n"
                "For use, refer to [ci_epuck_virtual_camrab_sensor.h].\n\n"
                "REQUIRED XML CONFIGURATION\n\n"
                "   <controllers>\n"
                "      ...\n"
                "      <my_controller>\n"
                "         ...\n"
                "         <sensors>\n"
                "            ...\n"
                "            <epuck_range_and_bearing implementation=\"medium\" medium=\"rab\" show_rays=\"false\" data_size=\"2\" noise_std_dev=\"0.0\" real_range_noise=\"false\" max_packets=\"UINT32_MAX\" loss_probability=\"0.0\"/>\n"
                "            <epuck_omnidirectional_camera implementation=\"rot_z_only\" medium=\"leds\" show_rays=\"false\" calibrated=\"true\" low_level_config=\"omnidirectional_camera_sensor.xml\"/> \n"
                "            <epuck_virtual_camrab implementation=\"default\" number_colors=\"4\"/>\n"
                "            ...\n"
                "         <sensors/>\n"
                "         ...\n"
                "      <my_controller/>\n"
                "      ...\n"
                "   <controllers>\n\n"
                "The attribute [medium] refers to the [id] of the [range_and_bearing] medium defined\n"
                "in the [media] xml section.\n"
                "For more information about the range-and-bearing or omnidirectional camera configuration, \n"
                "go check [epuck_range_and_bearing_default_sensor.cpp] or [epuck_omnidirectional_camera_sensor.cpp] \n"
                "\n"
                "The attribute [number_colors] is the number of different colors used to light the leds. Needs to \n"
                "coincide with the [number_colors] declaration of the virtucal camrab sensor. The default value\n"
                "is 3 and it is in the range[1,9]. \n"
                "The attribute [data_size] is the number of bytes sent and it is in the range [1,4]. Should \n"
                "coincide with the data_size value of the epuck_range_and_bearing sensor. \n"
                "\n"
                "   <media>\n"
                "      ...\n"
                "      <range_and_bearing id=\"rab\"/>\n"
                "      ...\n"
                "   </media>\n\n"
                , "Usable");
                
}

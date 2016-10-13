/**
 * @file <argos3/plugins/robots/e-puck/real_robot/real_epuck_virtual_camrab_sensor.cpp>
 *
 * @author Antoine Ligot - <aligot@ulb.ac.be>
 */

#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_virtual_camrab_sensor.h>

namespace argos {

    /* time between two send in micro seconds (so 10 000 micro seconds = 10 milli seconds) */
    static UInt32 TIME_BETWEEN_TWO_RECEIVE = 20000;

    /****************************************/
    /****************************************/
    
    static void VirtualSensorCleanupThreadData(void* pt_data) {

        CRealEPuckI2CDevice::ThreadCleanup();
        pthread_mutex_t* ptMutex = reinterpret_cast<pthread_mutex_t*>(pt_data);
        pthread_mutex_unlock(ptMutex);

    }
    
    /****************************************/
    /****************************************/

    void CRealEPuckVirtualCamrabSensor::CreateReading(CCI_EPuckOmnidirectionalCameraSensor::SBlob* s_blob, CCI_EPuckRangeAndBearingSensor::SReceivedPacket* s_rab_packet) {
        SReading* sNewReading = new SReading();
        sNewReading->Color = s_blob->Color;
        sNewReading->Distance = Fusion(s_blob->Distance, s_rab_packet->Range);
        sNewReading->Angle = RadiansToDegrees(s_blob->Angle);
        sNewReading->Data[0] = s_rab_packet->Data[0] >> m_unNumberBitsToShift;
        sNewReading->Data[1] = s_rab_packet->Data[1];
        sNewReading->Data[2] = s_rab_packet->Data[2];
        sNewReading->Data[3] = s_rab_packet->Data[3];
        sNewReading->Fusion = true;
        pthread_mutex_lock(&m_tReadingsQueueMutex);
        m_tQueuedReadings.push_back(sNewReading);
        pthread_mutex_unlock(&m_tReadingsQueueMutex);
    }
    
    /****************************************/
    /****************************************/

    void CRealEPuckVirtualCamrabSensor::CreateReading(CCI_EPuckRangeAndBearingSensor::SReceivedPacket* s_rab_packet) {
        SReading* sNewReading = new SReading();
        sNewReading->Color = IntegerToColor(s_rab_packet->Data[0] & m_unColorMask);
        sNewReading->Distance = s_rab_packet->Range;
        sNewReading->Angle = RadiansToDegrees(s_rab_packet->Bearing);
        sNewReading->Data[0] = s_rab_packet->Data[0] >> m_unNumberBitsToShift;
        sNewReading->Data[1] = s_rab_packet->Data[1];
        sNewReading->Data[2] = s_rab_packet->Data[2];
        sNewReading->Data[3] = s_rab_packet->Data[3];
        sNewReading->Fusion = false;
        pthread_mutex_lock(&m_tReadingsQueueMutex);
        m_tQueuedReadings.push_back(sNewReading);
        pthread_mutex_unlock(&m_tReadingsQueueMutex);
    }


    /****************************************/
    /****************************************/

    CRealEPuckVirtualCamrabSensor::CRealEPuckVirtualCamrabSensor() :
        CCI_EPuckVirtualCamrabSensor()
    {

    }

    /****************************************/
    /****************************************/
    
    void CRealEPuckVirtualCamrabSensor::Init(TConfigurationNode& t_node){
        CCI_EPuckVirtualCamrabSensor::Init(t_node);
        
        try {
            m_pcRealEPuck = &CRealEPuck::GetInstance();
            m_pcCamera = m_pcRealEPuck->GetController().GetSensor<CCI_EPuckOmnidirectionalCameraSensor>("epuck_omnidirectional_camera");
            m_pcCamera->Enable();
            m_pcRabSensor = m_pcRealEPuck->GetController().GetSensor<CCI_EPuckRangeAndBearingSensor>("epuck_range_and_bearing");
        
            /*
              * Start threads and related data
              */
            ::pthread_mutex_init(&m_tReadingsQueueMutex, NULL);
            ::pthread_create(&m_tDataFetcherThread, NULL, &StartThread, this);
            
        } catch (CARGoSException ex) {
            LOGERR<<"Error while initializing the virtual Camrab sensor!\n" ;
            LOGERR<<"Make sure the omnidirectional and the range-and-bearing sensors \n";
            LOGERR<<"are intialized correctly  as the Camrab sensor uses both.\n";
        }
    }

    /****************************************/
    /****************************************/   
    
    void CRealEPuckVirtualCamrabSensor::DataFetcherThread() {
        /* Register the function CleanupThreadData to be executed upon thread cancellation */
        pthread_cleanup_push(VirtualSensorCleanupThreadData, &m_tReadingsQueueMutex);
        /* Actual thread logic */
        std::vector<CCI_EPuckRangeAndBearingSensor::SReceivedPacket*> vecRabPackets;
        std::vector<CCI_EPuckOmnidirectionalCameraSensor::SBlob*> vecAllBlobs;
        std::vector<CCI_EPuckOmnidirectionalCameraSensor::SBlob*> vecFilteredBlobs;
        const CCI_EPuckOmnidirectionalCameraSensor::SReadings* sCameraReadings;
        while(true) {      
            pthread_testcancel();
            if(m_pcRabSensor!= NULL){  
                // Collect packets from range-and-bearing-sensor
                vecRabPackets = m_pcRabSensor->GetPackets();
                if (!vecRabPackets.empty()) {
                    if (m_pcCamera != NULL) {
                        // Collect blobs from omnidirectional camera
                        sCameraReadings = &m_pcCamera->GetReadings();
                        vecAllBlobs = sCameraReadings->BlobList;
                        // Trim the blobs to have one per robot
                        vecFilteredBlobs = FilterBlobs(vecAllBlobs);
                    }
                    // Match SReceivedPackets and SBlobs
                    InferRobots(vecFilteredBlobs, vecRabPackets);
                }
            }
            m_pcRabSensor->ClearPackets();
            ::usleep(TIME_BETWEEN_TWO_RECEIVE);
        }
        pthread_cleanup_pop(1);
    }


    /****************************************/
    /****************************************/
    
    void* CRealEPuckVirtualCamrabSensor::StartThread(void* pt_arg) {
        reinterpret_cast<CRealEPuckVirtualCamrabSensor*>(pt_arg)->DataFetcherThread();
        return NULL;
    }


    /****************************************/
    /****************************************/
    
    void CRealEPuckVirtualCamrabSensor::ReceiveData() {
        pthread_mutex_lock(&m_tReadingsQueueMutex);
        while(! m_tQueuedReadings.empty()) {
            m_tReadings.push_back(m_tQueuedReadings.front());
            m_tQueuedReadings.pop_front();
        }
        pthread_mutex_unlock(&m_tReadingsQueueMutex);
    }


    /***************************************/
    /***************************************/
    
    void CRealEPuckVirtualCamrabSensor::InferRobots(std::vector<CCI_EPuckOmnidirectionalCameraSensor::SBlob*> vec_blobs, std::vector<CCI_EPuckRangeAndBearingSensor::SReceivedPacket*> vec_packets) {
        if ((!vec_blobs.empty()) && (!vec_packets.empty())) {
            UInt8 unCurrentColor;
            Real fCurrentDiff;
            std::vector<CCI_EPuckOmnidirectionalCameraSensor::SBlob*> vecBlobsWithSameColor;
            for (std::vector<CCI_EPuckRangeAndBearingSensor::SReceivedPacket*>::iterator itRAB = vec_packets.begin(); itRAB != vec_packets.end(); ++itRAB) {
                /* If RAB message has valid measurements */
                if ((*itRAB)->Bearing != CRadians::ZERO) {
                    /* Get color from RAB message */
                    unCurrentColor = (*itRAB)->Data[0] & m_unColorMask;
                    /* Filter blobs corresponding to the color in RAB message */
                    vecBlobsWithSameColor = GetBlobsWithColor(vec_blobs, unCurrentColor);
                    /* If such blobs exist, get the blob with closest angle to RAB angle and create reading */
                    if (!vecBlobsWithSameColor.empty()) {
                        Real fMinDiff = 1000;
                        CCI_EPuckOmnidirectionalCameraSensor::SBlob* sElected = NULL;
                        for (std::vector<CCI_EPuckOmnidirectionalCameraSensor::SBlob*>::iterator itBlob = vecBlobsWithSameColor.begin(); itBlob != vecBlobsWithSameColor.end(); ++itBlob) {
                            fCurrentDiff = std::abs( RadiansToDegrees((*itBlob)->Angle) - RadiansToDegrees((*itRAB)->Bearing) );
                            if ((fCurrentDiff < fMinDiff) && (fCurrentDiff < 10)){
                                sElected = (*itBlob);
                                fMinDiff = fCurrentDiff;
                            }
                            if (sElected != NULL)
								CreateReading(sElected, *itRAB);
                            vec_blobs.erase(std::remove(vec_blobs.begin(), vec_blobs.end(), sElected), vec_blobs.end());
                        }
                    } else {
                        /* If no blob with same color exist, create a SReading based on rab message only */
                        CreateReading(*itRAB);
                    }
                } else {
                    /* If RAB message does not contain valid measurement, create a SReading based on rab message only */
                    CreateReading(*itRAB);
                }
            }
        } else {   
            /* If no blobs are perceived at all, create a SReading based on rab message only */
            if (!vec_packets.empty()) {
                for (std::vector<CCI_EPuckRangeAndBearingSensor::SReceivedPacket*>::iterator itRAB = vec_packets.begin(); itRAB != vec_packets.end(); ++itRAB) {
                    CreateReading(*itRAB);
                }
            }
        
        }
    }


}

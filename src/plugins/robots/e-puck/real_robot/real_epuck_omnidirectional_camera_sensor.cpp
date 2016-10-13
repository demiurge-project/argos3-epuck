/**
 * @file <argos3/plugins/robots/e-puck/real_robot/real_epuck_omnidirectional_camera_sensor.cpp>
 *
 * @author Gianpiero Francesca - <gianpiero.francesca@ulb.ac.be>
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_omnidirectional_camera_sensor.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_omnidirectional_camera_sensor.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck.h>
#include <pthread.h>
#include <math.h>
namespace argos {

/****************************************/
/****************************************/

CRealEPuckOmnidirectionalCameraSensor::CRealEPuckOmnidirectionalCameraSensor() :
    CCI_EPuckOmnidirectionalCameraSensor(),
    m_pcImageGrabber(NULL),
    m_pcBlobDetector(NULL),
    m_un_blobDetection_cycles(0)
{

}

/****************************************/
/****************************************/
void CRealEPuckOmnidirectionalCameraSensor::cleanBlobs(){
    UInt32 size=m_sReadings.BlobList.size();
    for (UInt32 index = 0; index < size; ++index) {
        delete m_sReadings.BlobList[index];
    }
    m_sReadings.BlobList.clear();
    m_sReadings.Counter+=1;
}
/****************************************/
/****************************************/
void CRealEPuckOmnidirectionalCameraSensor::Update(){
    if(m_bEnabled){
        if(m_un_blobDetection_cycles>m_sReadings.Counter){
            cleanBlobs();
            pthread_mutex_lock(&m_tBlobQueueMutex);
            const std::vector<CBlobDetector::SDetectionData*>& cDetectors = m_pcBlobDetector->GetDetectors();
            UInt32 unDetectorNum = cDetectors.size();
            for(size_t i = 0; i < unDetectorNum; ++i) {
                /*Transfer blobs*/
                for(std::map<UInt16, CBlobDetector::SBlob>::iterator it = cDetectors[i]->LabelBlobs.begin();
                    it != cDetectors[i]->LabelBlobs.end();
                    ++it) {
                    m_sReadings.BlobList.push_back(convertBlob(cDetectors[i]->Color,it->second));
                }
            }
            pthread_mutex_unlock(&m_tBlobQueueMutex);
        }
    }
}

/****************************************/
/****************************************/

CCI_EPuckOmnidirectionalCameraSensor::SBlob* CRealEPuckOmnidirectionalCameraSensor::convertBlob(CColor& color,CBlobDetector::SBlob& in_blob){
    CVector2 cImagePointWRTCenter((Real)m_pcImageGrabber->GetImageCenterY() - (Real)in_blob.CenterJ,
                                  (Real)m_pcImageGrabber->GetImageCenterX() - (Real)in_blob.CenterI);
    SBlob* psSensorBlob = new SBlob(color,CRadians(CRadians::PI_OVER_TWO - cImagePointWRTCenter.Angle()).SignedNormalize(),
                                    m_pcDistanceConverter->PixelToCmDistance(cImagePointWRTCenter.Length()),in_blob.Area);
    return psSensorBlob;
}

/****************************************/
/****************************************/
void CRealEPuckOmnidirectionalCameraSensor::Init(TConfigurationNode& t_tree){
    std::string m_strConfFileName("/home/root/ARGoS/low_level_configuration/omnidirectional_camera_sensor.xml");
    try {

        GetNodeAttributeOrDefault(t_tree,
                                  "low_level_config",
                                  m_strConfFileName,m_strConfFileName
                                  );
    } catch (CARGoSException& ex) {
        THROW_ARGOSEXCEPTION_NESTED("Unable to init omnidirectional camera sensor, \"low_level_config\" "
                                    << "attribute is missing, "
                                    << "mandatory when \"calibrated\" attribute is true", ex);
    }
    /*
     * Load and parse the XML configuration file
     */
    ticpp::Document tConf(m_strConfFileName);
    tConf.LoadFile();
    
    /*
     * Initialize the image grabber and the detectors
     */
    m_pcImageGrabber = new CEPuckImageGrabber;
    m_pcImageGrabber->Init(*tConf.FirstChildElement());
    m_pcBlobDetector = new CBlobDetector(m_pcImageGrabber->GetImageWidth(),
                                         m_pcImageGrabber->GetImageHeight(),
                                         false);
    m_pcBlobDetector->Init(*tConf.FirstChildElement());
    /*
     * Initialize image buffer
     */
    m_pcHSVImage = new CImage<SHSV888Pixel>(m_pcImageGrabber->GetImageWidth(),
                                            m_pcImageGrabber->GetImageHeight());

	
	/* Initialize distance converter */
	m_pcDistanceConverter = new CDistanceConverter();
	m_pcDistanceConverter->Init(*tConf.FirstChildElement());
    /*
     * Start threads and related data
     */
    ::pthread_mutex_init(&m_tBlobQueueMutex, NULL);
    ::pthread_create(&m_tBlobFetcherThread, NULL, &StartThread, this);
}
/****************************************/
/****************************************/

void* CRealEPuckOmnidirectionalCameraSensor::StartThread(void* pt_arg) {
    reinterpret_cast<CRealEPuckOmnidirectionalCameraSensor*>(pt_arg)->BlobFetcherThread();
    return NULL;
}
/****************************************/
/****************************************/

static void CleanupThreadData(void* pt_data) {
    pthread_mutex_t* ptMutex = reinterpret_cast<pthread_mutex_t*>(pt_data);
    pthread_mutex_unlock(ptMutex);
}
/****************************************/
/****************************************/
void CRealEPuckOmnidirectionalCameraSensor::BlobFetcherThread() {
    /* Register the function CleanupThreadData to be executed upon thread cancellation */
    pthread_cleanup_push(CleanupThreadData, &m_tBlobQueueMutex);
    /* Actual thread logic */
    printf("Camera Thread Launched\n");
    while(m_bEnabled) {
        pthread_testcancel();
        /* Proceed only if data has been received */
        /* Grab image from camera */
        m_pcImageGrabber->GrabFrame(*m_pcHSVImage);
        /* Apply blob detectors */
        pthread_mutex_lock(&m_tBlobQueueMutex);
        m_pcBlobDetector->Detect(*m_pcHSVImage);
        pthread_mutex_unlock(&m_tBlobQueueMutex);
        m_un_blobDetection_cycles++;
    }
    cleanBlobs();
    printf("Camera Thread Closed\n");
    pthread_cleanup_pop(1);
}
}

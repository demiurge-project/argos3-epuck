/**
 * @file <argos3/plugins/robots/e-puck/real_robot/real_epuck_omnidirectional_camera_sensor.cpp>
 *
 * @author Gianpiero Francesca - <gianpiero.francesca@ulb.ac.be>
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */
#ifndef REAL_EPUCK_OMNIDIRECTIONAL_CAMERA_SENSOR_H
#define REAL_EPUCK_OMNIDIRECTIONAL_CAMERA_SENSOR_H



#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_omnidirectional_camera_sensor.h>
#include <argos3/plugins/robots/e-puck/real_robot/vision/epuck_image_grabber.h>
#include <argos3/plugins/robots/e-puck/real_robot/vision/blob_detector.h>
#include <argos3/plugins/robots/e-puck/real_robot/vision/distance_converter.h>

namespace argos {

/**
    * @file <argos3/plugins/robots/e-puck/
    * real_robot/real_epuck_omnidirectional_camera.h>
    *
    * @brief This file provides the definition of the epuck
    * omnidirectional camera.
    *
    * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
    * @author Gianpiero Francesca <gianpiero.francesca@ulb.ac.be>
    */
class CRealEPuckOmnidirectionalCameraSensor :
        virtual public CCI_EPuckOmnidirectionalCameraSensor {
private:
    void cleanBlobs();
    SBlob* convertBlob(CColor& color,CBlobDetector::SBlob& in_blob);
public:
    /**
       * Constructor
       */
    CRealEPuckOmnidirectionalCameraSensor();

    /**
       * Destructor
       */
    virtual ~CRealEPuckOmnidirectionalCameraSensor(){
        delete m_pcHSVImage;
        delete m_pcImageGrabber;
        delete m_pcBlobDetector;

    }

    virtual void Init(TConfigurationNode& t_tree);
    virtual void Enable(){
        if(!m_bEnabled){
            m_bEnabled=true;
            /*
               * Start threads and related data
               */
            ::pthread_mutex_init(&m_tBlobQueueMutex, NULL);
            ::pthread_create(&m_tBlobFetcherThread, NULL, &StartThread, this);
        }
    }

    virtual void Update();
protected:
    CImage<SHSV888Pixel>* m_pcHSVImage;
    CEPuckImageGrabber* m_pcImageGrabber;
    CBlobDetector* m_pcBlobDetector;
    CDistanceConverter* m_pcDistanceConverter;
private:
    /** Handle to the camera thread */
    pthread_t m_tBlobFetcherThread;

    /** This mutex protects the concurrent access to the packet queue */
    pthread_mutex_t m_tBlobQueueMutex;

    /** Number of blobdetection cycles */
    UInt32 m_un_blobDetection_cycles;
private:
    void BlobFetcherThread();

    static void* StartThread(void* pt_arg);
};
}
#endif

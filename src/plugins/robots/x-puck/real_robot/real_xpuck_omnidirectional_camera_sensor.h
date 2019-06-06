/**
 * @file <argos3/plugins/robots/x-puck/real_robot/real_xpuck_omnidirectional_camera_sensor.cpp>
 *
 * @author Gianpiero Francesca - <gianpiero.francesca@ulb.ac.be>
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */
#ifndef REAL_XPUCK_OMNIDIRECTIONAL_CAMERA_SENSOR_H
#define REAL_XPUCK_OMNIDIRECTIONAL_CAMERA_SENSOR_H



#include <argos3/plugins/robots/x-puck/control_interface/ci_xpuck_omnidirectional_camera_sensor.h>
#include <argos3/plugins/robots/x-puck/real_robot/vision/xpuck_image_grabber.h>
#include <argos3/plugins/robots/x-puck/real_robot/vision/blob_detector.h>
#include <argos3/plugins/robots/x-puck/real_robot/vision/distance_converter.h>

namespace argos {

/**
    * @file <argos3/plugins/robots/x-puck/
    * real_robot/real_xpuck_omnidirectional_camera.h>
    *
    * @brief This file provides the definition of the xpuck
    * omnidirectional camera.
    *
    * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
    * @author Gianpiero Francesca <gianpiero.francesca@ulb.ac.be>
    */
class CRealXPuckOmnidirectionalCameraSensor :
        virtual public CCI_XPuckOmnidirectionalCameraSensor {
private:
    void cleanBlobs();
    SBlob* convertBlob(CColor& color,CBlobDetector::SBlob& in_blob);
public:
    /**
       * Constructor
       */
    CRealXPuckOmnidirectionalCameraSensor();

    /**
       * Destructor
       */
    virtual ~CRealXPuckOmnidirectionalCameraSensor(){
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
    CXPuckImageGrabber* m_pcImageGrabber;
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

/**
 * @file <argos3/plugins/robots/e-puck/real_robot/real_epuck_virtual_camrab_sensor.h>
 *
 * @brief This file provides the implementation of the epuck virtual camrab sensor.
 *
 * The sensor combines distances estimated by the range and bearing and the 
 * omnidirectional sensor. After matching the SBlobs received from the omnidirectional
 * camera and the SReceivedPackets received fron the range and bearing sensor,
 * the distances are fusioned using an average over the global variance of the two sensors.
 *
 * @author Antoine Ligot <aligot@ulb.ac.be>
 */

#ifndef REAL_EPUCK_VIRTUAL_CAMRAB_SENSOR_H
#define REAL_EPUCK_VIRTUAL_CAMRAB_SENSOR_H

namespace argos {
   class CRealEPuckVirtualCamrabSensor;
   class CRealEPuck;
}

#include <list>
#include <string>
#include <pthread.h>
#include <algorithm>

#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_virtual_camrab_sensor.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_omnidirectional_camera_sensor.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_range_and_bearing_sensor.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_i2c_sensor.h>

namespace argos {


class CRealEPuckVirtualCamrabSensor :
	virtual public CCI_EPuckVirtualCamrabSensor,
	virtual public CRealEPuckI2CSensor {
		
public:
    /**
     * Constructor
     */
    CRealEPuckVirtualCamrabSensor();

	/**
     * Destructor
     */
    virtual ~CRealEPuckVirtualCamrabSensor(){}
    
    virtual void Init(TConfigurationNode& t_node);
    
    virtual void ReceiveData();

private:

	void DataFetcherThread();
    
    static void* StartThread(void* pt_arg);
    
    /** Creates a SReading based on matching SBlob and SReceivedPacket and add it to m_tQueuedReadings*/
    void CreateReading(CCI_EPuckOmnidirectionalCameraSensor::SBlob* s_blob, CCI_EPuckRangeAndBearingSensor::SReceivedPacket* s_rab_packet);

    /** 
     * Creates a SReading solely based on SReveivedPacket and add it to m_tQueuedReadings. 
     * Called when no SBlob/SReveivedPacket match could be found, but since
     * important rab data could be received, a SReading containing that info 
     * needs to be create. */
    void CreateReading(CCI_EPuckRangeAndBearingSensor::SReceivedPacket* s_rab_packet);
    
    /** Creates a list of readings composed of merged RAB and omnidirectional camera information. */
    void InferRobots(std::vector<CCI_EPuckOmnidirectionalCameraSensor::SBlob*> vec_blobs, std::vector<CCI_EPuckRangeAndBearingSensor::SReceivedPacket*> vec_packets);

private:
    /** Pointer to the omnidirectional camera sensor */
    CCI_EPuckOmnidirectionalCameraSensor* m_pcCamera;

    /** Pointer to the omnidirectional camera sensor */
    CCI_EPuckRangeAndBearingSensor* m_pcRabSensor;
    
    /** Pointer to the epuck implementation */
    CRealEPuck* m_pcRealEPuck;

    /** Flag that stores whether the sensor is calibrated */
    bool m_bCalibrated;

    /** Flag that stores whether the sensor is enabled */
    bool m_bEnabled;

    /** Handle to the message fetcher thread */
    pthread_t m_tDataFetcherThread;

    /** This mutex protects the concurrent access to the reading queue */
    pthread_mutex_t m_tReadingsQueueMutex;

    /**
     * This is a list of readings read by the message fetcher thread.
     * The packets in this list get read by ReceiveData() every time step.
     * After each ReceiveData(), the list gets cleared.
     */
    std::list<CCI_EPuckVirtualCamrabSensor::SReading *> m_tQueuedReadings;
};

}
#endif

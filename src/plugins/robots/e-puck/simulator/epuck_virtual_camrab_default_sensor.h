/**
 * @file <argos3/plugins/robots/e-puck/simulator/epuck_virtual_camrab_default_sensor.h>
 *
 * @author Antoine Ligot - <aligot@ulb.ac.be>
 *
 * Virtual camrab default sensor implementation for the e-puck simulation.
 */

#ifndef EPUCK_VIRTUAL_CAMRAB_DEFAULT_SENSOR_H
#define EPUCK_VIRTUAL_CAMRAB_DEFAULT_SENSOR_H

namespace argos {
   class CEPuckVirtualCamrabDefaultSensor;
}


#include <argos3/core/simulator/sensor.h>
#include <argos3/core/utility/datatypes/byte_array.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/plugins/robots/e-puck/simulator/epuck_rab_equipped_entity.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_virtual_camrab_sensor.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_range_and_bearing_sensor.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_omnidirectional_camera_sensor.h>

namespace argos {

   class CEPuckVirtualCamrabDefaultSensor : public CSimulatedSensor, public CCI_EPuckVirtualCamrabSensor {

   public:
		CEPuckVirtualCamrabDefaultSensor() {}
		virtual ~CEPuckVirtualCamrabDefaultSensor() {}
		virtual void SetRobot(CComposableEntity& c_entity);
		virtual void Init(TConfigurationNode& t_tree);
		virtual void Update();
      
		virtual void Reset();
		virtual void Destroy();
		
		/** Creates a list of readings composed of merged RAB and omnidirectional camera information. */
		void InferRobots(std::vector<CCI_EPuckOmnidirectionalCameraSensor::SBlob*> vec_blobs, std::vector<CCI_EPuckRangeAndBearingSensor::SReceivedPacket*> vec_packets);
		
		/** Create a single reading composed of RAB and omnidirectional camera sensed information. */
		void CreateReading(CCI_EPuckOmnidirectionalCameraSensor::SBlob* s_blob, CCI_EPuckRangeAndBearingSensor::SReceivedPacket* s_RAB_packet);
		
		/** Create a single reading composed solely of RAB sensed information */
		void CreateReading(CCI_EPuckRangeAndBearingSensor::SReceivedPacket* s_RAB_packet);

	private:
		/** Pointer to the range and bearing equipped entity */
		CEpuckRABEquippedEntity* m_pcRangeAndBearingEquippedEntity;
		
		/** Pointer to the controllable entity */
		CControllableEntity* m_pcControllableEntity;
		
		/** Pointer to the omnidirectional camera sensor. */
		CCI_EPuckOmnidirectionalCameraSensor* m_pcCamera;
		
		/** Pointer to the range and bearing sensor. */
		CCI_EPuckRangeAndBearingSensor* m_pcRabSensor;
   };
   
}

#endif

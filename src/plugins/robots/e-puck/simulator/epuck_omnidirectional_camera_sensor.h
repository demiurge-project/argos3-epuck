/**
 * @file <argos3/plugins/robots/e-puck/simulator/epuck_omnidirectional_camera_sensor.h>
 *
 * @author Gianpiero Francesca - <gianpiero.francesca@ulb.ac.be>
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */
#ifndef EPUCK_CAMERA_SENSOR_H
#define EPUCK_CAMERA_SENSOR_H

namespace argos {
   class CCI_EPuckOmnidirectionalCameraSensor;
   class COmnidirectionalCameraEquippedEntity;
   class CLEDEntity;
   class CControllableEntity;
   class CLEDCheckOperation;
}

#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_omnidirectional_camera_sensor.h>

namespace argos {

   class CEPuckOmnidirectionalCameraSensor : public CSimulatedSensor,
                                                           public CCI_EPuckOmnidirectionalCameraSensor {

   public:

      CEPuckOmnidirectionalCameraSensor();

      virtual ~CEPuckOmnidirectionalCameraSensor();

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

      virtual void Destroy();

            virtual void Enable();

            virtual void Disable();

   protected:

      bool                                  m_bEnabled;
      COmnidirectionalCameraEquippedEntity* m_pcOmnicamEntity;
      CControllableEntity*                  m_pcControllableEntity;
      CEmbodiedEntity*                      m_pcEmbodiedEntity;
      CPositionalIndex<CLEDEntity>*         m_pcLEDIndex;
      CPositionalIndex<CEmbodiedEntity>*    m_pcEmbodiedIndex;
      CLEDCheckOperation*                   m_pcOperation;
      Real                                  m_fDistanceNoiseStdDev;
      CRandom::CRNG*                        m_pcRNG;
      bool                                  m_bShowRays;

   };
}

#endif

/**
 * @file <argos3/plugins/robots/x-puck/simulator/xpuck_omnidirectional_camera_sensor.h>
 *
 * @author Gianpiero Francesca - <gianpiero.francesca@ulb.ac.be>
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */
#ifndef XPUCK_CAMERA_SENSOR_H
#define XPUCK_CAMERA_SENSOR_H

namespace argos {
   class CCI_XPuckOmnidirectionalCameraSensor;
   class COmnidirectionalCameraEquippedEntity;
   class CLEDEntity;
   class CControllableEntity;
   class CLEDCheckOperation;
}

#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>
#include <argos3/plugins/robots/x-puck/control_interface/ci_xpuck_omnidirectional_camera_sensor.h>

namespace argos {

   class CXPuckOmnidirectionalCameraSensor : public CSimulatedSensor,
                                                           public CCI_XPuckOmnidirectionalCameraSensor {

   public:

      CXPuckOmnidirectionalCameraSensor();

      virtual ~CXPuckOmnidirectionalCameraSensor();

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

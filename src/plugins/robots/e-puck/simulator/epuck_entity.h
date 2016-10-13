/**
 * @file <argos3/plugins/robots/e-puck/simulator/epuck_entity.h>
 * @author Gianpiero Francesca - <gianpiero.francesca@ulb.ac.be>
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 * @author Xavier Deschuyteneer - <xavier.deschuyteneer@gmail.com>
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef EPUCK_ENTITY_H
#define EPUCK_ENTITY_H

namespace argos {
   class CControllableEntity;
   class CEmbodiedEntity;
   class CEPuckEntity;
   class CGroundSensorEquippedEntity;
   class CLEDEquippedEntity;
   class CLightSensorEquippedEntity;
   class COmnidirectionalCameraEquippedEntity;
   class CProximitySensorEquippedEntity;
   class CBatteryEquippedEntity;
   class CRABEquippedEntity;
}

#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/plugins/simulator/entities/wheeled_entity.h>
#include <argos3/plugins/robots/e-puck/simulator/epuck_rab_equipped_entity.h>
namespace argos {

   class CEPuckEntity : public CComposableEntity {

   public:

      ENABLE_VTABLE();

   public:

      CEPuckEntity();

      CEPuckEntity(const std::string& str_id,
                     const std::string& str_controller_id,
                     const CVector3& c_position = CVector3(),
                     const CQuaternion& c_orientation = CQuaternion());


      virtual void Init(TConfigurationNode& t_tree);
      virtual void Reset();
      virtual void Destroy();

      virtual void UpdateComponents();

      inline CControllableEntity& GetControllableEntity() {
         return *m_pcControllableEntity;
      }

      inline CEmbodiedEntity& GetEmbodiedEntity() {
         return *m_pcEmbodiedEntity;
      }

      inline CGroundSensorEquippedEntity& GetGroundSensorEquippedEntity() {
         return *m_pcGroundSensorEquippedEntity;
      }

      inline CLEDEquippedEntity& GetLEDEquippedEntity() {
         return *m_pcLEDEquippedEntity;
      }

      inline CLightSensorEquippedEntity& GetLightSensorEquippedEntity() {
         return *m_pcLightSensorEquippedEntity;
      }
      inline COmnidirectionalCameraEquippedEntity& GetOmnidirectionalCameraEquippedEntity() {
         return *m_pcOmnidirectionalCameraEquippedEntity;
      }
      inline CProximitySensorEquippedEntity& GetProximitySensorEquippedEntity() {
         return *m_pcProximitySensorEquippedEntity;
      }
      inline CBatteryEquippedEntity& GetBatteryEquippedEntity() {
         return *m_pcBatteryEquippedEntity;
      }

      inline CEpuckRABEquippedEntity& GetRABEquippedEntity() {
         return *m_pcRABEquippedEntity;
      }

      inline CRABEquippedEntity& GetIRComEquippedEntity() {
         return *m_pcIRComEquippedEntity;
      }

      inline CWheeledEntity& GetWheeledEntity() {
         return *m_pcWheeledEntity;
      }

      virtual std::string GetTypeDescription() const {
         return "epuck";
      }

   private:

      void SetLEDPosition();

   private:
      CControllableEntity*                   m_pcControllableEntity;
      CEmbodiedEntity*                       m_pcEmbodiedEntity;
      CGroundSensorEquippedEntity*           m_pcGroundSensorEquippedEntity;
      CLEDEquippedEntity*                    m_pcLEDEquippedEntity;
      CLightSensorEquippedEntity*            m_pcLightSensorEquippedEntity;
       COmnidirectionalCameraEquippedEntity*  m_pcOmnidirectionalCameraEquippedEntity;
      CProximitySensorEquippedEntity*        m_pcProximitySensorEquippedEntity;
      CBatteryEquippedEntity*        m_pcBatteryEquippedEntity;
      CEpuckRABEquippedEntity*                    m_pcRABEquippedEntity;
      CRABEquippedEntity*                    m_pcIRComEquippedEntity;
      CWheeledEntity*                        m_pcWheeledEntity;
   public:
      static const Real BODY_RADIUS                ;
      static const Real BODY_HEIGHT                ;

      static const Real INTERWHEEL_DISTANCE        ;
      static const Real HALF_INTERWHEEL_DISTANCE   ;
      static const Real WHEEL_RADIUS               ;

      static const Real PROXIMITY_SENSOR_RING_ELEVATION       ;
      static const Real PROXIMITY_SENSOR_RING_RADIUS          ;
      static const CRadians PROXIMITY_SENSOR_RING_START_ANGLE ;
      static const Real PROXIMITY_SENSOR_RING_RANGE           ;

      static const CRadians LED_RING_START_ANGLE   ;
      static const Real LED_RING_RADIUS            ;
      static const Real LED_RING_ELEVATION         ;

      static const Real RGB_LED_RING_ELEVATION     ;

      static const Real RAB_ELEVATION              ;
      static const Real OMNIDIRECTIONAL_CAMERA_ELEVATION ;
   };

}

#endif

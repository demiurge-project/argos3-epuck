/**
 * @file <argos3/plugins/robots/e-puck/simulator/epuck_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "epuck_entity.h"

#include <argos3/core/utility/math/matrix/rotationmatrix3.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/plugins/simulator/entities/rab_equipped_entity.h>
#include <argos3/plugins/simulator/entities/ground_sensor_equipped_entity.h>
#include <argos3/plugins/simulator/entities/led_equipped_entity.h>
#include <argos3/plugins/simulator/entities/light_sensor_equipped_entity.h>
#include <argos3/plugins/simulator/entities/omnidirectional_camera_equipped_entity.h>
#include <argos3/plugins/simulator/entities/proximity_sensor_equipped_entity.h>
#include <argos3/plugins/robots/e-puck/simulator/battery_equipped_entity.h>
#include <argos3/plugins/robots/e-puck/simulator/epuck_battery_sensor.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_range_and_bearing_sensor.h>

#include <argos3/core/utility/math/general.h>
namespace argos {

/****************************************/
/****************************************/

const Real CEPuckEntity::BODY_RADIUS                = 0.035f;
 const Real  CEPuckEntity::BODY_HEIGHT                =  0.076f;

 const Real  CEPuckEntity::INTERWHEEL_DISTANCE        = 0.053f;
 const Real  CEPuckEntity::HALF_INTERWHEEL_DISTANCE   = INTERWHEEL_DISTANCE * 0.5f;
 const Real  CEPuckEntity::WHEEL_RADIUS               = 0.0205f;

 const Real  CEPuckEntity::PROXIMITY_SENSOR_RING_ELEVATION       = 0.032f;
 const Real  CEPuckEntity::PROXIMITY_SENSOR_RING_RADIUS          = CEPuckEntity::BODY_RADIUS + 0.001;
 const CRadians  CEPuckEntity::PROXIMITY_SENSOR_RING_START_ANGLE = CRadians((ARGOS_PI / 8.0f) * 0.5f);
 const Real  CEPuckEntity::PROXIMITY_SENSOR_RING_RANGE           = 0.05f;

 const CRadians  CEPuckEntity::LED_RING_START_ANGLE   = CRadians((ARGOS_PI / 8.0f) * 0.5f);
 const Real  CEPuckEntity::LED_RING_RADIUS            = CEPuckEntity::BODY_RADIUS + 0.001;
 const Real  CEPuckEntity::LED_RING_ELEVATION         = 0.04f;

 const Real  CEPuckEntity::RGB_LED_RING_ELEVATION     = 0.0765f;

 const Real  CEPuckEntity::RAB_ELEVATION              = 0.052f;
 const Real  CEPuckEntity::OMNIDIRECTIONAL_CAMERA_ELEVATION = 0.122f;
/***********************************/
/****************************************/

CEPuckEntity::CEPuckEntity() :
    CComposableEntity(NULL),
    m_pcControllableEntity(NULL),
    m_pcEmbodiedEntity(NULL),
    m_pcGroundSensorEquippedEntity(NULL),
    m_pcLEDEquippedEntity(NULL),
    m_pcLightSensorEquippedEntity(NULL),
    m_pcProximitySensorEquippedEntity(NULL),
    m_pcBatteryEquippedEntity(NULL),
    m_pcRABEquippedEntity(NULL),
    m_pcWheeledEntity(NULL) {}


/****************************************/
/****************************************/

CEPuckEntity::CEPuckEntity(const std::string& str_id,
               const std::string& str_controller_id,
               const CVector3& c_position,
               const CQuaternion& c_orientation):
    CComposableEntity(NULL, str_id),
    m_pcControllableEntity(NULL),
    m_pcEmbodiedEntity(NULL),
    m_pcGroundSensorEquippedEntity(NULL),
    m_pcLEDEquippedEntity(NULL),
    m_pcLightSensorEquippedEntity(NULL),
    m_pcProximitySensorEquippedEntity(NULL),
    m_pcBatteryEquippedEntity(NULL),
    m_pcRABEquippedEntity(NULL),
    m_pcWheeledEntity(NULL){
  try {

      /*
        * Create and init components
        */
      /* Embodied entity */
      m_pcEmbodiedEntity = new CEmbodiedEntity(this, "body_0", c_position, c_orientation);
      AddComponent(*m_pcEmbodiedEntity);
      /* Wheeled entity and wheel positions (left, right) */
      m_pcWheeledEntity = new CWheeledEntity(this, "wheels_0", 2);
      AddComponent(*m_pcWheeledEntity);
      m_pcWheeledEntity->SetWheel(0, CVector3(0.0f,  HALF_INTERWHEEL_DISTANCE, 0.0f), WHEEL_RADIUS);
      m_pcWheeledEntity->SetWheel(1, CVector3(0.0f, -HALF_INTERWHEEL_DISTANCE, 0.0f), WHEEL_RADIUS);
      /* LED equipped entity, with LEDs [0-7] */
      m_pcLEDEquippedEntity = new CLEDEquippedEntity(this,
                                                     "leds_0");
      AddComponent(*m_pcLEDEquippedEntity);
      m_pcLEDEquippedEntity->AddLEDRing(
                  CVector3(0.0f, 0.0f, LED_RING_ELEVATION),
                  LED_RING_RADIUS,
                  LED_RING_START_ANGLE,
                  8,
                  m_pcEmbodiedEntity->GetOriginAnchor());
      /* RGB LED equipped entity, with LEDS [8-11] */
      m_pcLEDEquippedEntity->AddLED(CVector3( 0.018f,  0.0311769f, RGB_LED_RING_ELEVATION),m_pcEmbodiedEntity->GetOriginAnchor());
      m_pcLEDEquippedEntity->AddLED(CVector3( 0.018f, -0.0311769f, RGB_LED_RING_ELEVATION),m_pcEmbodiedEntity->GetOriginAnchor());
      m_pcLEDEquippedEntity->AddLED(CVector3(-(BODY_RADIUS+0.001f), 0.0f, RGB_LED_RING_ELEVATION),m_pcEmbodiedEntity->GetOriginAnchor());
      /* To improve speed, disable the LED equipped entity
        * It will be enabled only by an LED actuator, if used */
      m_pcLEDEquippedEntity->Disable();
      /* Proximity sensor equipped entity */
      m_pcProximitySensorEquippedEntity =
              new CProximitySensorEquippedEntity(this,
                                                 "proximity_0");
      AddComponent(*m_pcProximitySensorEquippedEntity);
      m_pcProximitySensorEquippedEntity->AddSensorRing(
                  CVector3(0.0f, 0.0f, PROXIMITY_SENSOR_RING_ELEVATION),
                  PROXIMITY_SENSOR_RING_RADIUS,
                  PROXIMITY_SENSOR_RING_START_ANGLE,
                  PROXIMITY_SENSOR_RING_RANGE,
                  8,
                  m_pcEmbodiedEntity->GetOriginAnchor());
      /* Battery sensor equipped entity */
      m_pcBatteryEquippedEntity =
              new CBatteryEquippedEntity(this,"battery_0");
      AddComponent(*m_pcBatteryEquippedEntity);
      /* Light sensor equipped entity */
      m_pcLightSensorEquippedEntity =
              new CLightSensorEquippedEntity(this,
                                             "light_0");
      AddComponent(*m_pcLightSensorEquippedEntity);
      m_pcLightSensorEquippedEntity->AddSensorRing(
                  CVector3(0.0f, 0.0f, PROXIMITY_SENSOR_RING_ELEVATION),
                  PROXIMITY_SENSOR_RING_RADIUS,
                  PROXIMITY_SENSOR_RING_START_ANGLE,
                  PROXIMITY_SENSOR_RING_RANGE,
                  8,
                  m_pcEmbodiedEntity->GetOriginAnchor());
      /* Ground sensor equipped entity */
      m_pcGroundSensorEquippedEntity =
              new CGroundSensorEquippedEntity(this,
                                              "ground_0");
      AddComponent(*m_pcGroundSensorEquippedEntity);
      m_pcGroundSensorEquippedEntity->AddSensor(CVector2(0.03f, -0.009f),
                                                CGroundSensorEquippedEntity::TYPE_GRAYSCALE,m_pcEmbodiedEntity->GetOriginAnchor());
      m_pcGroundSensorEquippedEntity->AddSensor(CVector2(0.03f,  0.0f),
                                                CGroundSensorEquippedEntity::TYPE_GRAYSCALE,m_pcEmbodiedEntity->GetOriginAnchor());
      m_pcGroundSensorEquippedEntity->AddSensor(CVector2(0.03f,  0.009f),
                                                CGroundSensorEquippedEntity::TYPE_GRAYSCALE,m_pcEmbodiedEntity->GetOriginAnchor());
      /* RAB equipped entity */
      m_pcRABEquippedEntity = new CEpuckRABEquippedEntity(this,
                                                     "rab_0",
                                                     CCI_EPuckRangeAndBearingSensor::MAX_BYTES_RECEIVED,
                                                     0.8f,
                                                     m_pcEmbodiedEntity->GetOriginAnchor(),
                                                     *m_pcEmbodiedEntity,
                                                     CVector3(0.0f, 0.0f, RAB_ELEVATION),CQuaternion().FromEulerAngles(CRadians::ZERO,CRadians::ZERO,CRadians::ZERO));
      AddComponent(*m_pcRABEquippedEntity);
      /* To improve speed, disable the RAB equipped entity
        * It will be enabled only by a RAB actuator, if used */
      m_pcRABEquippedEntity->Disable();

      /* Omnidirectional camera equipped entity */
      CDegrees cAperture(70.0f);
      //GetNodeAttributeOrDefault(t_tree, "omnidirectional_camera_aperture", cAperture, cAperture);
      m_pcOmnidirectionalCameraEquippedEntity = new COmnidirectionalCameraEquippedEntity(this,
                                                                                         "omnidirectional_camera_0",
                                                                                         ToRadians(cAperture),
                                                                                         CVector3(0.0f, 0.0f, OMNIDIRECTIONAL_CAMERA_ELEVATION));
      AddComponent(*m_pcOmnidirectionalCameraEquippedEntity);

      /* IRCom equipped entity */
      /* TODO
       * Create an IRCOMEquippedEntity for the epuck and the TAM, in which, from the basic RABEquippedEntity of the epuck,
       *  we add a flag that tells whether the actuator is active or not. On the sensor, after the check of the medium
       * (which will not change and will be always the RAB medium), we check whether the actuator we intersected is actually enabled or not.
       */
      m_pcIRComEquippedEntity = new CEpuckRABEquippedEntity(this,
                                                            "rab_1",
                                                            1,
                                                            0.05f,
                                                            m_pcEmbodiedEntity->GetOriginAnchor(),
                                                            *m_pcEmbodiedEntity,
                                                            CVector3(0.0f, 0.0f, RAB_ELEVATION),CQuaternion().FromEulerAngles(CRadians::ZERO,CRadians::ZERO,CRadians::ZERO));
      AddComponent(*m_pcIRComEquippedEntity);
      m_pcIRComEquippedEntity->Disable();

      /* Controllable entity
          It must be the last one, for actuators/sensors to link to composing entities correctly */
      m_pcControllableEntity = new CControllableEntity(this, "controller_0");
      AddComponent(*m_pcControllableEntity);
      m_pcControllableEntity->SetController(str_controller_id);
      /* Update components */
      UpdateComponents();
  }
  catch(CARGoSException& ex) {
      LOG<<ex.what()<<"\n";
      THROW_ARGOSEXCEPTION_NESTED("Failed to initialize entity \"" << GetId() << "\".", ex);
  }


}


/****************************************/
/****************************************/

void CEPuckEntity::Init(TConfigurationNode& t_tree) {
    try {
        /*
          * Init parent
          */
        CComposableEntity::Init(t_tree);
        /*
          * Create and init components
          */
        /* Embodied entity */
        m_pcEmbodiedEntity = new CEmbodiedEntity(this);
        AddComponent(*m_pcEmbodiedEntity);
        m_pcEmbodiedEntity->Init(GetNode(t_tree, "body"));
        /* Wheeled entity and wheel positions (left, right) */
        m_pcWheeledEntity = new CWheeledEntity(this, "wheels_0", 2);
        AddComponent(*m_pcWheeledEntity);
        m_pcWheeledEntity->SetWheel(0, CVector3(0.0f,  HALF_INTERWHEEL_DISTANCE, 0.0f), WHEEL_RADIUS);
        m_pcWheeledEntity->SetWheel(1, CVector3(0.0f, -HALF_INTERWHEEL_DISTANCE, 0.0f), WHEEL_RADIUS);
        /* LED equipped entity, with LEDs [0-7] */
        m_pcLEDEquippedEntity = new CLEDEquippedEntity(this,
                                                       "leds_0");
        AddComponent(*m_pcLEDEquippedEntity);
        m_pcLEDEquippedEntity->AddLEDRing(
                    CVector3(0.0f, 0.0f, LED_RING_ELEVATION),
                    LED_RING_RADIUS,
                    LED_RING_START_ANGLE,
                    8,
                    m_pcEmbodiedEntity->GetOriginAnchor());
        /* RGB LED equipped entity, with LEDS [8-11] */
        m_pcLEDEquippedEntity->AddLED(CVector3( 0.018f,  0.0311769f, RGB_LED_RING_ELEVATION),m_pcEmbodiedEntity->GetOriginAnchor());
        m_pcLEDEquippedEntity->AddLED(CVector3( 0.018f, -0.0311769f, RGB_LED_RING_ELEVATION),m_pcEmbodiedEntity->GetOriginAnchor());
        m_pcLEDEquippedEntity->AddLED(CVector3(-(BODY_RADIUS+0.001f), 0.0f, RGB_LED_RING_ELEVATION),m_pcEmbodiedEntity->GetOriginAnchor());
        /* To improve speed, disable the LED equipped entity
          * It will be enabled only by an LED actuator, if used */
        m_pcLEDEquippedEntity->Disable();
        /* Proximity sensor equipped entity */
        m_pcProximitySensorEquippedEntity =
                new CProximitySensorEquippedEntity(this,
                                                   "proximity_0");
        AddComponent(*m_pcProximitySensorEquippedEntity);
        m_pcProximitySensorEquippedEntity->AddSensorRing(
                    CVector3(0.0f, 0.0f, PROXIMITY_SENSOR_RING_ELEVATION),
                    PROXIMITY_SENSOR_RING_RADIUS,
                    PROXIMITY_SENSOR_RING_START_ANGLE,
                    PROXIMITY_SENSOR_RING_RANGE,
                    8,
                    m_pcEmbodiedEntity->GetOriginAnchor());
        /* Battery sensor equipped entity */
        m_pcBatteryEquippedEntity =
                new CBatteryEquippedEntity(this,"battery_0");
        AddComponent(*m_pcBatteryEquippedEntity);
        /* Light sensor equipped entity */
        m_pcLightSensorEquippedEntity =
                new CLightSensorEquippedEntity(this,
                                               "light_0");
        AddComponent(*m_pcLightSensorEquippedEntity);
        m_pcLightSensorEquippedEntity->AddSensorRing(
                    CVector3(0.0f, 0.0f, PROXIMITY_SENSOR_RING_ELEVATION),
                    PROXIMITY_SENSOR_RING_RADIUS,
                    PROXIMITY_SENSOR_RING_START_ANGLE,
                    PROXIMITY_SENSOR_RING_RANGE,
                    8,
                    m_pcEmbodiedEntity->GetOriginAnchor());
        /* Ground sensor equipped entity */
        m_pcGroundSensorEquippedEntity =
                new CGroundSensorEquippedEntity(this,
                                                "ground_0");
        AddComponent(*m_pcGroundSensorEquippedEntity);
        m_pcGroundSensorEquippedEntity->AddSensor(CVector2(0.03f, -0.009f),
                                                  CGroundSensorEquippedEntity::TYPE_GRAYSCALE,m_pcEmbodiedEntity->GetOriginAnchor());
        m_pcGroundSensorEquippedEntity->AddSensor(CVector2(0.03f,  0.0f),
                                                  CGroundSensorEquippedEntity::TYPE_GRAYSCALE,m_pcEmbodiedEntity->GetOriginAnchor());
        m_pcGroundSensorEquippedEntity->AddSensor(CVector2(0.03f,  0.009f),
                                                  CGroundSensorEquippedEntity::TYPE_GRAYSCALE,m_pcEmbodiedEntity->GetOriginAnchor());
        /* RAB equipped entity */
        m_pcRABEquippedEntity = new CEpuckRABEquippedEntity(this,
                                                       "rab_0",
                                                       CCI_EPuckRangeAndBearingSensor::MAX_BYTES_RECEIVED,
                                                       0.8f,
                                                       m_pcEmbodiedEntity->GetOriginAnchor(),
                                                       *m_pcEmbodiedEntity,
                                                       CVector3(0.0f, 0.0f, RAB_ELEVATION));
        AddComponent(*m_pcRABEquippedEntity);
        /* To improve speed, disable the RAB equipped entity
          * It will be enabled only by a RAB actuator, if used */
        m_pcRABEquippedEntity->Disable();

        /* Omnidirectional camera equipped entity */
        CDegrees cAperture(70.0f);
        GetNodeAttributeOrDefault(t_tree, "omnidirectional_camera_aperture", cAperture, cAperture);
        m_pcOmnidirectionalCameraEquippedEntity = new COmnidirectionalCameraEquippedEntity(this,
                                                                                           "omnidirectional_camera_0",
                                                                                           ToRadians(cAperture),
                                                                                           CVector3(0.0f, 0.0f, OMNIDIRECTIONAL_CAMERA_ELEVATION));
        AddComponent(*m_pcOmnidirectionalCameraEquippedEntity);

        /* IRCom equipped entity */
        /* TODO
         * Create an IRCOMEquippedEntity for the epuck and the TAM, in which, from the basic RABEquippedEntity of the epuck,
         *  we add a flag that tells whether the actuator is active or not. On the sensor, after the check of the medium
         * (which will not change and will be always the RAB medium), we check whether the actuator we intersected is actually enabled or not.
         */
        m_pcIRComEquippedEntity = new CEpuckRABEquippedEntity(this,
                                                              "rab_1",
                                                              1,
                                                              0.05f,
                                                              m_pcEmbodiedEntity->GetOriginAnchor(),
                                                              *m_pcEmbodiedEntity,
                                                              CVector3(0.0f, 0.0f, RAB_ELEVATION));
        AddComponent(*m_pcIRComEquippedEntity);
        m_pcIRComEquippedEntity->Disable();

        /* Controllable entity
            It must be the last one, for actuators/sensors to link to composing entities correctly */
        m_pcControllableEntity = new CControllableEntity(this);
        AddComponent(*m_pcControllableEntity);
        m_pcControllableEntity->Init(GetNode(t_tree, "controller"));
        /* Update components */
        UpdateComponents();
    }
    catch(CARGoSException& ex) {
        LOG<<ex.what()<<"\n";
        THROW_ARGOSEXCEPTION_NESTED("Failed to initialize entity \"" << GetId() << "\".", ex);
    }
}

/****************************************/
/****************************************/

void CEPuckEntity::Reset() {
    /* Reset all components */
    CComposableEntity::Reset();
    /* Update components */
    UpdateComponents();
}

/****************************************/
/****************************************/

void CEPuckEntity::Destroy() {
    CComposableEntity::Destroy();
}

/****************************************/
/****************************************/

#define UPDATE(COMPONENT) if(COMPONENT->IsEnabled()) COMPONENT->Update();

void CEPuckEntity::UpdateComponents() {
    UPDATE(m_pcRABEquippedEntity);
    UPDATE(m_pcIRComEquippedEntity);
    UPDATE(m_pcLEDEquippedEntity);
}

/****************************************/
/****************************************/

REGISTER_ENTITY(CEPuckEntity,
                "e-puck",
                "Carlo Pinciroli [ilpincy@gmail.com]",
                "1.0",
                "The e-puck robot.",
                "The e-puck is a open-hardware, extensible robot intended for education. In its\n"
                "simplest form, it is a two-wheeled robot equipped with proximity sensors,\n"
                "ground sensors, light sensors, a microphone, a frontal camera, and a ring of\n"
                "red LEDs. More information is available at http://www.epuck.org\n\n"
                "REQUIRED XML CONFIGURATION\n\n"
                "  <arena ...>\n"
                "    ...\n"
                "    <e-puck id=\"fb0\">\n"
                "      <body position=\"0.4,2.3,0.25\" orientation=\"45,90,0\" />\n"
                "      <controller config=\"mycntrl\" />\n"
                "    </e-puck>\n"
                "    ...\n"
                "  </arena>\n\n"
                "The 'id' attribute is necessary and must be unique among the entities. If two\n"
                "entities share the same id, initialization aborts.\n"
                "The 'body/position' attribute specifies the position of the pucktom point of the\n"
                "e-puck in the arena. When the robot is untranslated and unrotated, the\n"
                "pucktom point is in the origin and it is defined as the middle point between\n"
                "the two wheels on the XY plane and the lowest point of the robot on the Z\n"
                "axis, that is the point where the wheels touch the floor. The attribute values\n"
                "are in the X,Y,Z order.\n"
                "The 'body/orientation' attribute specifies the orientation of the e-puck. All\n"
                "rotations are performed with respect to the pucktom point. The order of the\n"
                "angles is Z,Y,X, which means that the first number corresponds to the rotation\n"
                "around the Z axis, the second around Y and the last around X. This reflects\n"
                "the internal convention used in ARGoS, in which rotations are performed in\n"
                "that order. Angles are expressed in degrees. When the robot is unrotated, it\n"
                "is oriented along the X axis.\n"
                "The 'controller/config' attribute is used to assign a controller to the\n"
                "e-puck. The value of the attribute must be set to the id of a previously\n"
                "defined controller. Controllers are defined in the <controllers> XML subtree.\n\n"
                "OPTIONAL XML CONFIGURATION\n\n"
                "None for the time being.\n",
                "Under development"
                );

/****************************************/
/****************************************/

REGISTER_STANDARD_SPACE_OPERATIONS_ON_COMPOSABLE(CEPuckEntity);

/****************************************/
/****************************************/

}

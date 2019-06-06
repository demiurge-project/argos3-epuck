/**
 * @file <argos3/plugins/robots/x-puck/simulator/dynamics2d_xpuck_model.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#include "dynamics2d_xpuck_model.h"
#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_gripping.h>
#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_engine.h>

namespace argos {

   /****************************************/
   /****************************************/

   static const Real XPUCK_RADIUS                   = CXPuckEntity::BODY_RADIUS;
   static const Real XPUCK_INTERWHEEL_DISTANCE      = CXPuckEntity::INTERWHEEL_DISTANCE;
   static const Real XPUCK_HEIGHT                   =CXPuckEntity::BODY_HEIGHT;

   static const Real XPUCK_MAX_FORCE                = 1.5f;
   static const Real XPUCK_MAX_TORQUE               = 1.5f;

   enum XPUCK_WHEELS {
      XPUCK_LEFT_WHEEL = 0,
      XPUCK_RIGHT_WHEEL = 1
   };

   /****************************************/
   /****************************************/

   CDynamics2DXPuckModel::CDynamics2DXPuckModel(CDynamics2DEngine& c_engine,
                                                CXPuckEntity& c_entity) :
      CDynamics2DSingleBodyObjectModel(c_engine, c_entity),
      m_cXPuckEntity(c_entity),
      m_cWheeledEntity(m_cXPuckEntity.GetWheeledEntity()),
      m_cDiffSteering(c_engine,
                      XPUCK_MAX_FORCE,
                      XPUCK_MAX_TORQUE,
                      XPUCK_INTERWHEEL_DISTANCE),
      m_fMass(0.4f),
      m_fCurrentWheelVelocity(m_cWheeledEntity.GetWheelVelocities()) {
      /* Create the body with initial position and orientation */
      m_ptBaseBody =
         cpSpaceAddBody(GetDynamics2DEngine().GetPhysicsSpace(),
                        cpBodyNew(m_fMass,
                                  cpMomentForCircle(m_fMass,
                                                    0.0f,
                                                    XPUCK_RADIUS + XPUCK_RADIUS,
                                                    cpvzero)));
      const CVector3& cPosition = GetEmbodiedEntity().GetOriginAnchor().Position;
      m_ptBaseBody->p = cpv(cPosition.GetX(), cPosition.GetY());
      CRadians cXAngle, cYAngle, cZAngle;
      GetEmbodiedEntity().GetOriginAnchor().Orientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
      cpBodySetAngle(m_ptBaseBody, cZAngle.GetValue());
      /* Create the body shape */
      m_ptBaseShape =
         cpSpaceAddShape(GetDynamics2DEngine().GetPhysicsSpace(),
                         cpCircleShapeNew(m_ptBaseBody,
                                          XPUCK_RADIUS,
                                          cpvzero));
      m_ptBaseShape->e = 0.0; // No elasticity
      m_ptBaseShape->u = 0.7; // Lots of friction
      /* Constrain the base body to follow the diff steering control */
      m_cDiffSteering.AttachTo(m_ptBaseBody);
      /* Associate this model to the body data for ray queries */
      SetBody(m_ptBaseBody,XPUCK_HEIGHT);
   }

   /****************************************/
   /****************************************/

   CDynamics2DXPuckModel::~CDynamics2DXPuckModel() {
      m_cDiffSteering.Detach();
   }

   /****************************************/
   /****************************************/

   void CDynamics2DXPuckModel::Reset() {
      CDynamics2DSingleBodyObjectModel::Reset();
      m_cDiffSteering.Reset();

   }

   /****************************************/
   /****************************************/

   void CDynamics2DXPuckModel::UpdateFromEntityStatus() {
      /* Do we want to move? */
      if((m_fCurrentWheelVelocity[XPUCK_LEFT_WHEEL] != 0.0f) ||
         (m_fCurrentWheelVelocity[XPUCK_RIGHT_WHEEL] != 0.0f)) {
         m_cDiffSteering.SetWheelVelocity(m_fCurrentWheelVelocity[XPUCK_LEFT_WHEEL],
                                          m_fCurrentWheelVelocity[XPUCK_RIGHT_WHEEL]);
      }
      else {
         /* No, we don't want to move - zero all speeds */
         m_cDiffSteering.Reset();
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_DYNAMICS2D_OPERATIONS_ON_ENTITY(CXPuckEntity, CDynamics2DXPuckModel);

   /****************************************/
   /****************************************/

}

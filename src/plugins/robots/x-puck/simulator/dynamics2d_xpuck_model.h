/**
 * @file <argos3/plugins/robots/x-puck/simulator/dynamics2d_xpuck_model.h>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef DYNAMICS2D_XPUCK_MODEL_H
#define DYNAMICS2D_XPUCK_MODEL_H

namespace argos {
   class CDynamics2DDifferentialSteeringControl;
   class CDynamics2DGripper;
   class CDynamics2DGrippable;
}

#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_model.h>
#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_single_body_object_model.h>
#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_differentialsteering_control.h>
#include <argos3/plugins/robots/x-puck/simulator/xpuck_entity.h>

namespace argos {

   class CDynamics2DXPuckModel : public CDynamics2DSingleBodyObjectModel {

   public:

      CDynamics2DXPuckModel(CDynamics2DEngine& c_engine,
                              CXPuckEntity& c_entity);
      virtual ~CDynamics2DXPuckModel();

      virtual void Reset();

      virtual void UpdateFromEntityStatus();

   private:

      CXPuckEntity& m_cXPuckEntity;
      CWheeledEntity& m_cWheeledEntity;

      CDynamics2DDifferentialSteeringControl m_cDiffSteering;

      cpFloat  m_fMass;
      cpShape* m_ptBaseShape;
      cpBody*  m_ptBaseBody;

      const Real* m_fCurrentWheelVelocity;

   };

}

#endif

/**
 * @file <argos3/plugins/robots/x-puck/control_interface/ci_xpuck_micro_sensor.h
 *
 * @brief This file provides the implementation of the xpuck micro
 * sensor.
 *
 * This file provides the definition of the xpuck micro sensor.  The
 * sensors are evenly spaced on the left, right and back to the base
 * of the xpuck.  The micros are placed as follow:
 *
 *              front
 *   l                          r
 *   e   MIC_L        MIC_R i
 *   f           .            g
 *   t                          h
 *             MIC_B            t
 *
 *              back
 *
 *
 * @author Lorenzo Garattoni- <lgaratto@ulb.ac.be>
 * @author Matteo Amaducci <matteo.amaducci2@studio.unibo.it>
 */

#include "ci_xpuck_micro_sensor.h"

namespace argos {

   /****************************************/
   /****************************************/

   std::ostream& operator<<(std::ostream& c_os,
                            const CCI_XPuckMicroSensor::SReading& s_reading) {
      c_os << "SReading("
           << s_reading.Right
           << ","
           << s_reading.Left
           << ","
           << s_reading.Back
           << ")";
      return c_os;
   }

   /****************************************/
   /****************************************/
}

/**
 * @file <argos3/plugins/robots/x-puck/control_interface/ci_xpuck_accelerometer_sensor.cpp
 *
 * @author Lorenzo Garattoni- <lgaratto@ulb.ac.be>
 */
#include "ci_xpuck_accelerometer_sensor.h"

namespace argos {

   std::ostream& operator<<(std::ostream& c_os,
                            const CCI_XPuckAccelerometerSensor::SReading& s_reading) {
      c_os << "SReading("
           << s_reading.X
           << ","
           << s_reading.Y
           << ","
           << s_reading.Z
           << ")";
      return c_os;
   }
}

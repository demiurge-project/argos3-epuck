/**
 * @file <argos3/plugins/robots/e-puck/control_interface/ci_epuck_accelerometer_sensor.cpp
 *
 * @author Lorenzo Garattoni- <lgaratto@ulb.ac.be>
 */
#include "ci_epuck_accelerometer_sensor.h"

namespace argos {

   std::ostream& operator<<(std::ostream& c_os,
                            const CCI_EPuckAccelerometerSensor::SReading& s_reading) {
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

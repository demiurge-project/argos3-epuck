#
# e-puck control interface headers
#
set(ARGOS3_HEADERS_PLUGINS_ROBOTS_EPUCK_CONTROLINTERFACE
  # Sensors
  control_interface/ci_epuck_proximity_sensor.h
  control_interface/ci_epuck_range_and_bearing_sensor.h
  control_interface/ci_epuck_ground_sensor.h
  control_interface/ci_epuck_light_sensor.h
  control_interface/ci_epuck_encoder_sensor.h
  control_interface/ci_epuck_accelerometer_sensor.h
  control_interface/ci_epuck_ircom_sensor.h
  control_interface/ci_epuck_micro_sensor.h
  control_interface/ci_epuck_battery_sensor.h
  control_interface/ci_epuck_omnidirectional_camera_sensor.h
  control_interface/ci_epuck_virtual_camrab_sensor.h
  # control_interface/ci_epuck_camera_sensor.h
  # control_interface/ci_epuck_front_camera_sensor.h
  # Actuators
  control_interface/ci_epuck_base_leds_actuator.h
  control_interface/ci_epuck_rgb_leds_actuator.h
  control_interface/ci_epuck_range_and_bearing_actuator.h
  control_interface/ci_epuck_wheels_actuator.h
  control_interface/ci_epuck_ircom_actuator.h
  control_interface/ci_epuck_virtual_camrab_actuator.h)

#
# e-puck control interface sources
#
set(ARGOS3_SOURCES_PLUGINS_ROBOTS_EPUCK
  ${ARGOS3_HEADERS_PLUGINS_ROBOTS_EPUCK_CONTROLINTERFACE}
  # Sensors
  control_interface/ci_epuck_proximity_sensor.cpp
  control_interface/ci_epuck_battery_sensor.cpp
  control_interface/ci_epuck_range_and_bearing_sensor.cpp
  control_interface/ci_epuck_ground_sensor.cpp
  control_interface/ci_epuck_light_sensor.cpp
  control_interface/ci_epuck_encoder_sensor.cpp
  control_interface/ci_epuck_accelerometer_sensor.cpp
  control_interface/ci_epuck_micro_sensor.cpp
  control_interface/ci_epuck_omnidirectional_camera_sensor.cpp
  control_interface/ci_epuck_virtual_camrab_sensor.cpp
  # Actuators
  control_interface/ci_epuck_base_leds_actuator.cpp
  control_interface/ci_epuck_rgb_leds_actuator.cpp
  control_interface/ci_epuck_range_and_bearing_actuator.cpp
  control_interface/ci_epuck_wheels_actuator.cpp
  control_interface/ci_epuck_ircom_actuator.cpp
  control_interface/ci_epuck_virtual_camrab_actuator.cpp)

#
# Install location for the e-puck control interface headers
#
install(
  FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_EPUCK_CONTROLINTERFACE}
  DESTINATION include/argos3/plugins/robots/e-puck/control_interface)

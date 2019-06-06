#
# x-puck control interface headers
#
set(ARGOS3_HEADERS_PLUGINS_ROBOTS_XPUCK_CONTROLINTERFACE
  # Sensors
  control_interface/ci_xpuck_proximity_sensor.h
  control_interface/ci_xpuck_range_and_bearing_sensor.h
  control_interface/ci_xpuck_ground_sensor.h
  control_interface/ci_xpuck_light_sensor.h
  control_interface/ci_xpuck_encoder_sensor.h
  control_interface/ci_xpuck_accelerometer_sensor.h
  control_interface/ci_xpuck_ircom_sensor.h
  control_interface/ci_xpuck_micro_sensor.h
  control_interface/ci_xpuck_battery_sensor.h
  control_interface/ci_xpuck_omnidirectional_camera_sensor.h
  control_interface/ci_xpuck_virtual_camrab_sensor.h
  # control_interface/ci_xpuck_camera_sensor.h
  # control_interface/ci_xpuck_front_camera_sensor.h
  # Actuators
  control_interface/ci_xpuck_base_leds_actuator.h
  control_interface/ci_xpuck_rgb_leds_actuator.h
  control_interface/ci_xpuck_range_and_bearing_actuator.h
  control_interface/ci_xpuck_wheels_actuator.h
  control_interface/ci_xpuck_ircom_actuator.h
  control_interface/ci_xpuck_virtual_camrab_actuator.h)

#
# x-puck control interface sources
#
set(ARGOS3_SOURCES_PLUGINS_ROBOTS_XPUCK
  ${ARGOS3_HEADERS_PLUGINS_ROBOTS_XPUCK_CONTROLINTERFACE}
  # Sensors
  control_interface/ci_xpuck_proximity_sensor.cpp
  control_interface/ci_xpuck_battery_sensor.cpp
  control_interface/ci_xpuck_range_and_bearing_sensor.cpp
  control_interface/ci_xpuck_ground_sensor.cpp
  control_interface/ci_xpuck_light_sensor.cpp
  control_interface/ci_xpuck_encoder_sensor.cpp
  control_interface/ci_xpuck_accelerometer_sensor.cpp
  control_interface/ci_xpuck_micro_sensor.cpp
  control_interface/ci_xpuck_omnidirectional_camera_sensor.cpp
  control_interface/ci_xpuck_virtual_camrab_sensor.cpp
  # Actuators
  control_interface/ci_xpuck_base_leds_actuator.cpp
  control_interface/ci_xpuck_rgb_leds_actuator.cpp
  control_interface/ci_xpuck_range_and_bearing_actuator.cpp
  control_interface/ci_xpuck_wheels_actuator.cpp
  control_interface/ci_xpuck_ircom_actuator.cpp
  control_interface/ci_xpuck_virtual_camrab_actuator.cpp)

#
# Install location for the x-puck control interface headers
#
install(
  FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_XPUCK_CONTROLINTERFACE}
  DESTINATION include/argos3/plugins/robots/x-puck/control_interface)

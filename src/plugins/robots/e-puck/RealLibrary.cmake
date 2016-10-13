#
# e-puck real robot code
#
# Base headers
set(ARGOS3_HEADERS_PLUGINS_ROBOTS_EPUCK_REALROBOT
  # Base
  real_robot/real_epuck.h
  real_robot/real_epuck_base.h
  real_robot/real_epuck_i2c_device.h
  real_robot/real_epuck_i2c_sensor.h
  real_robot/real_epuck_i2c_actuator.h
  # Sensors
  real_robot/real_epuck_serial_sensor.h
  real_robot/real_epuck_proximity_sensor.h
  real_robot/real_epuck_battery_sensor.h
  real_robot/real_epuck_light_sensor.h
  real_robot/real_epuck_ground_sensor.h
  real_robot/real_epuck_accelerometer_sensor.h
  real_robot/real_epuck_range_and_bearing_sensor.h
  real_robot/real_epuck_ircom_sensor.h
  real_robot/real_epuck_micro_sensor.h
  real_robot/real_epuck_omnidirectional_camera_sensor.h
  real_robot/real_epuck_virtual_camrab_sensor.h
  # Actuators
  real_robot/real_epuck_serial_actuator.h
  real_robot/real_epuck_wheels_actuator.h
  real_robot/real_epuck_base_leds_actuator.h
  real_robot/real_epuck_rgb_leds_actuator.h
  real_robot/real_epuck_range_and_bearing_actuator.h
  real_robot/real_epuck_ircom_actuator.h
  real_robot/real_epuck_virtual_camrab_actuator.h)
# Install location
install(
  FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_EPUCK_REALROBOT}
  DESTINATION include/argos3/plugins/robots/e-puck/real_robot)
# Vision headers
set(ARGOS3_HEADERS_PLUGINS_ROBOTS_EPUCK_REALROBOT_VISION
  real_robot/vision/blob_detector.h
  real_robot/vision/client_server_protocol.h
  real_robot/vision/epuck_camera_configurator.h
  real_robot/vision/epuck_image_grabber.h
  real_robot/vision/extended_configurable_resource.h
  real_robot/vision/image.h
  real_robot/vision/image_grabber.h
  real_robot/vision/distance_converter.h)
# Install location
install(
  FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_EPUCK_REALROBOT_VISION}
  DESTINATION include/argos3/plugins/robots/e-puck/real_robot/vision)

# Library sources
set(ARGOS3_SOURCES_PLUGINS_ROBOTS_EPUCK
  ${ARGOS3_SOURCES_PLUGINS_ROBOTS_EPUCK}
  ${ARGOS3_HEADERS_PLUGINS_ROBOTS_EPUCK_REALROBOT}
  ${ARGOS3_HEADERS_PLUGINS_ROBOTS_EPUCK_REALROBOT_VISION}
  # Base
  real_robot/real_epuck.cpp
  real_robot/real_epuck_main.cpp
  real_robot/real_epuck_base.cpp
  real_robot/real_epuck_i2c_device.cpp
  # Sensors
  real_robot/real_epuck_proximity_sensor.cpp
  real_robot/real_epuck_battery_sensor.cpp
  real_robot/real_epuck_light_sensor.cpp
  real_robot/real_epuck_ground_sensor.cpp
  real_robot/real_epuck_accelerometer_sensor.cpp
  real_robot/real_epuck_range_and_bearing_sensor.cpp
  real_robot/real_epuck_ircom_sensor.cpp
  real_robot/real_epuck_micro_sensor.cpp
  real_robot/real_epuck_omnidirectional_camera_sensor.cpp
  real_robot/real_epuck_virtual_camrab_sensor.cpp
  # Actuators
  real_robot/real_epuck_wheels_actuator.cpp
  real_robot/real_epuck_base_leds_actuator.cpp
  real_robot/real_epuck_rgb_leds_actuator.cpp
  real_robot/real_epuck_range_and_bearing_actuator.cpp
  real_robot/real_epuck_ircom_actuator.cpp
  real_robot/real_epuck_virtual_camrab_actuator.cpp
  # Vision
  real_robot/vision/blob_detector.cpp
  real_robot/vision/epuck_image_grabber.cpp
  real_robot/vision/distance_converter.cpp
  real_robot/vision/image.cpp)

#
# Create e-puck library
#
# Create target
add_library(argos3plugin_${ARGOS_BUILD_FOR}_epuck SHARED
  ${ARGOS3_SOURCES_PLUGINS_ROBOTS_EPUCK})
# Link libraries
target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_epuck
  argos3core_${ARGOS_BUILD_FOR}
  ${PTHREADS_LIBRARY})
# Install location
install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_epuck
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)



#
# Create configurator server
#
set(ARGOS3_SERVER_SOURCES_PLUGINS_ROBOTS_EPUCK_REALROBOT_VISION
  ${ARGOS3_HEADERS_PLUGINS_ROBOTS_EPUCK_REALROBOT_VISION}
  real_robot/vision/blob_detector.cpp
  real_robot/vision/epuck_camera_configurator.cpp
  real_robot/vision/epuck_image_grabber.cpp
  real_robot/vision/distance_converter.cpp
  real_robot/vision/image.cpp)
add_executable(epuck_camera_configurator_server
  ${ARGOS3_SERVER_SOURCES_PLUGINS_ROBOTS_EPUCK_REALROBOT_VISION})
target_link_libraries(epuck_camera_configurator_server
  argos3core_${ARGOS_BUILD_FOR})

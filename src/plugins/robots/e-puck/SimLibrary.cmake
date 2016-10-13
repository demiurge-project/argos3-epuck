#
# e-puck simulator code
#
# Headers
set(ARGOS3_HEADERS_PLUGINS_ROBOTS_EPUCK_SIMULATOR
  simulator/dynamics2d_epuck_model.h
  simulator/epuck_entity.h
  simulator/battery_equipped_entity.h
  simulator/epuck_base_leds_default_actuator.h
  simulator/epuck_rgb_leds_default_actuator.h
  simulator/epuck_wheels_default_actuator.h
  simulator/epuck_range_and_bearing_default_actuator.h
  simulator/epuck_virtual_camrab_default_actuator.h
  simulator/epuck_virtual_camrab_default_sensor.h
  simulator/epuck_ircom_default_actuator.h
  simulator/epuck_omnidirectional_camera_sensor.h
  simulator/epuck_proximity_default_sensor.h
  simulator/epuck_battery_sensor.h
  simulator/epuck_light_default_sensor.h
  simulator/epuck_range_and_bearing_default_sensor.h
  simulator/epuck_ircom_default_sensor.h
  simulator/epuck_ground_rotzonly_sensor.h
  simulator/epuck_rab_equipped_entity.h)
if(ARGOS_COMPILE_QTOPENGL)
  set(ARGOS3_HEADERS_PLUGINS_ROBOTS_EPUCK_SIMULATOR
    ${ARGOS3_HEADERS_PLUGINS_ROBOTS_EPUCK_SIMULATOR}
    simulator/qtopengl_epuck.h)
endif(ARGOS_COMPILE_QTOPENGL)

# Install location for the e-puck simulator headers
install(
  FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_EPUCK_SIMULATOR}
  DESTINATION include/argos3/plugins/robots/e-puck/simulator)

# Sources
set(ARGOS3_SOURCES_PLUGINS_ROBOTS_EPUCK
  ${ARGOS3_SOURCES_PLUGINS_ROBOTS_EPUCK}
  ${ARGOS3_HEADERS_PLUGINS_ROBOTS_EPUCK_SIMULATOR}
  simulator/dynamics2d_epuck_model.cpp
  simulator/epuck_entity.cpp
  simulator/battery_equipped_entity.cpp
  simulator/epuck_base_leds_default_actuator.cpp
  simulator/epuck_rgb_leds_default_actuator.cpp
  simulator/epuck_wheels_default_actuator.cpp
  simulator/epuck_range_and_bearing_default_actuator.cpp
  simulator/epuck_virtual_camrab_default_actuator.cpp
  simulator/epuck_virtual_camrab_default_sensor.cpp
  simulator/epuck_ircom_default_actuator.cpp
  simulator/epuck_omnidirectional_camera_sensor.cpp
  simulator/epuck_proximity_default_sensor.cpp
  simulator/epuck_battery_sensor.cpp
  simulator/epuck_light_default_sensor.cpp
  simulator/epuck_range_and_bearing_default_sensor.cpp
  simulator/epuck_ircom_default_sensor.cpp
  simulator/epuck_ground_rotzonly_sensor.cpp
  simulator/epuck_rab_equipped_entity.cpp)
if(ARGOS_COMPILE_QTOPENGL)
  set(ARGOS3_SOURCES_PLUGINS_ROBOTS_EPUCK
    ${ARGOS3_SOURCES_PLUGINS_ROBOTS_EPUCK}
    simulator/qtopengl_epuck.cpp)
endif(ARGOS_COMPILE_QTOPENGL)

#
# Create e-puck plugin
#
# Create target
add_library(argos3plugin_${ARGOS_BUILD_FOR}_epuck SHARED
  ${ARGOS3_SOURCES_PLUGINS_ROBOTS_EPUCK})
# Link libraries
target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_epuck
  argos3core_${ARGOS_BUILD_FOR}
  argos3plugin_${ARGOS_BUILD_FOR}_genericrobot
  argos3plugin_${ARGOS_BUILD_FOR}_dynamics2d)
if(ARGOS_COMPILE_QTOPENGL)
  target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_epuck
    argos3plugin_${ARGOS_BUILD_FOR}_qtopengl)
endif(ARGOS_COMPILE_QTOPENGL)

# Install location for the e-puck plugin
install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_epuck
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)

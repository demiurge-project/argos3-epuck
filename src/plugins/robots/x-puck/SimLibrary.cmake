#
# x-puck simulator code
#
# Headers
set(ARGOS3_HEADERS_PLUGINS_ROBOTS_XPUCK_SIMULATOR
  simulator/dynamics2d_xpuck_model.h
  simulator/xpuck_entity.h
  simulator/battery_equipped_entity.h
  simulator/xpuck_base_leds_default_actuator.h
  simulator/xpuck_rgb_leds_default_actuator.h
  simulator/xpuck_wheels_default_actuator.h
  simulator/xpuck_range_and_bearing_default_actuator.h
  simulator/xpuck_virtual_camrab_default_actuator.h
  simulator/xpuck_virtual_camrab_default_sensor.h
  simulator/xpuck_ircom_default_actuator.h
  simulator/xpuck_omnidirectional_camera_sensor.h
  simulator/xpuck_proximity_default_sensor.h
  simulator/xpuck_battery_sensor.h
  simulator/xpuck_light_default_sensor.h
  simulator/xpuck_range_and_bearing_default_sensor.h
  simulator/xpuck_ircom_default_sensor.h
  simulator/xpuck_ground_rotzonly_sensor.h
  simulator/xpuck_rab_equipped_entity.h)
if(ARGOS_COMPILE_QTOPENGL)
  set(ARGOS3_HEADERS_PLUGINS_ROBOTS_XPUCK_SIMULATOR
    ${ARGOS3_HEADERS_PLUGINS_ROBOTS_XPUCK_SIMULATOR}
    simulator/qtopengl_xpuck.h)
endif(ARGOS_COMPILE_QTOPENGL)

# Install location for the x-puck simulator headers
install(
  FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_XPUCK_SIMULATOR}
  DESTINATION include/argos3/plugins/robots/x-puck/simulator)

# Sources
set(ARGOS3_SOURCES_PLUGINS_ROBOTS_XPUCK
  ${ARGOS3_SOURCES_PLUGINS_ROBOTS_XPUCK}
  ${ARGOS3_HEADERS_PLUGINS_ROBOTS_XPUCK_SIMULATOR}
  simulator/dynamics2d_xpuck_model.cpp
  simulator/xpuck_entity.cpp
  simulator/battery_equipped_entity.cpp
  simulator/xpuck_base_leds_default_actuator.cpp
  simulator/xpuck_rgb_leds_default_actuator.cpp
  simulator/xpuck_wheels_default_actuator.cpp
  simulator/xpuck_range_and_bearing_default_actuator.cpp
  simulator/xpuck_virtual_camrab_default_actuator.cpp
  simulator/xpuck_virtual_camrab_default_sensor.cpp
  simulator/xpuck_ircom_default_actuator.cpp
  simulator/xpuck_omnidirectional_camera_sensor.cpp
  simulator/xpuck_proximity_default_sensor.cpp
  simulator/xpuck_battery_sensor.cpp
  simulator/xpuck_light_default_sensor.cpp
  simulator/xpuck_range_and_bearing_default_sensor.cpp
  simulator/xpuck_ircom_default_sensor.cpp
  simulator/xpuck_ground_rotzonly_sensor.cpp
  simulator/xpuck_rab_equipped_entity.cpp)
if(ARGOS_COMPILE_QTOPENGL)
  set(ARGOS3_SOURCES_PLUGINS_ROBOTS_XPUCK
    ${ARGOS3_SOURCES_PLUGINS_ROBOTS_XPUCK}
    simulator/qtopengl_xpuck.cpp)
endif(ARGOS_COMPILE_QTOPENGL)

#
# Create x-puck plugin
#
# Create target
add_library(argos3plugin_${ARGOS_BUILD_FOR}_xpuck SHARED
  ${ARGOS3_SOURCES_PLUGINS_ROBOTS_XPUCK})
# Link libraries
target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_xpuck
  argos3core_${ARGOS_BUILD_FOR}
  argos3plugin_${ARGOS_BUILD_FOR}_genericrobot
  argos3plugin_${ARGOS_BUILD_FOR}_dynamics2d)
if(ARGOS_COMPILE_QTOPENGL)
  target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_xpuck
    argos3plugin_${ARGOS_BUILD_FOR}_qtopengl)
endif(ARGOS_COMPILE_QTOPENGL)

# Install location for the x-puck plugin
install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_xpuck
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)

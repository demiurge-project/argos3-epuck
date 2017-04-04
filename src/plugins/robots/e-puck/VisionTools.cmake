#
# We use  CMake's Qt includes of ArGoS3
#

#
# Vision headers
#
set(ARGOS3_HEADERS_PLUGINS_ROBOTS_EPUCK_REALROBOT_VISION
  real_robot/vision/blob_detector.h
  real_robot/vision/client_server_protocol.h
  real_robot/vision/epuck_camera_configurator.h
  real_robot/vision/epuck_image_grabber.h
  real_robot/vision/extended_configurable_resource.h
  real_robot/vision/image.h
  real_robot/vision/image_grabber.h
  real_robot/vision/distance_converter.h)

#
# Vision MOC headers
# The MOC is now automatic with QT5
#


#
# Vision sources
#
set(ARGOS3_CLIENT_SOURCES_PLUGINS_ROBOTS_EPUCK_REALROBOT_VISION
  ${ARGOS3_HEADERS_PLUGINS_ROBOTS_EPUCK_REALROBOT_VISION}
  ${ARGOS3_MOC_SOURCE_PLUGINS_ROBOTS_EPUCK_REALROBOT_VISION}
  real_robot/vision/epuck_camera_client_blob_detector_configurator.cpp
  real_robot/vision/epuck_camera_client_image_grabber_configurator.cpp
  real_robot/vision/epuck_camera_client_distance_converter_configurator.cpp
  real_robot/vision/epuck_camera_client_main_window.cpp
  real_robot/vision/image.cpp)
#
# Add the current binary dir to the include path to allow the compiler to find
# the moc-generated files
#
include_directories(${CMAKE_CURRENT_BINARY_DIR})
#
# Create target
#
add_executable(epuck_camera_configurator_client
  ${ARGOS3_CLIENT_SOURCES_PLUGINS_ROBOTS_EPUCK_REALROBOT_VISION})

target_link_libraries(epuck_camera_configurator_client
  ${ARGOS_QTOPENGL_LIBRARIES}
  argos3core_${ARGOS_BUILD_FOR})



#
# Testing stuff
#
add_executable(test_image_conversions
  real_robot/vision/image.cpp
  real_robot/vision/test_image_conversions.cpp)
target_link_libraries(test_image_conversions
  argos3core_${ARGOS_BUILD_FOR})

add_executable(test_blob_detection
  real_robot/vision/blob_detector.h
  real_robot/vision/blob_detector.cpp
  real_robot/vision/image.h
  real_robot/vision/image.cpp
  real_robot/vision/test_blob_detection.cpp)
target_link_libraries(test_blob_detection
  argos3core_${ARGOS_BUILD_FOR})

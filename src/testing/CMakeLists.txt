if(ARGOS_BUILD_FOR_SIMULATOR)
  add_library(console MODULE console.h client.h console.cpp client.cpp)
  target_link_libraries(console argos3plugin_${ARGOS_BUILD_FOR}_epuck)
  
  add_library(watcher MODULE watcher.h watcher.cpp)
  target_link_libraries(watcher argos3plugin_${ARGOS_BUILD_FOR}_epuck)
  
  add_library(tam_experiment_epuck MODULE tam_experiment_epuck.h tam_experiment_epuck.cpp)
  target_link_libraries(tam_experiment_epuck argos3plugin_${ARGOS_BUILD_FOR}_epuck)
  
  add_library(rab_selective_test MODULE rab_selective_test.h rab_selective_test.cpp)
  target_link_libraries(rab_selective_test argos3plugin_${ARGOS_BUILD_FOR}_epuck)

  #add_subdirectory(NEAT.1.2.1)
else()
  add_executable(console
    console.h
    console.cpp
    client.h
    client.cpp
    ../plugins/robots/e-puck/real_robot/real_epuck_main.cpp)
  target_link_libraries(console argos3plugin_${ARGOS_BUILD_FOR}_epuck)

  add_executable(watcher
    watcher.h
    watcher.cpp
    ../plugins/robots/e-puck/real_robot/real_epuck_main.cpp)
  target_link_libraries(watcher argos3plugin_${ARGOS_BUILD_FOR}_epuck)

  add_executable(calibration
    calibrate_epuck.h
    calibrate_epuck.cpp
    ../plugins/robots/e-puck/real_robot/real_epuck_main.cpp)
  target_link_libraries(calibration argos3plugin_${ARGOS_BUILD_FOR}_epuck)

  #add_subdirectory(NEAT.1.2.1)
endif(ARGOS_BUILD_FOR_SIMULATOR)

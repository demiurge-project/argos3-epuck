#
# Get information about the current processor
#
execute_process(
  COMMAND uname -m
  COMMAND tr -d '\n'
  OUTPUT_VARIABLE ARGOS_PROCESSOR_ARCH)

#
# General compilation flags
#
set(CMAKE_C_FLAGS                "-Wall")
set(CMAKE_C_FLAGS_RELEASE        "-Os -DNDEBUG")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-Os -ggdb3 -DNDEBUG")
set(CMAKE_C_FLAGS_DEBUG          "-ggdb3")

set(CMAKE_CXX_FLAGS                "-Wall")
set(CMAKE_CXX_FLAGS_RELEASE        "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Os -ggdb3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG          "-ggdb3")

if(APPLE)
  # MAC OSX
  # Allow for dynamic lookup of undefined symbols
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -undefined dynamic_lookup")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -undefined dynamic_lookup")
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -undefined dynamic_lookup")
  set(ARGOS_SHARED_LIBRARY_EXTENSION "dylib")
  set(ARGOS_MODULE_LIBRARY_EXTENSION "so")
  set(ARGOS_DYNAMIC_LIBRARY_VARIABLE "DYLD_LIBRARY_PATH")
  # Add address sanitizer support for CLang
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsanitize=address -fno-optimize-sibling-calls -fno-omit-frame-pointer")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-optimize-sibling-calls -fno-omit-frame-pointer")
  set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_DEBUG} ${CMAKE_C_FLAGS_RELEASE}")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_CXX_FLAGS_DEBUG}")
  set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS_DEBUG} -fsanitize=address")
  set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} -fsanitize=address")
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address")
  set(CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_MODULE_LINKER_FLAGS_RELEASE} ${CMAKE_MODULE_LINKER_FLAGS_DEBUG}")
  set(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} ${CMAKE_SHARED_LINKER_FLAGS_DEBUG}")
  set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELEASE} ${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
else(APPLE)
  # Linux
  # Avoid discarding unused symbols to allow plugins to work
  set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-as-needed")
  set(ARGOS_SHARED_LIBRARY_EXTENSION "so")
  set(ARGOS_MODULE_LIBRARY_EXTENSION "so")
  set(ARGOS_DYNAMIC_LIBRARY_VARIABLE "LD_LIBRARY_PATH")
endif(APPLE)

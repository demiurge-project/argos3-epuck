# Set system name
set(CMAKE_SYSTEM_NAME Linux)

# Set tool paths
set(CMAKE_AR xc16-ar CACHE STRING "XC16 ar command")
set(CMAKE_ASM_COMPILER xc16-gcc CACHE STRING "XC16 as command")
#set(CMAKE_ASM_COMPILER xc16-as CACHE STRING "XC16 as command")
set(CMAKE_C_COMPILER xc16-gcc CACHE STRING "XC16 gcc command")
set(CMAKE_CXX_COMPILER g++ CACHE STRING "local g++ command")
set(CMAKE_LINKER xc16-ld CACHE STRING "XC16 ld command")
set(CMAKE_NM xc16-nm CACHE STRING "XC16 nm command")
set(CMAKE_OBJCOPY xc16-objcopy CACHE STRING "XC16 objcopy command")
set(CMAKE_OBJDUMP xc16-objdump CACHE STRING "XC16 objdump command")
set(CMAKE_RANLIB xc16-ranlib CACHE STRING "XC16 ranlib command")
set(CMAKE_STRIP xc16-strip CACHE STRING "XC16 strip command")

# Compilation flags
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--heap=512 -Wl,--script=p30F6014A.gld" CACHE STRING "XC16 default LDFLAGS")

set(CMAKE_MODULE_LINKER_FLAGS "-Wl,--heap=512 -Wl,--script=p30F6014A.gld" CACHE STRING "XC16 default LDFLAGS")

set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--heap=512 -Wl,--script=p30F6014A.gld" CACHE STRING "XC16 default LDFLAGS")

set(CMAKE_C_FLAGS "-g -Wall -mcpu=30F6014A" CACHE STRING "XC16 default CFLAGS")

# Enable assembly language
set(CMAKE_ASM_FLAGS "-g -Wall -mcpu=30F6014A" CACHE STRING "XC16 asm flags")
enable_language(ASM)

#set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_VERBOSE_MAKEFILE off)

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/a_d
  ${CMAKE_SOURCE_DIR}/a_d/advance_ad_scan 
  ${CMAKE_SOURCE_DIR}/bluetooth
  ${CMAKE_SOURCE_DIR}/btcom
  ${CMAKE_SOURCE_DIR}/randb
  ${CMAKE_SOURCE_DIR}/camera/fast_2_timer
  ${CMAKE_SOURCE_DIR}/codec
  ${CMAKE_SOURCE_DIR}/I2C
  ${CMAKE_SOURCE_DIR}/motor_led/advance_one_timer
  ${CMAKE_SOURCE_DIR}/motor_led
  ${CMAKE_SOURCE_DIR}/uart
        ${CMAKE_SOURCE_DIR}/ircom

)
link_directories(
  ${CMAKE_BINARY_DIR}/a_d/advance_ad_scan
    ${CMAKE_BINARY_DIR}/a_d
        ${CMAKE_SOURCE_DIR}/randb
  ${CMAKE_BINARY_DIR}/bluetooth
  ${CMAKE_BINARY_DIR}/btcom
  ${CMAKE_BINARY_DIR}/camera/fast_2_timer
  ${CMAKE_BINARY_DIR}/codec
  ${CMAKE_BINARY_DIR}/I2C
  ${CMAKE_BINARY_DIR}/motor_led/advance_one_timer
  ${CMAKE_BINARY_DIR}/motor_led
  ${CMAKE_BINARY_DIR}/uart
        ${CMAKE_BINARY_DIR}/ircom
)

project(epucklib)
cmake_minimum_required(VERSION 2.6)

add_subdirectory(uart)
add_subdirectory(motor_led)
add_subdirectory(a_d)
add_subdirectory(I2C)

add_subdirectory(bluetooth)

add_subdirectory(btcom)
add_subdirectory(firmware)
add_subdirectory(ircom)

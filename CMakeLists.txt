cmake_minimum_required(VERSION 3.7)

include(FetchContent)

FetchContent_Declare(
  microcpp
  GIT_REPOSITORY https://github.com/wilypomegranate/microcpp.git
  GIT_TAG devel
  )

FetchContent_GetProperties(microcpp)
if(NOT microcpp_POPULATED)
  FetchContent_Populate(microcpp)
  add_subdirectory(${microcpp_SOURCE_DIR} ${microcpp_BINARY_DIR})
endif()

project(thermostat VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

SET(MICROCPP_WIFI_SSID "" CACHE STRING "WIFI SSID")
SET(MICROCPP_WIFI_PW "" CACHE STRING "WIFI PW")
SET(SERIAL_DEVICE "" CACHE STRING "SERIAL DEVICE TO FLASH")
SET(ESP_BAUD_RATE "" CACHE STRING "ESP SERIAL BAUD RATE")

if (MICROCPP_WIFI_SSID STREQUAL "")
  message(FATAL_ERROR "MICROCPP_WIFI_SSID must be provided.")
endif()

if (MICROCPP_WIFI_PW STREQUAL "")
  message(FATAL_ERROR "MICROCPP_WIFI_PW must be provided.")
endif()

if (SERIAL_DEVICE STREQUAL "")
  message(FATAL_ERROR "SERIAL_DEVICE must be provided.")
endif()

file(GLOB SOURCES src/*.cpp)
add_executable(main.elf ${SOURCES})
set_target_properties(main.elf PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(main.elf PRIVATE ${microcpp_SOURCE_DIR}/src)
target_compile_definitions(main.elf PRIVATE
  MICROCPP_WIFI_SSID="${MICROCPP_WIFI_SSD}"
  MICROCPP_WIFI_PW="${MICROCPP_WIFI_PW}"
  )

add_custom_target(
  main.objcopy ALL
  DEPENDS main.hex
  )

add_custom_command(
  OUTPUT main.hex
  COMMAND avr-objcopy -O ihex -R .eeprom main.elf main.hex
  DEPENDS main.elf
  )

add_custom_target(
  flash
  COMMAND avrdude -F -V -c arduino -p ATMEGA328P -P ${SERIAL_DEVICE} -b 115200 -U flash:w:main.hex
  DEPENDS main.hex
  )

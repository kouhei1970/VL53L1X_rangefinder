cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(VL53LX_rangefinder C CXX ASM)
set(CMAKE_C_STNDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

pico_sdk_init()
#include_directories(/Users/itoukouhei/pico/eigen)

add_executable(VL53LX_rangefinder
  main.c
  vl53l1_platform.c
  VL53L1X_api.c
  VL53L1X_calibration.c
)

target_link_libraries(VL53LX_rangefinder
  hardware_i2c 
  hardware_uart 
  pico_stdlib
)

pico_enable_stdio_usb(VL53LX_rangefinder 1)
pico_enable_stdio_uart(VL53LX_rangefinder 0)

pico_add_extra_outputs(VL53LX_rangefinder)

add_custom_target(Flash
    DEPENDS VL53LX_rangefinder
    COMMAND sudo picotool load -f ${PROJECT_BINARY_DIR}/VL53LX_rangefinder.uf2
)

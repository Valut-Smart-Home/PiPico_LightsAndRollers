cmake_minimum_required(VERSION 3.13)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(sample C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(FAMILY rp2040)
set(BOARD pico_sdk)

pico_sdk_init()

set(PICO_UART_ENABLE_CRLF_SUPPORT,0)

add_executable(lights_rollers main.c usb_descriptors.c)

target_include_directories(lights_rollers PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        $ENV{PICO_SDK_PATH}/lib/tinyusb/hw
        $ENV{PICO_SDK_PATH}/lib/tinyusb/src)

pico_add_extra_outputs(lights_rollers)

target_link_libraries(lights_rollers pico_stdlib hardware_pwm hardware_uart hardware_i2c tinyusb_device tinyusb_board)
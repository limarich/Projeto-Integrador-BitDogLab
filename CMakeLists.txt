cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico_w CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(integracao-perifericos-rp2040 C CXX ASM)

pico_sdk_init()

add_executable(${PROJECT_NAME}  integracao-perifericos-rp2040.c libs/leds.c libs/buzzer.c libs/ssd1306.c)

# configura PIO 
pico_generate_pio_header(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/pio_matrix.pio)

pico_enable_stdio_uart(${PROJECT_NAME}   1)
pico_enable_stdio_usb(${PROJECT_NAME}   1)

target_link_libraries(${PROJECT_NAME}  
        pico_stdlib
        hardware_clocks
        hardware_pio
        hardware_adc
        hardware_pwm
        hardware_i2c
        pico_time
)

target_link_libraries(${PROJECT_NAME}   
        
)

pico_add_extra_outputs(${PROJECT_NAME})


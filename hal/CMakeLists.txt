cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(MCU_MODEL STM32F302x8)
set(CPU_PARAMETERS
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    --specs=nosys.specs)

set(LIB_NAME STM32_HAL)

set(HAL_INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/Legacy
)

add_library(${LIB_NAME} STATIC
    src/stm32f3xx_hal.c
    src/stm32f3xx_hal_cortex.c
    src/stm32f3xx_hal_rcc.c
    src/stm32f3xx_hal_rcc_ex.c
    src/stm32f3xx_hal_gpio.c
    src/stm32f3xx_hal_adc.c
    src/stm32f3xx_hal_adc_ex.c
    src/stm32f3xx_hal_dma.c
    src/stm32f3xx_hal_uart.c
    src/stm32f3xx_hal_uart_ex.c
)

target_include_directories(${LIB_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/Legacy
    ${CMAKE_CURRENT_SOURCE_DIR}/../sys/inc
)

target_compile_options(${LIB_NAME} PRIVATE
    ${CPU_PARAMETERS}
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wdouble-promotion
    -Wformat=2 -Wformat-truncation
    -fno-common
    -Wno-unused-parameter
    -Wno-unused-variable
    -Wno-undef
    $<$<CONFIG:Debug>:-Og -g3 -ggdb>
    $<$<CONFIG:Release>:-Og -g3>
)

target_compile_definitions(${LIB_NAME} PRIVATE
    # USE_HAL_TIM_REGISTER_CALLBACKS=0
    # USE_HAL_ADC_REGISTER_CALLBACKS=0
    # USE_HAL_UART_REGISTER_CALLBACKS=0
    # USE_RTOS=0
    ${MCU_MODEL}
    $<$<CONFIG:Debug>:DEBUG>
)

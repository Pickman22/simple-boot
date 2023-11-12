cmake_minimum_required(VERSION 3.16)
project(simple-boot LANGUAGES C ASM)

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

set(STARTUP_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/../sys/src/startup_stm32f302x8.s)
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/linker/stm32f30_flash.ld)
set(EXECUTABLE ${CMAKE_PROJECT_NAME})
set(SYS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../sys/inc)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../hal ../../hal/build)

add_executable(${EXECUTABLE} 
    ../sys/src/stm32f3xx_it.c
    ../sys/src/system_stm32f3xx.c
    ../sys/src/startup_stm32f302x8.s
    fw/src/main.c
    fw/src/mcu.c
    fw/src/uart.c
    fw/src/gpio.c
    fw/src/printf.c
    fw/src/flash.c
)

target_link_libraries(${EXECUTABLE} STM32_HAL)

target_compile_options(${EXECUTABLE} PRIVATE
    ${CPU_PARAMETERS}
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wdouble-promotion
    -Wformat=2 -Wformat-truncation
    -Wundef
    -fno-common
    -Wno-unused-parameter
    $<$<CONFIG:Debug>:-Og -g3 -ggdb>
    $<$<CONFIG:Release>:-Og -g3>
)

target_compile_definitions(${EXECUTABLE} PRIVATE
    USE_HAL_TIM_REGISTER_CALLBACKS=0
    USE_HAL_ADC_REGISTER_CALLBACKS=0
    USE_HAL_UART_REGISTER_CALLBACKS=0
    USE_RTOS=0
    STATIC_FUNC=static
    ${MCU_MODEL}
    # USE_FULL_ASSERT
    $<$<CONFIG:Debug>:DEBUG>
)

target_link_options(${EXECUTABLE} PRIVATE
    ${CPU_PARAMETERS}
    LINKER:--gc-sections
    -T${LINKER_SCRIPT}
    LINKER:-Map,${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE}.map
    LINKER:--print-memory-usage
    --specs=nosys.specs
    -lc
)

set_target_properties(${EXECUTABLE} PROPERTIES
    SUFFIX .elf
)

target_include_directories(${EXECUTABLE} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../hal/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../hal/inc/Legacy
    ${CMAKE_CURRENT_SOURCE_DIR}/../sys/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/fw/inc
)

add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${EXECUTABLE}>
)

add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${EXECUTABLE}>
        ${EXECUTABLE}.hex
)

add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${EXECUTABLE}>
        ${EXECUTABLE}.bin
)

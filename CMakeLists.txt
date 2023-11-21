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

set(COMMON_COMPILE_OPTS
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
set(HAL_USE_DEFINES
    USE_HAL_TIM_REGISTER_CALLBACKS=0
    USE_HAL_ADC_REGISTER_CALLBACKS=0
    USE_HAL_UART_REGISTER_CALLBACKS=0
    USE_RTOS=0
)

set(COMMON_LINK_OPTS
    LINKER:--gc-sections
    LINKER:--print-memory-usage
    --specs=nosys.specs
    -lc
)

set(APP_EXE simple-boot-app)
set(BOOT_EXE simple-boot-boot)

add_subdirectory(hal)
add_subdirectory(sys)

add_executable(${APP_EXE} 
    appl/fw/src/main.c
    appl/fw/src/mcu.c
    appl/fw/src/uart.c
    appl/fw/src/gpio.c
    appl/fw/src/printf.c
    appl/fw/src/flash.c
)

add_executable(${BOOT_EXE} 
    boot/fw/src/main.c
    boot/fw/src/mcu.c
    boot/fw/src/uart.c
    boot/fw/src/gpio.c
    boot/fw/src/printf.c
    boot/fw/src/flash.c
)

target_link_libraries(${APP_EXE}
    STM32_HAL
    $<TARGET_OBJECTS:stm32_sys>
)

target_link_libraries(${BOOT_EXE}
    STM32_HAL
    $<TARGET_OBJECTS:stm32_sys>
)

target_compile_options(${APP_EXE} PRIVATE
    ${CPU_PARAMETERS}
    ${COMMON_COMPILE_OPTS}
)

target_compile_options(${BOOT_EXE} PRIVATE
    ${CPU_PARAMETERS}
    ${COMMON_COMPILE_OPTS}
    $<$<CONFIG:Debug>:-Og -g3 -ggdb>
    $<$<CONFIG:Release>:-Og -g3>
)

target_compile_definitions(${APP_EXE} PRIVATE
    ${MCU_MODEL}
    ${HAL_USE_DEFINES}
    STATIC_FUNC=static
    $<$<CONFIG:Debug>:DEBUG>
)

target_compile_definitions(${BOOT_EXE} PRIVATE
    ${MCU_MODEL}
    ${HAL_USE_DEFINES}
    STATIC_FUNC=static
    $<$<CONFIG:Debug>:DEBUG>
)

target_link_options(${APP_EXE} PRIVATE
    ${CPU_PARAMETERS}
    ${COMMON_LINK_OPTS}
    -T${CMAKE_CURRENT_SOURCE_DIR}/appl/linker/stm32f30_flash.ld
    LINKER:-Map,${CMAKE_CURRENT_BINARY_DIR}/${APP_EXE}.map
)

target_link_options(${BOOT_EXE} PRIVATE
    ${CPU_PARAMETERS}
    ${COMMON_LINK_OPTS}
    -T${CMAKE_CURRENT_SOURCE_DIR}/boot/linker/stm32f30_flash.ld
    LINKER:-Map,${CMAKE_CURRENT_BINARY_DIR}/${BOOT_EXE}.map
)

set_target_properties(${APP_EXE} PROPERTIES
    SUFFIX .elf
)

set_target_properties(${BOOT_EXE} PROPERTIES
    SUFFIX .elf
)

target_include_directories(${APP_EXE} PRIVATE
    ./appl/fw/inc
)

target_include_directories(${BOOT_EXE} PRIVATE
    ./boot/fw/inc
)

add_custom_command(TARGET ${APP_EXE} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${APP_EXE}>
)

add_custom_command(TARGET ${APP_EXE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${APP_EXE}>
        ${APP_EXE}.hex
)

add_custom_command(TARGET ${APP_EXE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${APP_EXE}>
        ${APP_EXE}.bin
)

add_custom_command(TARGET ${BOOT_EXE} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${BOOT_EXE}>
)

add_custom_command(TARGET ${BOOT_EXE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${BOOT_EXE}>
        ${BOOT_EXE}.hex
)

add_custom_command(TARGET ${BOOT_EXE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${BOOT_EXE}>
        ${BOOT_EXE}.bin
)

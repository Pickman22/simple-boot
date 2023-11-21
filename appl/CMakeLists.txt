add_executable(${APP_EXE} 
    fw/src/main.c
    fw/src/mcu.c
    fw/src/uart.c
    fw/src/gpio.c
    fw/src/printf.c
    fw/src/flash.c)

target_link_libraries(${APP_EXE}
    STM32_HAL
    $<TARGET_OBJECTS:stm32_sys>)

target_compile_options(${APP_EXE} PRIVATE
    ${CPU_PARAMETERS}
    ${COMMON_COMPILE_OPTS})

target_compile_definitions(${APP_EXE} PRIVATE
    ${MCU_MODEL}
    ${HAL_USE_DEFINES}
    STATIC_FUNC=static
    $<$<CONFIG:Debug>:DEBUG>)

target_link_options(${APP_EXE} PRIVATE
    ${CPU_PARAMETERS}
    ${COMMON_LINK_OPTS}
    -T${CMAKE_CURRENT_SOURCE_DIR}/linker/stm32f30_flash.ld
    LINKER:-Map,${CMAKE_CURRENT_BINARY_DIR}/${APP_EXE}.map)

set_target_properties(${APP_EXE} PROPERTIES
    SUFFIX .elf)

set_target_properties(${APP_EXE} PROPERTIES
    ADDITIONAL_CLEAN_FILES
    "${APP_EXE}.map;${APP_EXE}.bin;${APP_EXE}.hex")

target_include_directories(${APP_EXE} PRIVATE
    fw/inc)

add_custom_command(TARGET ${APP_EXE} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${APP_EXE}>
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${APP_EXE}> ${APP_EXE}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${APP_EXE}> ${APP_EXE}.bin)

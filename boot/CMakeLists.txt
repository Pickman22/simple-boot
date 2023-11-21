add_executable(${BOOT_EXE} 
    fw/src/main.c
    fw/src/mcu.c
    fw/src/uart.c
    fw/src/gpio.c
    fw/src/printf.c
    fw/src/flash.c)

target_link_libraries(${BOOT_EXE}
    STM32_HAL
    $<TARGET_OBJECTS:stm32_sys>)

target_compile_options(${BOOT_EXE} PRIVATE
    ${CPU_PARAMETERS}
    ${COMMON_COMPILE_OPTS})

target_compile_definitions(${BOOT_EXE} PRIVATE
    ${MCU_MODEL}
    STATIC_FUNC=static
    $<$<CONFIG:Debug>:DEBUG>)

target_link_options(${BOOT_EXE} PRIVATE
    ${CPU_PARAMETERS}
    ${COMMON_LINK_OPTS}
    -T${CMAKE_CURRENT_SOURCE_DIR}/linker/stm32f30_flash.ld
    LINKER:-Map,${CMAKE_CURRENT_BINARY_DIR}/${BOOT_EXE}.map)

target_include_directories(${BOOT_EXE} PRIVATE
    fw/inc)

generate_output_binaries(${BOOT_EXE})
clean_output_binaries(${BOOT_EXE})

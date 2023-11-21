set(SYS_LIB_NAME stm32_sys)

add_library(${SYS_LIB_NAME} OBJECT
    ./src/stm32f3xx_it.c
    ./src/system_stm32f3xx.c
    ./src/startup_stm32f302x8.s
)

target_include_directories(${SYS_LIB_NAME} PUBLIC
    ./inc
)

target_compile_options(${SYS_LIB_NAME} PRIVATE
    ${CPU_PARAMETERS}
    ${COMMON_COMPILE_OPTS}
)

target_compile_definitions(${SYS_LIB_NAME} PRIVATE
    ${MCU_MODEL}
)

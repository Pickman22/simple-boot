set(STM32_HAL_LIB_NAME STM32_HAL)

set(HAL_INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/Legacy)

add_library(${STM32_HAL_LIB_NAME} STATIC
    src/stm32f3xx_hal.c
    src/stm32f3xx_hal_cortex.c
    src/stm32f3xx_hal_rcc.c
    src/stm32f3xx_hal_rcc_ex.c
    src/stm32f3xx_hal_gpio.c
    src/stm32f3xx_hal_adc.c
    src/stm32f3xx_hal_adc_ex.c
    src/stm32f3xx_hal_dma.c
    src/stm32f3xx_hal_uart.c
    src/stm32f3xx_hal_uart_ex.c)

target_include_directories(${STM32_HAL_LIB_NAME} PUBLIC
    ./inc
    ./inc/Legacy
    ./../sys/inc)

target_compile_options(${STM32_HAL_LIB_NAME} PRIVATE
    ${CPU_PARAMETERS}
    ${COMMON_COMPILE_OPTS}
    -Wundef
    -fno-common
    -Wno-unused-parameter
)

target_compile_definitions(${STM32_HAL_LIB_NAME} PUBLIC
    USE_HAL_TIM_REGISTER_CALLBACKS=0
    USE_HAL_ADC_REGISTER_CALLBACKS=0
    USE_HAL_UART_REGISTER_CALLBACKS=0
    USE_RTOS=0
    ${MCU_MODEL})

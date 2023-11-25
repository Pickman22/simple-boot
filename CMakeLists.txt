cmake_minimum_required(VERSION 3.16)
project(simple-boot LANGUAGES C ASM)
include(compiler.cmake)
include(common.cmake)

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
    -mfloat-abi=hard)

set(COMMON_COMPILE_OPTS
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wdouble-promotion
    -Wformat=2
    -Wformat-truncation
    -ffunction-sections
    -fdata-sections
    -fno-math-errno
    -fsingle-precision-constant
    -Og)

set(COMMON_LINK_OPTS
    LINKER:--gc-sections
    LINKER:--print-memory-usage
    --specs=nano.specs
    --specs=nosys.specs)

set(APP_EXE simple-boot-app)
set(BOOT_EXE simple-boot-boot)

add_subdirectory(hal)
add_subdirectory(sys)
add_subdirectory(appl)
add_subdirectory(boot)

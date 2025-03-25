#CMakeLists.txt
# Create a CodeBlocks project:
# cmake . -G "CodeBlocks - Unix Makefiles"
# sudo openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000" -c "program build/baremetal.elf verify reset exit"


cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
set(PICO_SDK_PATH "/home/jss/projects/software/pico/pico-sdk")
#set(PICO_EXTRAS_PATH "/home/jss/projects/software/pico/pico-extras")
set(PICO_BOARD pico_w)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)
#include(${PICO_EXTRAS_PATH}/external/pico_extras_import.cmake)

cmake_minimum_required(VERSION 3.12)
set(this baremetal)

#variable_watch(this)
#variable_watch(source)

project(${this} C CXX ASM)

message("Building project ${PROJECT_NAME}")

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

set(src)
add_subdirectory("src")

# Create target
add_executable(
  ${this}
  ./src/main.c
  ${source}
)

message ("linking with ${CMAKE_CURRENT_LIST_DIR} and ${source} directories")
target_include_directories(${this} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

# Add pico_stdlib library which aggregates commonly used features
# Add pico_w arch
if (PICO_CYW43_SUPPORTED)
    message("Linking ${PROJECT_NAME} with PICO_CYW43_SUPPORTED")
    target_link_libraries(${this} pico_stdlib pico_cyw43_arch_lwip_threadsafe_background pico_lwip_http)
else()
    target_link_libraries(${this} ${source} pico_stdlib)
endif()


# transfer Cmake variables to include file "src/config.h"
# config.h.in is a project file. config.h is dynamically created
configure_file(
    ${CMAKE_SOURCE_DIR}/src/config.h.in
    ${CMAKE_SOURCE_DIR}/src/config.h
)

pico_enable_stdio_usb(${this} 0)
pico_enable_stdio_uart(${this} 1)
target_compile_options(${this} PRIVATE -DPARAM_ASSERTIONS_ENABLE_ALL=1)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${this})


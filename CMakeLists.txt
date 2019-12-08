cmake_minimum_required(VERSION 3.2)

set(CMAKE_C_OUTPUT_EXTENSION ".rel")
set(CMAKE_C_COMPILER /usr/local/bin/sdcc)
set(CMAKE_SYSTEM_NAME Generic) # No linux target etc

include_directories("STM8S_StdPeriph_Driver/inc" "inc")

# aux_source_directory("STM8S_StdPeriph_Driver/src" spd_src)

# Prevent default configuration
set(CMAKE_C_FLAGS_INIT "")
set(CMAKE_EXE_LINKER_FLAGS_INIT "")
project(STM8Blink C)
SET(CMAKE_C_FLAGS "-mstm8 --std-c99")
add_definitions(-DSDCC -DSTM8S003)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "dist")
add_executable(main src/main.c)

# set_target_properties(main 
#     RUNTIME_OUTPUT_DIRECTORY "dist"
# )
# Flash targets
add_custom_target(flash ALL COMMAND stm8flash -c stlinkv2 -p stm8s003f3 -w dist/main.ihx DEPENDS main)

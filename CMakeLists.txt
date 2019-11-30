cmake_minimum_required(VERSION 3.2)

set(CMAKE_C_OUTPUT_EXTENSION ".rel")
set(CMAKE_C_COMPILER /usr/local/sdcc/bin/sdcc)
set(CMAKE_SYSTEM_NAME Generic) # No linux target etc

include_directories("STM8S_StdPeriph_Driver/inc" ".")

# aux_source_directory("STM8S_StdPeriph_Driver/src" spd_src)

# Prevent default configuration
set(CMAKE_C_FLAGS_INIT "")
set(CMAKE_EXE_LINKER_FLAGS_INIT "")
project(STM8Blink C)
SET(CMAKE_C_FLAGS "-mstm8 --std-c99")
add_definitions(-DSDCC -DSTM8S003)
add_executable(main main.c stm8s_it.c STM8S_StdPeriph_Driver/src/stm8s_adc1.c STM8S_StdPeriph_Driver/src/stm8s_beep.c STM8S_StdPeriph_Driver/src/stm8s_exti.c STM8S_StdPeriph_Driver/src/stm8s_itc.c)
# Flash targets
add_custom_target(flash ALL COMMAND stm8flash -c stlink -p stm8s105c6 -w main.ihx)

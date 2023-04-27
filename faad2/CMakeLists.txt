cmake_minimum_required(VERSION 3.4.1)
project(faad2)

set(FAAD2_OPTION -DHAVE_CONFIG_H)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/libfaad
                    ${CMAKE_CURRENT_SOURCE_DIR}/libfaad/codebook)

file(GLOB libfaad_source libfaad/*.c)
add_library(faad2 SHARED ${libfaad_source})
target_compile_definitions(faad2 PRIVATE ${FAAD2_OPTION})
# the minimum version of CMake.
cmake_minimum_required(VERSION 3.4.1)
project(libsvm)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(svm SHARED svm.cpp)

file(
    GLOB_RECURSE mains RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/svm-scale.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/svm-train.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/svm-predict.c"
)

foreach(mainfile IN LISTS mains)
    get_filename_component(mainname ${mainfile} NAME_WE)
    add_executable(${mainname} ${mainfile})
    target_link_libraries(${mainname} svm)
endforeach()

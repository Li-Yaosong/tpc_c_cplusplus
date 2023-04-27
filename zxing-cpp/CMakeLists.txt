cmake_minimum_required(VERSION 3.4.1)
project(zxing_cpp)
set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/core
                    ${CMAKE_CURRENT_SOURCE_DIR}/core/src)

file(GLOB libzxing core/src/*.cpp)
file(GLOB libzxing_qrcode core/src/qrcode/*.cpp)
file(GLOB libzxing_pdf417 core/src/pdf417/*.cpp)
file(GLOB libzxing_oned core/src/oned/*.cpp)
file(GLOB libzxing_maxicode core/src/maxicode/*.cpp)
file(GLOB libzxing_libzueci core/src/libzueci/*.c)
file(GLOB libzxing_datamatrix core/src/datamatrix/*.cpp)
file(GLOB libzxing_aztec core/src/aztec/*.cpp)
file(GLOB libzxing_c core/src/*.c)
add_library(zxing SHARED ${libzxing} ${libzxing_qrcode} ${libzxing_pdf417}
            ${libzxing_oned} ${libzxing_maxicode} ${libzxing_libzueci}
            ${libzxing_datamatrix} ${libzxing_aztec} ${libzxing_c} )


file(
    GLOB_RECURSE mains RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/example/ZXingReader.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/example/ZXingWriter.cpp"
)
foreach(mainfile IN LISTS mains)
    get_filename_component(mainname ${mainfile} NAME_WE)
    add_executable(${mainname} ${mainfile})
target_link_libraries(${mainname} PUBLIC zxing)
endforeach()




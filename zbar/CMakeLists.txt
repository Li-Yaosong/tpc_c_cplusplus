# the minimum version of CMake.
cmake_minimum_required(VERSION 3.4.1)
project(ZBar)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/zbar
                    ${CMAKE_CURRENT_SOURCE_DIR}/third_party_libjpeg
                    ${CMAKE_CURRENT_SOURCE_DIR}/third_party_libpng
                    )

add_subdirectory(third_party_libjpeg)
add_subdirectory(third_party_libpng)

add_library(ZBar SHARED
                  zbar/jpeg.c
                  zbar/img_scanner.c
                  zbar/decoder.c
                  zbar/image.c
                  zbar/symbol.c
                  zbar/convert.c
                  zbar/config.c
                  zbar/scanner.c
                  zbar/error.c
                  zbar/refcnt.c
                  zbar/video.c
                  zbar/video/null.c
                  zbar/decoder/code128.c
                  zbar/decoder/code39.c
                  zbar/decoder/ean.c
                  zbar/decoder/i25.c
                  zbar/decoder/qr_finder.c
                  zbar/decoder/pdf417.c
                  zbar/qrcode/bch15_5.c
                  zbar/qrcode/binarize.c
                  zbar/qrcode/isaac.c
                  zbar/qrcode/qrdec.c
                  zbar/qrcode/qrdectxt.c
                  zbar/qrcode/rs.c
                  zbar/qrcode/util.c
                 )

add_library(libjpegStatic STATIC IMPORTED)
set_target_properties(libjpegStatic PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/third_party_libjpeg/libs/${OHOS_ARCH}/libjpegStatic.a)

add_library(libpngStatic STATIC IMPORTED)
set_target_properties(libpngStatic PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/third_party_libpng/libs/${OHOS_ARCH}/libpngStatic.a)


target_link_libraries(ZBar PUBLIC libjpegStatic)
target_link_libraries(ZBar PUBLIC libpngStatic)
target_link_libraries(ZBar PUBLIC libz.so)

add_executable(scan_image examples/scan_image.c)
target_link_libraries(scan_image PUBLIC ZBar)
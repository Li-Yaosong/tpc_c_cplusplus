# the minimum version of CMake.
cmake_minimum_required(VERSION 3.4.1)
project(minizip-ng)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../libiconv/ libiconv_binary_dir)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../openssl openssl_binary_dir)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../bzip2 bzip2_binary_dir)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../zstd zstd_binary_dir)

set(MINIZIP_SOURCE_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}/../libiconv/libiconv/
        ${CMAKE_CURRENT_SOURCE_DIR}/../libiconv/libiconv/lib/
        ${CMAKE_CURRENT_SOURCE_DIR}/../libiconv/libiconv/libcharset/
        ${CMAKE_CURRENT_SOURCE_DIR}/../openssl/openssl/
        ${CMAKE_CURRENT_SOURCE_DIR}/../openssl/openssl/crypto/
        ${CMAKE_CURRENT_SOURCE_DIR}/../openssl/openssl/include/
        ${CMAKE_CURRENT_SOURCE_DIR}/../openssl/openssl/include/openssl/
        ${CMAKE_CURRENT_SOURCE_DIR}/../openssl/openssl/crypto/modes/
        ${CMAKE_CURRENT_SOURCE_DIR}/../openssl/openssl/crypto/ec/curve448/
        ${CMAKE_CURRENT_SOURCE_DIR}/../openssl/openssl/crypto/ec/curve448/arch_32/
        ${CMAKE_CURRENT_SOURCE_DIR}/../bzip2/bzip2/
        ${CMAKE_CURRENT_SOURCE_DIR}/../zlib/zlib/
        ${CMAKE_CURRENT_SOURCE_DIR}/../xz/xz/
        ${CMAKE_CURRENT_SOURCE_DIR}/../xz/xz/src/liblzma/api/
        ${CMAKE_CURRENT_SOURCE_DIR}/../xz/xz/src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../xz/xz/dos/
        ${CMAKE_CURRENT_SOURCE_DIR}/../xz/xz/src/lz/
        ${CMAKE_CURRENT_SOURCE_DIR}/../xz/xz/src/lzma/
        ${CMAKE_CURRENT_SOURCE_DIR}/../xz/xz/src/common/
        ${CMAKE_CURRENT_SOURCE_DIR}/../xz/xz/src/liblzma/
        ${CMAKE_CURRENT_SOURCE_DIR}/../xz/xz/src/liblzma/common/
        ${CMAKE_CURRENT_SOURCE_DIR}/../xz/xz/src/liblzma/check/
        ${CMAKE_CURRENT_SOURCE_DIR}/../zstd/zstd/
        ${CMAKE_CURRENT_SOURCE_DIR}/../zstd/zstd/lib/
        ${CMAKE_CURRENT_SOURCE_DIR}/minizip-ng/
        )

set(MINIZIP_DEFINITIONS
        -DHAVE_BZIP2
        -DHAVE_INTTYPES_H
        -DHAVE_PKCRYPT
        -DHAVE_STDINT_H
        -DHAVE_WZAES
        -DHAVE_ZLIB
        -DHAVE_ZSTD
        -DHAVE_LZMA
        -DMZ_ZIP_SIGNING
        -DHAVE_ICONV
        -DLZMA_API_STATIC
        -D_POSIX_C_SOURCE=200112L
        -DOPENSSL_ARM64_PLATFORM
        -D_GNU_SOURCE
        )

set(MINIZIP_SOURCE
      "${CMAKE_CURRENT_SOURCE_DIR}/minizip-ng/mz_compat.c"
      "${CMAKE_CURRENT_SOURCE_DIR}/minizip-ng/mz_crypt.c"
      "${CMAKE_CURRENT_SOURCE_DIR}/minizip-ng/mz_crypt_openssl.c"
      "${CMAKE_CURRENT_SOURCE_DIR}/minizip-ng/mz_os.c"
      "${CMAKE_CURRENT_SOURCE_DIR}/minizip-ng/mz_os_posix.c"
      "${CMAKE_CURRENT_SOURCE_DIR}/minizip-ng/mz_strm.c"
      "${CMAKE_CURRENT_SOURCE_DIR}/minizip-ng/mz_strm_buf.c"
      "${CMAKE_CURRENT_SOURCE_DIR}/minizip-ng/mz_strm_bzip.c"
      "${CMAKE_CURRENT_SOURCE_DIR}/minizip-ng/mz_strm_mem.c"
      "${CMAKE_CURRENT_SOURCE_DIR}/minizip-ng/mz_strm_os_posix.c"
      "${CMAKE_CURRENT_SOURCE_DIR}/minizip-ng/mz_strm_pkcrypt.c"
      "${CMAKE_CURRENT_SOURCE_DIR}/minizip-ng/mz_strm_split.c"
      "${CMAKE_CURRENT_SOURCE_DIR}/minizip-ng/mz_strm_wzaes.c"
      "${CMAKE_CURRENT_SOURCE_DIR}/minizip-ng/mz_strm_zlib.c"
      "${CMAKE_CURRENT_SOURCE_DIR}/minizip-ng/mz_zip.c"
      "${CMAKE_CURRENT_SOURCE_DIR}/minizip-ng/mz_zip_rw.c"
      "${CMAKE_CURRENT_SOURCE_DIR}/minizip-ng/mz_strm_lzma.c"
      "${CMAKE_CURRENT_SOURCE_DIR}/minizip-ng/mz_strm_zstd.c")

set(MINIZIP_FLAG -fPIC
                 -Wno-error=implicit-function-declaration
                 -Wall
                 -Wextra
                 -Wno-error=missing-braces
                 -Wno-missing-braces
                 -Wno-error=visibility
                 -Wno-visibility
                 -Wno-error=unused-function
                 -Wno-unused-function
                 -Wno-error=unused-variable
                 -Wno-unused-variable
                 -Wno-error=undef
                 -Wno-error=deprecated-declarations
                 -Wno-deprecated-declarations
                 -Wno-error=sign-compare
                 -Wno-error=parentheses-equality
                 -Wno-parentheses-equality
                 -Wno-incompatible-pointer-types
                 -Wno-unused-command-line-argument
                 -Wno-error=unused-parameter
                 -Wno-unused-parameter
                 -Wno-error=header-hygiene
                 -frtti
                 -fexceptions
                 -std=gnu99)

add_library(minizip_shared SHARED ${MINIZIP_SOURCE})
target_compile_options(minizip_shared PRIVATE ${MINIZIP_FLAG})
target_compile_definitions(minizip_shared PRIVATE ${MINIZIP_DEFINITIONS})
target_include_directories(minizip_shared PRIVATE ${MINIZIP_SOURCE_DIR})

target_link_libraries(minizip_shared bzip2)
target_link_libraries(minizip_shared iconv)
target_link_libraries(minizip_shared zstd)
target_link_libraries(minizip_shared openssl)
target_link_libraries(minizip_shared z)
target_link_libraries(minizip_shared xz)
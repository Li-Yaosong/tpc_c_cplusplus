cmake_minimum_required (VERSION 3.12)

project(LIBZIP VERSION 0.1.0)

enable_language(C)

option(ENABLE_OPENSSL "Enable use of OpenSSL" ON)

option(ENABLE_BZIP2 "Enable use of BZip2" ON)
option(ENABLE_LZMA "Enable use of LZMA" ON)
option(ENABLE_ZSTD "Enable use of Zstandard" ON)

option(ENABLE_FDOPEN "Enable zip_fdopen, which is not allowed in Microsoft CRT secure libraries" ON)

option(BUILD_TOOLS "Build tools in the src directory (zipcmp, zipmerge, ziptool)" ON)
option(BUILD_REGRESS "Build regression tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)

set(TARGET_NAME zip)
set(TARGET_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libzip)
set(TARGET_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/adapted/include)

set(TARGET_INCLUDE ${TARGET_LIB_PATH})
set(TARGET_INSTALL_INCLUDEDIR include)
set(TARGET_INSTALL_BINDIR bin)
set(TARGET_INSTALL_LIBDIR lib)

set(TARGET_LIB   ${TARGET_LIB_PATH}/lib/zip_add.c
                 ${TARGET_LIB_PATH}/lib/zip_add_dir.c
                 ${TARGET_LIB_PATH}/lib/zip_add_entry.c
                 ${TARGET_LIB_PATH}/lib/zip_algorithm_deflate.c
                 ${TARGET_LIB_PATH}/lib/zip_buffer.c
                 ${TARGET_LIB_PATH}/lib/zip_close.c
                 ${TARGET_LIB_PATH}/lib/zip_delete.c
                 ${TARGET_LIB_PATH}/lib/zip_dir_add.c
                 ${TARGET_LIB_PATH}/lib/zip_dirent.c
                 ${TARGET_LIB_PATH}/lib/zip_discard.c
                 ${TARGET_LIB_PATH}/lib/zip_entry.c
                 ${TARGET_LIB_PATH}/lib/zip_error.c
                 ${TARGET_LIB_PATH}/lib/zip_error_clear.c
                 ${TARGET_LIB_PATH}/lib/zip_error_get.c
                 ${TARGET_LIB_PATH}/lib/zip_error_get_sys_type.c
                 ${TARGET_LIB_PATH}/lib/zip_error_strerror.c
                 ${TARGET_LIB_PATH}/lib/zip_error_to_str.c
                 ${TARGET_LIB_PATH}/lib/zip_extra_field.c
                 ${TARGET_LIB_PATH}/lib/zip_extra_field_api.c
                 ${TARGET_LIB_PATH}/lib/zip_fclose.c
                 ${TARGET_LIB_PATH}/lib/zip_fdopen.c
                 ${TARGET_LIB_PATH}/lib/zip_file_add.c
                 ${TARGET_LIB_PATH}/lib/zip_file_error_clear.c
                 ${TARGET_LIB_PATH}/lib/zip_file_error_get.c
                 ${TARGET_LIB_PATH}/lib/zip_file_get_comment.c
                 ${TARGET_LIB_PATH}/lib/zip_file_get_external_attributes.c
                 ${TARGET_LIB_PATH}/lib/zip_file_get_offset.c
                 ${TARGET_LIB_PATH}/lib/zip_file_rename.c
                 ${TARGET_LIB_PATH}/lib/zip_file_replace.c
                 ${TARGET_LIB_PATH}/lib/zip_file_set_comment.c
                 ${TARGET_LIB_PATH}/lib/zip_file_set_encryption.c
                 ${TARGET_LIB_PATH}/lib/zip_file_set_external_attributes.c
                 ${TARGET_LIB_PATH}/lib/zip_file_set_mtime.c
                 ${TARGET_LIB_PATH}/lib/zip_file_strerror.c
                 ${TARGET_LIB_PATH}/lib/zip_fopen.c
                 ${TARGET_LIB_PATH}/lib/zip_fopen_encrypted.c
                 ${TARGET_LIB_PATH}/lib/zip_fopen_index.c
                 ${TARGET_LIB_PATH}/lib/zip_fopen_index_encrypted.c
                 ${TARGET_LIB_PATH}/lib/zip_fread.c
                 ${TARGET_LIB_PATH}/lib/zip_fseek.c
                 ${TARGET_LIB_PATH}/lib/zip_ftell.c
                 ${TARGET_LIB_PATH}/lib/zip_get_archive_comment.c
                 ${TARGET_LIB_PATH}/lib/zip_get_archive_flag.c
                 ${TARGET_LIB_PATH}/lib/zip_get_encryption_implementation.c
                 ${TARGET_LIB_PATH}/lib/zip_get_file_comment.c
                 ${TARGET_LIB_PATH}/lib/zip_get_name.c
                 ${TARGET_LIB_PATH}/lib/zip_get_num_entries.c
                 ${TARGET_LIB_PATH}/lib/zip_get_num_files.c
                 ${TARGET_LIB_PATH}/lib/zip_hash.c
                 ${TARGET_LIB_PATH}/lib/zip_io_util.c
                 ${TARGET_LIB_PATH}/lib/zip_libzip_version.c
                 ${TARGET_LIB_PATH}/lib/zip_memdup.c
                 ${TARGET_LIB_PATH}/lib/zip_name_locate.c
                 ${TARGET_LIB_PATH}/lib/zip_new.c
                 ${TARGET_LIB_PATH}/lib/zip_open.c
                 ${TARGET_LIB_PATH}/lib/zip_pkware.c
                 ${TARGET_LIB_PATH}/lib/zip_progress.c
                 ${TARGET_LIB_PATH}/lib/zip_rename.c
                 ${TARGET_LIB_PATH}/lib/zip_replace.c
                 ${TARGET_LIB_PATH}/lib/zip_set_archive_comment.c
                 ${TARGET_LIB_PATH}/lib/zip_set_archive_flag.c
                 ${TARGET_LIB_PATH}/lib/zip_set_default_password.c
                 ${TARGET_LIB_PATH}/lib/zip_set_file_comment.c
                 ${TARGET_LIB_PATH}/lib/zip_set_file_compression.c
                 ${TARGET_LIB_PATH}/lib/zip_set_name.c
                 ${TARGET_LIB_PATH}/lib/zip_source_accept_empty.c
                 ${TARGET_LIB_PATH}/lib/zip_source_begin_write.c
                 ${TARGET_LIB_PATH}/lib/zip_source_begin_write_cloning.c
                 ${TARGET_LIB_PATH}/lib/zip_source_buffer.c
                 ${TARGET_LIB_PATH}/lib/zip_source_call.c
                 ${TARGET_LIB_PATH}/lib/zip_source_close.c
                 ${TARGET_LIB_PATH}/lib/zip_source_commit_write.c
                 ${TARGET_LIB_PATH}/lib/zip_source_compress.c
                 ${TARGET_LIB_PATH}/lib/zip_source_crc.c
                 ${TARGET_LIB_PATH}/lib/zip_source_error.c
                 ${TARGET_LIB_PATH}/lib/zip_source_file_common.c
                 ${TARGET_LIB_PATH}/lib/zip_source_file_stdio.c
                 ${TARGET_LIB_PATH}/lib/zip_source_free.c
                 ${TARGET_LIB_PATH}/lib/zip_source_function.c
                 ${TARGET_LIB_PATH}/lib/zip_source_get_file_attributes.c
                 ${TARGET_LIB_PATH}/lib/zip_source_is_deleted.c
                 ${TARGET_LIB_PATH}/lib/zip_source_layered.c
                 ${TARGET_LIB_PATH}/lib/zip_source_open.c
                 ${TARGET_LIB_PATH}/lib/zip_source_pass_to_lower_layer.c
                 ${TARGET_LIB_PATH}/lib/zip_source_pkware_decode.c
                 ${TARGET_LIB_PATH}/lib/zip_source_pkware_encode.c
                 ${TARGET_LIB_PATH}/lib/zip_source_read.c
                 ${TARGET_LIB_PATH}/lib/zip_source_remove.c
                 ${TARGET_LIB_PATH}/lib/zip_source_rollback_write.c
                 ${TARGET_LIB_PATH}/lib/zip_source_seek.c
                 ${TARGET_LIB_PATH}/lib/zip_source_seek_write.c
                 ${TARGET_LIB_PATH}/lib/zip_source_stat.c
                 ${TARGET_LIB_PATH}/lib/zip_source_supports.c
                 ${TARGET_LIB_PATH}/lib/zip_source_tell.c
                 ${TARGET_LIB_PATH}/lib/zip_source_tell_write.c
                 ${TARGET_LIB_PATH}/lib/zip_source_window.c
                 ${TARGET_LIB_PATH}/lib/zip_source_write.c
                 ${TARGET_LIB_PATH}/lib/zip_source_zip.c
                 ${TARGET_LIB_PATH}/lib/zip_source_zip_new.c
                 ${TARGET_LIB_PATH}/lib/zip_stat.c
                 ${TARGET_LIB_PATH}/lib/zip_stat_index.c
                 ${TARGET_LIB_PATH}/lib/zip_stat_init.c
                 ${TARGET_LIB_PATH}/lib/zip_strerror.c
                 ${TARGET_LIB_PATH}/lib/zip_string.c
                 ${TARGET_LIB_PATH}/lib/zip_unchange.c
                 ${TARGET_LIB_PATH}/lib/zip_unchange_all.c
                 ${TARGET_LIB_PATH}/lib/zip_unchange_archive.c
                 ${TARGET_LIB_PATH}/lib/zip_unchange_data.c
                 ${TARGET_LIB_PATH}/lib/zip_utf-8.c
                 ${TARGET_LIB_PATH}/lib/zip_source_file_stdio_named.c
                 ${TARGET_LIB_PATH}/lib/zip_random_unix.c
                 ${CMAKE_CURRENT_SOURCE_DIR}/adapted/lib/zip_err_str.c)

set(DEFINES -Dzip_EXPORTS
            -fPIC)

add_library(${TARGET_NAME} ${TARGET_LIB})

if(ENABLE_BZIP2)
  target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../bzip2)
  set(HAVE_LIBBZ2 1)
  target_sources(${TARGET_NAME} PRIVATE ${TARGET_LIB_PATH}/lib/zip_algorithm_bzip2.c)
  target_link_libraries(${TARGET_NAME} PRIVATE bzip2)
endif()

if(ENABLE_LZMA)
  target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../xz)
  set(HAVE_LIBLZMA 1)
  target_sources(${TARGET_NAME} PRIVATE ${TARGET_LIB_PATH}/lib/zip_algorithm_xz.c)
  target_link_libraries(${TARGET_NAME} PRIVATE liblzma)
endif()

if(ENABLE_LZMA)
  target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../zstd)
  set(HAVE_LIBZSTD 1)
  target_sources(${TARGET_NAME} PRIVATE ${TARGET_LIB_PATH}/lib/zip_algorithm_zstd.c)
  target_link_libraries(${TARGET_NAME} PRIVATE zstd)
endif()

if(ENABLE_OPENSSL)
  target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../openssl
                                                  ${CMAKE_CURRENT_SOURCE_DIR}/../openssl/openssl/include
                                                  ${CMAKE_CURRENT_SOURCE_DIR}/../openssl/adapted/include)
  set(HAVE_CRYPTO 1)
  set(HAVE_OPENSSL 1)
  target_sources(${TARGET_NAME} PRIVATE ${TARGET_LIB_PATH}/lib/zip_crypto_openssl.c
                                      ${TARGET_LIB_PATH}/lib/zip_winzip_aes.c
                                      ${TARGET_LIB_PATH}/lib/zip_source_winzip_aes_decode.c
                                      ${TARGET_LIB_PATH}/lib/zip_source_winzip_aes_encode.c)
  target_link_libraries(${TARGET_NAME} PRIVATE crypto)
endif()

target_link_libraries(zip PRIVATE z)
target_include_directories(${TARGET_NAME} PUBLIC ${TARGET_INCLUDE_PATH}
                                                 ${TARGET_LIB_PATH}/examples
                                                 ${TARGET_LIB_PATH}/src
                                                 ${TARGET_LIB_PATH}/lib
                                                 ${TARGET_LIB_PATH}/regress)
target_compile_options(${TARGET_NAME} PUBLIC ${DEFINES})

if(BUILD_REGRESS)
set(TEST_PROGRAMS
  add_from_filep
  can_clone_file
  fopen_unchanged
  fseek
  fuzz_main
  nonrandomopentest
  liboverride-test
)

set(GETOPT_USERS
  fread
  tryopen
)

set(HOLE_USERS
  hole
  ziptool_regress
)
set(ZIP_PROGRAMS ${TEST_PROGRAMS} ${GETOPT_USERS} ${HOLE_USERS})

foreach(PROGRAM ${ZIP_PROGRAMS})
    add_executable(${PROGRAM} ${TARGET_LIB_PATH}/regress/${PROGRAM}.c)
    target_link_libraries(${PROGRAM} PUBLIC ${TARGET_NAME})
    target_include_directories(${PROGRAM} PRIVATE BEFORE ${TARGET_LIB_PATH}/lib ${PROJECT_BINARY_DIR} ${TARGET_INCLUDE_PATH})
endforeach()

foreach(PROGRAM ${GETOPT_USERS} ${HOLE_USERS})
    target_sources(${PROGRAM} PRIVATE ${TARGET_LIB_PATH}/src/getopt.c)
    target_include_directories(${PROGRAM} PRIVATE BEFORE ${TARGET_LIB_PATH}/src)
endforeach()
foreach(PROGRAM ${HOLE_USERS})
    target_sources(${PROGRAM} PRIVATE ${TARGET_LIB_PATH}/regress/source_hole.c)
endforeach()
target_include_directories(ziptool_regress PRIVATE BEFORE ${TARGET_LIB_PATH}/src)
set(DL_USERS
  nonrandomopen
  liboverride
)

foreach(PROGRAM ${DL_USERS})
  add_library(${PROGRAM} MODULE ${TARGET_LIB_PATH}/regress/${PROGRAM}.c)
  target_include_directories(${PROGRAM} PRIVATE BEFORE ${TARGET_LIB_PATH}/lib ${PROJECT_BINARY_DIR} ${TARGET_INCLUDE_PATH})
endforeach()
endif()

if(BUILD_EXAMPLES)
foreach(PROGRAM add-compressed-data autoclose-archive in-memory)
  add_executable(${PROGRAM} ${TARGET_LIB_PATH}/examples/${PROGRAM}.c)
  target_link_libraries(${PROGRAM} ${TARGET_NAME})
  target_include_directories(${PROGRAM} PRIVATE BEFORE ${PROJECT_SOURCE_DIR}/lib ${PROJECT_BINARY_DIR})
endforeach()
endif()

if(BUILD_TOOLS)
foreach(PROGRAM zipcmp zipmerge ziptool)
    add_executable(${PROGRAM} ${TARGET_LIB_PATH}/src/${PROGRAM}.c)
    target_link_libraries(${PROGRAM} PRIVATE ${TARGET_NAME})
    target_include_directories(${PROGRAM} PRIVATE BEFORE ${PROJECT_SOURCE_DIR}/lib ${PROJECT_BINARY_DIR})
    target_sources(${PROGRAM} PRIVATE ${TARGET_LIB_PATH}/src/getopt.c)
endforeach()
target_sources(zipcmp PRIVATE ${TARGET_LIB_PATH}/src/diff_output.c)
target_link_libraries(zipcmp PRIVATE z)
endif()
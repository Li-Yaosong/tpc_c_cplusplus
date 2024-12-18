
if(NOT IMAGEKNIFEC_ROOT_PATH)
	message(FATAL_ERROR "-- [IMAGEKNIFEC]: IMAGEKNIFEC_ROOT_PATH must be setted up at first.")
endif()

set(IMAGEKNIFEC_SEARCH_PATH ${IMAGEKNIFEC_ROOT_PATH})
set(CMAKE_CXX_STANDARD 17)
# find base path
find_path(IMAGEKNIFEC_PATH
    NAMES include/ImageKnife.h
    PATHS ${IMAGEKNIFEC_SEARCH_PATH}
    CMAKE_FIND_ROOT_PATH_BOTH
    )

# find includes
if(NOT IMAGEKNIFEC_INCLUDE_DIRS)
    find_path(IMAGEKNIFEC_INCLUDE_DIR
        NAMES Imageknife.h
        PATHS ${IMAGEKNIFEC_PATH}
        PATH_SUFFIXES include
        CMAKE_FIND_ROOT_PATH_BOTH
    )

    set(IMAGEKNIFEC_INCLUDE_DIRS
        ${IMAGEKNIFEC_INCLUDE_DIR})
endif()

# find link directories
if(NOT IMAGEKNIFEC_LINK_DIRS)
    find_path(IMAGEKNIFEC_LIB_LINK_DIRS
        NAMES libimageknifec.so
        PATHS "${IMAGEKNIFEC_PATH}"
        PATH_SUFFIXES "libs/${CMAKE_OHOS_ARCH_ABI}"
        CMAKE_FIND_ROOT_PATH_BOTH
    )

    set(IMAGEKNIFEC_LINK_DIRS
        ${IMAGEKNIFEC_LIB_LINK_DIRS})

endif()

# find librarys
if(NOT IMAGEKNIFEC_LIBRARIS)
    find_library(IMAGEKNIFEC_LIBRARY
        NAMES imageknifec
        PATHS "${IMAGEKNIFEC_PATH}"
        PATH_SUFFIXES "libs/${CMAKE_OHOS_ARCH_ABI}"
        CMAKE_FIND_ROOT_PATH_BOTH
    )

    set(IMAGEKNIFEC_LIBRARIS
        imageknifec
        ace_napi.z
        ace_ndk.z
        rcp_c
        image_source
        pixelmap
        rawfile.z
        ohresmgr
        hilog_ndk.z
        ohcrypto)

endif()

# 编译宏
if(NOT IMAGEKNIFEC_DEFINITIONS)
    if(OHOS)
        set(IMAGEKNIFEC_DEFINITIONS ${IMAGEKNIFEC_DEFINITIONS} "IMAGEKNIFEC_ENABLE_NAPI=1")
    endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(IMAGEKNIFEC DEFAULT_MSG
    IMAGEKNIFEC_INCLUDE_DIRS
    IMAGEKNIFEC_LIBRARIS
)

if(IMAGEKNIFEC_FOUND AND NOT TARGET Imageknifec::libimageknifec)
    # 创建接口lib
    add_library(Imageknifec::libimageknifec INTERFACE IMPORTED)

    # 设置target 头文件依赖
    target_include_directories(Imageknifec::libimageknifec INTERFACE ${IMAGEKNIFEC_INCLUDE_DIRS})

    # 设置target link依赖路径
    target_link_directories(Imageknifec::libimageknifec INTERFACE ${IMAGEKNIFEC_LINK_DIRS})

    # 设置target lib依赖
    target_link_libraries(Imageknifec::libimageknifec INTERFACE ${IMAGEKNIFEC_LIBRARIS})

    # 设置target 编译宏
    target_compile_definitions(Imageknifec::libimageknifec INTERFACE ${IMAGEKNIFEC_DEFINITIONS})

    # 安全编译选项 NO Rpath 规则
    set(CMAKE_SKIP_RPATH TRUE)
endif()

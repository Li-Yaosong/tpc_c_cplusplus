cmake_minimum_required(VERSION 3.4.1)
project(third_party_libpng)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libs/${OHOS_ARCH})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(pngStatic STATIC
      arm/arm_init.c
      arm/filter_neon_intrinsics.c
      arm/palette_neon_intrinsics.c
      png.c
      pngerror.c
      pngget.c
      pngmem.c
      pngpread.c
      pngread.c
      pngrio.c
      pngrtran.c
      pngrutil.c
      pngset.c
      pngtrans.c
      pngwio.c
      pngwrite.c
      pngwtran.c
      pngwutil.c)



# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)
message("CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}")
set(SNAPPY_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../third_party/snappy)
message("SNAPPY_SRC ${SNAPPY_SRC}")

get_filename_component(SNAPPY_SRC "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../third_party/snappy" ABSOLUTE)
message("SNAPPY_SRC: " ${SNAPPY_SRC})

message("CMAKE_SOURCE_DIR: " ${CMAKE_SOURCE_DIR})

# 设置静态库文件目录
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../output/snappy/jniLibs/${ANDROID_ABI})

# 动态库文件目录
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../output/snappy/jniLibs/${ANDROID_ABI})

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

set(Snappy_Library_SRCS
        # Provides a relative path to your source file(s).
        ${SNAPPY_SRC}/snappy.cc
        ${SNAPPY_SRC}/snappy.h
        ${SNAPPY_SRC}/snappy-c.cc
        ${SNAPPY_SRC}/snappy-c.h
        ${SNAPPY_SRC}/snappy-internal.h
        ${SNAPPY_SRC}/snappy-sinksource.cc
        ${SNAPPY_SRC}/snappy-sinksource.h
        ${SNAPPY_SRC}/snappy-stubs-internal.cc
        ${SNAPPY_SRC}/snappy-stubs-internal.h
        ${SNAPPY_SRC}/snappy-stubs-public.h
        ${SNAPPY_SRC}/snappy_compress_fuzzer.cc
        ${SNAPPY_SRC}/snappy_uncompress_fuzzer.cc
        )
message("CMAKE_LIBRARY_OUTPUT_DIRECTORY: " ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
add_library( # Sets the name of the library.
        snappy
        STATIC
        ${Snappy_Library_SRCS}
        )
message("ARCHIVE_OUTPUT_DIRECTORY: " ${ARCHIVE_OUTPUT_DIRECTORY})
message("LIBRARY_OUTPUT_PATH: " ${LIBRARY_OUTPUT_PATH})
message("CMAKE_CURRENT_BINARY_DIR: " ${CMAKE_CURRENT_BINARY_DIR})
message("CMAKE_INSTALL_PREFIX: " ${CMAKE_INSTALL_PREFIX})

execute_process(
        COMMAND
        ${CMAKE_COMMAND}
        -E
        copy
        ${CMAKE_CURRENT_BINARY_DIR}/libsnappy.a
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../output/snappy/jniLibs/${ANDROID_ABI}/libsnappy.a)

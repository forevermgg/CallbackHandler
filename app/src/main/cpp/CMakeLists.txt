# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)
include(CheckEnv.cmake)
include(Paths.cmake)
project("callbackhandler" C CXX)
# RE2 needs Pthreads on non-WIN32
find_package(Threads)

include_directories(common)
include_directories(log)
include_directories(platform/android)
include_directories(memory)

set(FLATBUFFERS_SRC_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/../../../../third_party/flatbuffers/include)
message("FLATBUFFERS_SRC_INCLUDE ${FLATBUFFERS_SRC_INCLUDE}")
add_subdirectory(flatbuffers)
include_directories(${FLATBUFFERS_SRC_INCLUDE})
FILE(GLOB Generated_SRCS generated/*.h)

set(LEVELDB_SRC_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/../../../../third_party/leveldb/include)
message("LEVELDB_SRC_INCLUDE ${LEVELDB_SRC_INCLUDE}")
add_subdirectory(leveldb)
include_directories(${LEVELDB_SRC_INCLUDE})

add_subdirectory(snappy)

# https://github.com/google/minimalloc/blob/main/CMakeLists.txt
set(EXTERNAL_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../../../../external/)
message("EXTERNAL_SRC = ${EXTERNAL_SRC}/abseil-cpp")
add_subdirectory(${EXTERNAL_SRC}/abseil-cpp ${EXTERNAL_SRC}/abseil-cpp/build)

include(cmake/FindAtomics.cmake)

add_library(${CMAKE_PROJECT_NAME} SHARED
        # List C/C++ source files with relative paths to this CMakeLists.txt.
        AutoBitmap.cc
        common/macros.h
        debug.h
        debug.cpp
        downcast.h
        compiler.h
        VirtualMachineEnv.h
        VirtualMachineEnv.cpp
        common/CallbackHandler.h
        common/CallbackHandler.cpp
        common/CallbackUtils.h
        common/CallbackUtils.cpp
        load_library.cpp
        platform/android/thread_local.h
        platform/android/thread_local.cc
        platform/android/jni_android.h
        platform/android/jni_android.cc
        generate.h
        linker_jni.h
        linker_jni.cc
        log/log.h
        log/log.cc
        log/log_assert.h
        log/log_level.h
        log/log_settings.h
        log/log_settings.cc
        log/log_settings_state.cc
        log/logging.h
        log/logging.cc
        log/log_android.cc
        log/log_android_callback.cc
        memory/mutex.h
        memory/mutex_pthread.cc
        memory/singleton.h
        memory/Condition.h
        memory/Invocable.h
        platform/android/embedded_file.h
        platform/android/jni_weak_ref.h
        platform/android/jni_weak_ref.cc
        platform/android/scoped_java_ref.h
        platform/android/scoped_java_ref.cc
        platform/android/string_conversion.h
        platform/android/string_conversion.cc
        platform/android/util_android.h
        platform/android/util_android.cc
        thread/ThreadUtils.h
        thread/ThreadUtils.cpp
        common/NioUtils.h
        common/NioUtils.cpp
        Test.cpp
        Test.h
        test_jni.cc
        native-lib.cpp
        ${Generated_SRCS})

add_library("chromium_android_linker" SHARED
        # List C/C++ source files with relative paths to this CMakeLists.txt.
        linker_jni_onload.cc
        linker_jni.h
        linker_jni.cc)

target_link_libraries(${CMAKE_PROJECT_NAME}
        # List libraries link to the target library
        android
        flatbuffers
        snappy
        leveldb
        absl::flags_parse
        absl::flags
        absl::statusor
        log)
if(ANDROID)
  message("Forevermeng")
endif()
target_link_libraries("chromium_android_linker"
        # List libraries link to the target library
        android
        log)

# Hack the memmove bug on Samsung device.
if (ARMEABI OR ARMEABI_V7A)
  message("Forevermeng ARMEABI OR ARMEABI_V7A")
else()
  message("Forevermeng NO ARMEABI OR ARMEABI_V7A")
endif()

# Set default build type.
if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to 'RelWithDebInfo' as none was specified.")
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING
          "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
          FORCE)
else()
  message("CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
endif()

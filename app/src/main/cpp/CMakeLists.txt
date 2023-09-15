# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)
#[[message("CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}")
message("CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}")
message("CMAKE_CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR}")
message("CMAKE_CURRENT_LIST_FILE ${CMAKE_CURRENT_LIST_FILE}")
message("CMAKE_CURRENT_LIST_LINE ${CMAKE_CURRENT_LIST_LINE}")]]
# Declares the project name. The project name can be accessed via ${ PROJECT_NAME},
# Since this is the top level CMakeLists.txt, the project name is also accessible
# with ${CMAKE_PROJECT_NAME} (both CMake variables are in-sync within the top level
# build script scope).
project("callbackhandler")

include_directories(common)
include_directories(log)
include_directories(platform/android)
include_directories(memory)
set(FLATBUFFERS_SRC_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/../../../../third_party/flatbuffers/include)
message("FLATBUFFERS_SRC_INCLUDE ${FLATBUFFERS_SRC_INCLUDE}")
set(LEVELDB_SRC_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/../../../../third_party/leveldb/include)
message("LEVELDB_SRC_INCLUDE ${LEVELDB_SRC_INCLUDE}")
add_subdirectory(flatbuffers)
add_subdirectory(leveldb)
add_subdirectory(snappy)
include_directories(${FLATBUFFERS_SRC_INCLUDE})
include_directories(${LEVELDB_SRC_INCLUDE})

FILE(GLOB Generated_SRCS generated/*.h)
# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
#
# In this top level CMakeLists.txt, ${CMAKE_PROJECT_NAME} is used to define
# the target library name; in the sub-module's CMakeLists.txt, ${PROJECT_NAME}
# is preferred for the same purpose.
#
# In order to load a library into your app from Java/Kotlin, you must call
# System.loadLibrary() and pass the name of the library defined here;
# for GameActivity/NativeActivity derived applications, the same library name must be
# used in the AndroidManifest.xml file.
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

# Specifies libraries CMake should link to your target library. You
# can link libraries from various origins, such as libraries defined in this
# build script, prebuilt third-party libraries, or Android system libraries.
target_link_libraries(${CMAKE_PROJECT_NAME}
        # List libraries link to the target library
        android
        flatbuffers
        leveldb
        log)

target_link_libraries("chromium_android_linker"
        # List libraries link to the target library
        android
        log)
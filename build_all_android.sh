#
# Ensure that ANDROID_NDK environment variable is set to your Android NDK location
# e.g. /Library/Android/sdk/ndk-bundle

#!/bin/bash

# Fail on any error.
set -e
# Display commands being run.
set -x

if [ -z "$ANDROID_NDK" ]; then
  echo "Please set ANDROID_NDK to the Android NDK folder"
  exit 1
fi

# Directories, paths and filenames
BUILD_DIR=build

CMAKE_ARGS="-S ./app/src/main/cpp \
  -DBUILD_SHARED_LIBS=true \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo \
  -DANDROID_TOOLCHAIN=clang \
  -DANDROID_STL=c++_shared \
  -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake \
  -DCMAKE_INSTALL_PREFIX=./app/src/main/cpp"


function build_CallbackHandler {

  ABI=$1
  MINIMUM_API_LEVEL=$2
  ABI_BUILD_DIR=build/${ABI}
  STAGING_DIR=staging

  echo "Building CallbackHandler for ${ABI}"
  echo "Building ABI_BUILD_DIR for ${ABI_BUILD_DIR}"
  echo "Building STAGING_DIR for ${STAGING_DIR}"
  mkdir -p ${ABI_BUILD_DIR} ${ABI_BUILD_DIR}/${STAGING_DIR}

  cmake -B${ABI_BUILD_DIR} \
        -DANDROID_ABI=${ABI} \
        -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY=${STAGING_DIR}/lib/${ABI} \
        -DANDROID_PLATFORM=android-${MINIMUM_API_LEVEL}\
        ${CMAKE_ARGS}

  pushd ${ABI_BUILD_DIR}
  make -j5
  popd
}

build_CallbackHandler armeabi-v7a 21
build_CallbackHandler arm64-v8a 21
build_CallbackHandler x86 21
build_CallbackHandler x86_64 21
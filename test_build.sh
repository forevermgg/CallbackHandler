#!/bin/bash

# Test that the prefab binary exists
if hash prefab 2>/dev/null; then
  echo "Prefab is installed"
else
  echo "Prefab binary not found. See https://github.com/google/prefab for install instructions"
  # exit 1
fi

# Get the version string from the source
major=$(grep "#define OBOE_VERSION_MAJOR" version/Version.h | cut -d' ' -f3)
minor=$(grep "#define OBOE_VERSION_MINOR" version/Version.h | cut -d' ' -f3)
patch=$(grep "#define OBOE_VERSION_PATCH" version/Version.h | cut -d' ' -f3)
version=$major"."$minor"."$patch

echo "Building libraries for Oboe version "$version
#./build_all_android.sh

ABIS=("x86" "x86_64" "arm64-v8a" "armeabi-v7a")

python3 utils/git-sync-deps.py
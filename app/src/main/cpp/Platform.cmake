# --- [ Determine target processor
IF(CMAKE_OSX_ARCHITECTURES)
    LIST(LENGTH CMAKE_OSX_ARCHITECTURES CMAKE_OSX_ARCHITECTURES_COUNT)
    IF(CMAKE_OSX_ARCHITECTURES_COUNT GREATER 1)
        MESSAGE(FATAL_ERROR "Unsupported build with multiple OSX architectures (${CMAKE_OSX_ARCHITECTURES}). "
                "Specify a single architecture in CMAKE_OSX_ARCHITECTURES and re-configure. ")
    ENDIF()
    IF(NOT CMAKE_OSX_ARCHITECTURES MATCHES "^(x86_64|arm64|arm64e|arm64_32)$")
        MESSAGE(FATAL_ERROR "Unrecognized CMAKE_OSX_ARCHITECTURES value \"${CMAKE_OSX_ARCHITECTURES}\"")
    ENDIF()
    SET(TARGET_PROCESSOR "${CMAKE_OSX_ARCHITECTURES}")
    ADD_COMPILE_OPTIONS("-Wno-shorten-64-to-32")
    message("CMAKE_OSX_ARCHITECTURES-TARGET_PROCESSOR = ${TARGET_PROCESSOR}")
ELSEIF(CMAKE_GENERATOR MATCHES "^Visual Studio " AND CMAKE_GENERATOR_PLATFORM)
    IF(CMAKE_GENERATOR_PLATFORM STREQUAL "Win32")
        SET(TARGET_PROCESSOR "x86")
    ELSEIF(CMAKE_GENERATOR_PLATFORM STREQUAL "x64")
        SET(TARGET_PROCESSOR "x86_64")
    ELSEIF(CMAKE_GENERATOR_PLATFORM STREQUAL "ARM64")
        SET(TARGET_PROCESSOR "arm64")
    ELSEIF(CMAKE_GENERATOR_PLATFORM STREQUAL "ARM64EC")
        SET(TARGET_PROCESSOR "arm64ec")
    ELSE()
        MESSAGE(FATAL_ERROR "Unsupported Visual Studio architecture \"${CMAKE_GENERATOR_PLATFORM}\"")
    ENDIF()
    message("CMAKE_GENERATOR-TARGET_PROCESSOR = ${TARGET_PROCESSOR}")
ELSEIF(CMAKE_SYSTEM_PROCESSOR MATCHES "^i[3-7]86$")
    SET(TARGET_PROCESSOR "x86")
    message("CMAKE_SYSTEM_PROCESSOR-TARGET_PROCESSOR-X6 = ${TARGET_PROCESSOR}")
ELSEIF(CMAKE_SYSTEM_PROCESSOR STREQUAL "AMD64")
    SET(TARGET_PROCESSOR "x86_64")
    message("CMAKE_SYSTEM_PROCESSOR-TARGET_PROCESSOR-AMD64 = ${TARGET_PROCESSOR}")
ELSEIF(CMAKE_SYSTEM_PROCESSOR MATCHES "^armv[5-8]")
    SET(TARGET_PROCESSOR "arm")
    message("CMAKE_SYSTEM_PROCESSOR-TARGET_PROCESSOR-ARM = ${TARGET_PROCESSOR}")
ELSEIF(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
    SET(TARGET_PROCESSOR "arm64")
    message("CMAKE_SYSTEM_PROCESSOR-TARGET_PROCESSOR-AARCH = ${TARGET_PROCESSOR}")
ELSEIF(NOT TARGET_PROCESSOR MATCHES "^(x86(_64)?|arm64|riscv(32|64|128)|Hexagon)$")
    SET(TARGET_PROCESSOR "${CMAKE_SYSTEM_PROCESSOR}")
    message("CMAKE_SYSTEM_PROCESSOR-TARGET_PROCESSOR = ${TARGET_PROCESSOR}")
    message("CMAKE_SYSTEM_PROCESSOR = ${CMAKE_SYSTEM_PROCESSOR}")
ELSE()
    MESSAGE(FATAL_ERROR "Unrecognized CMAKE_SYSTEM_PROCESSOR value \"${CMAKE_SYSTEM_PROCESSOR}\"")
ENDIF()

IF(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    # Disable "unary minus operator applied to unsigned type, result still unsigned" warning
    ADD_COMPILE_OPTIONS("/wd4146")
    # Test files have many sections, increase the limit. See
    # https://learn.microsoft.com/en-us/cpp/build/reference/bigobj-increase-number-of-sections-in-dot-obj-file.
    ADD_COMPILE_OPTIONS("/bigobj")
ENDIF()
message("CMAKE_C_COMPILER_ID = ${CMAKE_C_COMPILER_ID}")

IF(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # Disable "note: parameter passing for argument of type ... changed/will change in ..."
    ADD_COMPILE_OPTIONS("-Wno-psabi")
ENDIF()
message("CMAKE_CXX_COMPILER_ID = ${CMAKE_CXX_COMPILER_ID}")

# ---[ Build flags
IF(NOT CMAKE_SYSTEM_NAME)
    MESSAGE(FATAL_ERROR "CMAKE_SYSTEM_NAME not defined")
ELSEIF(NOT CMAKE_SYSTEM_NAME MATCHES "^(Android|Darwin|iOS|Linux|Windows|CYGWIN|MSYS|QURT)$")
    MESSAGE(FATAL_ERROR "Unrecognized CMAKE_SYSTEM_NAME value \"${CMAKE_SYSTEM_NAME}\"")
ENDIF()
IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
    # Disable min/max macros as they break std::min/max and std::numeric_limits<T>min/max.
    ADD_COMPILE_DEFINITIONS("NOMINMAX")
ENDIF()
message("CMAKE_SYSTEM_NAME = ${CMAKE_SYSTEM_NAME}")

# ---[ Download deps


# ---[ Configure cpuinfo
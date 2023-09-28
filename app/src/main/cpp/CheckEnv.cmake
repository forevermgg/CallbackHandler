# Require at least C++17
if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
endif()
if(${CMAKE_CXX_STANDARD} LESS 17)
    message(FATAL_ERROR "Requires C++17 or later, but is configured for C++${CMAKE_CXX_STANDARD})")
endif()

# find javah
find_package(Java COMPONENTS Development)
if (NOT Java_Development_FOUND)
    if (DEFINED ENV{JAVA_HOME} AND EXISTS "$ENV{JAVA_HOME}/bin/javah")
        set(Java_JAVAH_EXECUTABLE "$ENV{JAVA_HOME}/bin/javah")
    elseif (EXISTS "/usr/bin/javah")
        set(Java_JAVAH_EXECUTABLE "/usr/bin/javah")
    else()
        message(FATAL_ERROR "Cannot find javah")
    endif()
endif()
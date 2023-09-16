### Platform
```cmake
# Determine the location of the libraries to use based on the platform.
if(ANDROID)
elseif(APPLE)
    if(IOS)
    else() #Assume MacOS
    endif()
elseif(MSVC)
else()  # LINUX
endif()
```
### CMAKE_HOST_WIN32
`CMAKE_HOST_WIN32`是`CMake`预定义的一个变量，用于判断当前的构建环境是否为Windows操作系统。

在`CMake`中，`CMAKE_HOST_WIN32`变量的值为`TRUE`表示当前构建环境为`Window`s操作系统，否则为`FALSE`。

这个变量通常用于条件判断，以便根据不同的操作系统采取相应的构建配置或行为。例如，可以使用以下方式来检查是否为`Windows`环境：

```cmake
if(CMAKE_HOST_WIN32)
# Windows平台相关配置
else()
# 非Windows平台相关配置
endif()
```        
需要注意的是，`CMAKE_HOST_WIN32`只能用于判断构建机器的操作系统类型，并不能判断生成的目标代码将在哪种操作系统上运行。要设置目标代码所针对的操作系统，请使用`CMAKE_SYSTEM_NAME`变量。

除了`CMAKE_HOST_WIN32`，`CMake`还提供了其他一些与系统平台相关的预定义变量，如`CMAKE_HOST_SYSTEM`、`CMAKE_SYSTEM_NAME`等，可用于进行更精细的操作系统判断和配置。
`QtCopyFileIfDifferent.cmake`
```cmake

cmake_minimum_required(VERSION 3.16)

set(copy_strategy "copy_if_different")
if(CMAKE_HOST_WIN32)
    file(SIZE "${SRC_FILE_PATH}" size)
    # If file size is bigger than 2GB copy it unconditionally
    if(size GREATER_EQUAL 2147483648)
        set(copy_strategy "copy")
    endif()
endif()

execute_process(COMMAND ${CMAKE_COMMAND} -E ${copy_strategy} "${SRC_FILE_PATH}" "${DST_FILE_PATH}")
```

```cmake
# copy_if_different works incorrect in Windows if file size if bigger than 2GB.
# See https://gitlab.kitware.com/cmake/cmake/-/issues/23052 and QTBUG-99491 for details.
function(copy_file_if_different_command out_var src_file dst_file)
    # The CMake version higher than 3.23 doesn't contain the issue
    if(CMAKE_HOST_WIN32 AND CMAKE_VERSION VERSION_LESS 3.23)
        set(${out_var} "${CMAKE_COMMAND}"
            "-DSRC_FILE_PATH=${src_file}"
            "-DDST_FILE_PATH=${dst_file}"
            -P "${_qt_6_config_cmake_dir}/QtCopyFileIfDifferent.cmake"
            PARENT_SCOPE
        )
    else()
        set(${out_var} "${CMAKE_COMMAND}"
            -E copy_if_different
            "${src_file}"
            "${dst_file}"
            PARENT_SCOPE
        )
    endif()
endfunction()
```
### CMAKE_SYSTEM_NAME
`CMAKE_SYSTEM_NAME`是一个`CMake`预定义的变量，用于表示当前操作系统的名称。

这个变量的值是根据`CMake`在配置过程中自动检测得到的，它可以告诉你正在构建的项目所运行的操作系统。例如，可能的取值有：

+ "Linux"：表示`Linux`操作系统。

+ "Windows"：表示`Windows`操作系统。
+ "Darwin"：表示`macOS`操作系统。
+ "Android"：表示`Android`操作系统。
+ "iOS"：表示`iOS`操作系统。
等等...
通过使用`CMAKE_SYSTEM_NAME`变量，你可以在`CMakeLists.txt`文件中根据不同的操作系统进行条件判断和设置。

下面是一个示例，在`CMakeLists.txt`文件中根据不同操作系统设置不同的编译选项：

```cmake
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
# Linux specific compilation options
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
# Windows specific compilation options
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
# macOS specific compilation options
elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
# Android specific compilation options
elseif(CMAKE_SYSTEM_NAME STREQUAL "iOS")
# iOS specific compilation options
endif()
```
在上述示例中，根据`CMAKE_SYSTEM_NAME`的不同取值，可以针对特定的操作系统设置相应的编译选项。

需要注意的是，`CMAKE_SYSTEM_NAME`是一个只读变量，由`CMake`自动检测并设置。在执行`CMake`构建过程之前，它将被正确配置。

```cmake
if(CMAKE_SYSTEM_NAME MATCHES "^Windows|Linux|Android")
    set(REALM_NEEDS_OPENSSL TRUE)
endif()
```
通过使用`CMAKE_SYSTEM_NAME`变量和正则表达式的匹配`（MATCHES）`判断条件，可以确定操作系统名称是否与指定的模式匹配。
在代码中，如果`CMAKE_SYSTEM_NAME`的值匹配正则表达式 "^Windows|Linux|Android"，即以 "Windows"、"Linux" 或 "Android" 开头，则执行下面的代码块。
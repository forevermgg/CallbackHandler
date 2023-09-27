# Suppress all warnings from third-party projects.
# 这段代码是一个CMake命令用于在构建过程中禁止来自第三方项目的警告信息。具体而言，它使用了set_property命令，并将编译选项-w添加到目录的属性中。
# COMPILE_OPTIONS是一个目录属性，它用于指定编译选项，可以通过set_property命令的APPEND PROPERTY选项来追加值。在这种情况下，-w表示禁止编译器产生任何警告信息。
# 这样做的目的是为了在构建过程中抑制第三方项目产生的警告，以减少构建日志中的噪音和干扰。然而，需要注意的是，禁用警告可能会隐藏一些潜在的问题，因此在实际开发中应该谨慎使用，并确保对代码进行适当的质量控制和测试。
set_property(DIRECTORY APPEND PROPERTY COMPILE_OPTIONS -w)

# 这段代码是一个CMake变量设置命令，用于指定第三方源代码的根目录。具体而言，它使用了set命令，并定义了名为EXTERNAL_ROOT_DIR的变量，值为${CMAKE_CURRENT_SOURCE_DIR}。
# CMAKE_CURRENT_SOURCE_DIR是CMake预定义的变量，表示当前处理的CMakeLists.txt文件所在的目录。
# 通过将${CMAKE_CURRENT_SOURCE_DIR}赋值给EXTERNAL_ROOT_DIR变量，即设置了一个替代的根目录，用于存放所有第三方项目的源代码。
# 这样做的目的是为了在构建过程中将第三方项目的源代码存放在指定的根目录下，以便更好地组织和管理这些代码。
# 需要注意的是，CACHE STRING选项是将该变量设置为可缓存的字符串类型，这意味着可以在命令行或其他方式中通过缓存机制修改该变量的值。
set(EXTERNAL_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE STRING
        "Root location of all third_party projects")

message("EXTERNAL_ROOT_DIR  =  ${EXTERNAL_ROOT_DIR}")
# Find abseil
# 这段代码用于在CMake中查找Abseil库的源代码目录。它首先检查是否定义了absl_SOURCE_DIR变量，如果定义了，则将其值赋给EFFCEE_ABSEIL_DIR变量，并通过FORCE选项强制缓存该变量的值。
# 如果没有定义absl_SOURCE_DIR变量，则会在预定义的第三方根目录${EXTERNAL_ROOT_DIR}下搜索Abseil库的源代码目录。这里使用了两种可能的命名约定，分别是abseil_cpp和abseil-cpp，并检查这两个目录是否存在。如果找到匹配的目录，则将其路径赋给EFFCEE_ABSEIL_DIR变量，并缓存该变量的值。
# 通过这段代码，可以在构建过程中自动查找Abseil库的源代码目录，并将其路径存储在EFFCEE_ABSEIL_DIR变量中，以便后续使用。
# 需要注意的是，CACHE STRING选项是将该变量设置为可缓存的字符串类型，使得该变量的值可以在命令行或其他方式中进行修改。
if(absl_SOURCE_DIR)
    set(EFFCEE_ABSEIL_DIR "${absl_SOURCE_DIR}" CACHE STRING "Location of abseil source" FORCE)
else()
    # Allow for abseil-cpp or abseil_cpp
    if (IS_DIRECTORY "${EXTERNAL_ROOT_DIR}/abseil_cpp")
        set(EFFCEE_ABSEIL_DIR "${EXTERNAL_ROOT_DIR}/abseil_cpp" CACHE STRING "Location of abseil source")
    elseif(IS_DIRECTORY "${EXTERNAL_ROOT_DIR}/abseil-cpp")
        set(EFFCEE_ABSEIL_DIR "${EXTERNAL_ROOT_DIR}/abseil-cpp" CACHE STRING "Location of abseil source")
    endif()
endif()

message("EFFCEE_ABSEIL_DIR  =  ${EFFCEE_ABSEIL_DIR}")
### target_sources
`target_sources()`是`CMake`中的一个命令，用于向指定的目标（例如库或可执行文件）添加源文件。

以下是`target_sources()`的使用示例：

```cmake
target_sources(target_name PRIVATE source1.cpp source2.cpp)
```
`target_name`表示要添加源文件的目标名称。可以是已经存在的目标，也可以是尚未创建的目标。
`PRIVATE`是一种属性修饰符，表示这些源文件仅对当前目标可见。其他依赖该目标的目标无法访问这些源文件。
`source1.cpp`和`source2.cpp`是要添加的源文件列表。
通过调用`target_sources()`命令，可以将源文件与特定目标关联起来。这些源文件将在构建过程中被编译并链接到目标中，以生成最终的输出文件（例如库文件或可执行文件）。

需要注意的是，`target_sources()`命令应在`add_library()`或`add_executable()`命令之后使用，以确保正确地将源文件与目标进行关联。同时，还可以使用其他命令（如`target_compile_options()`、`target_include_directories()`等）来进一步配置和管理目标。

### target_include_directories
`target_include_directories()`是`CMake`中的一个命令，用于为指定的目标（例如库或可执行文件）添加包含目录。

以下是`target_include_directories()`的使用示例：

```cmake
target_include_directories(target_name PUBLIC include_dir1 PRIVATE include_dir2)
```
+ `target_name`表示要添加包含目录的目标名称。可以是已经存在的目标，也可以是尚未创建的目标。
+ `PUBLIC`和`PRIVATE`是属性修饰符，用于指定包含目录的可见性。具有`PUBLIC`属性的包含目录将对当前目标及其依赖项可见，而具有`PRIVATE`属性的包含目录仅对当前目标可见。
+ `include_dir1`和`include_dir2`是要添加的包含目录路径列表。
+ 通过调用`target_include_directories()`命令，可以将包含目录与特定目标关联起来。这些包含目录将在编译过程中被设置为目标的包含路径，以供源文件中的`#include`指令使用。

PUBLIC属性的包含目录还会被传递给依赖该目标的其他目标，使它们可以访问相同的包含路径。而PRIVATE属性的包含目录只对当前目标可见，不会被传递给其他目标。

需要注意的是，`target_include_directories()`命令应在`add_library()`或`add_executable()`命令之后使用，以确保正确地将包含目录与目标进行关联。同时，还可以使用其他命令（如`target_sources()`、`target_compile_options()`等）来进一步配置和管理目标。

### target_compile_definitions
target_compile_definitions()是CMake中的一个命令，用于为指定的目标（例如库或可执行文件）添加编译定义。

以下是target_compile_definitions()的使用示例：

```cmake
target_compile_definitions(target_name PRIVATE DEFINITION1 DEFINITION2)
```
`target_name`表示要添加编译定义的目标名称。可以是已经存在的目标，也可以是尚未创建的目标。
`PRIVATE`是一种属性修饰符，表示这些编译定义仅对当前目标可见。其他依赖该目标的目标无法访问这些定义。
`DEFINITION1`和`DEFINITION2`是要添加的编译定义。
通过调用`target_compile_definitions()`命令，可以将编译定义与特定目标关联起来。这些定义将被加入到目标的编译过程中，作为预处理器定义传递给源文件。

这些定义可用于控制条件编译、启用或禁用特定功能、设置宏等。在源文件中，可以使用条件语句（如`#ifdef`、`#ifndef`）或直接使用定义的常量和宏。

需要注意的是，`target_compile_definitions()`命令应在`add_library()`或`add_executable()`命令之后使用，以确保正确地将编译定义与目标进行关联。同时，还可以使用其他命令（如`target_sources()`、`target_include_directories()`等）来进一步配置和管理目标。

### 以下是target_link_libraries
`target_link_libraries()`是`CMake`中的一个命令，用于为指定的目标（例如库或可执行文件）添加链接的库。

以下是`target_link_libraries()`的使用示例：

```cmake
target_link_libraries(target_name PUBLIC library1 PRIVATE library2)
```
`target_name`表示要添加链接库的目标名称。可以是已经存在的目标，也可以是尚未创建的目标。
`PUBLIC`和`PRIVATE`是属性修饰符，用于指定库的可见性。具有PUBLIC属性的库将对当前目标及其依赖项可见，而具有`PRIVATE`属性的库仅对当前目标可见。
`library1`和`library2`是要链接的库。
通过调用`target_link_libraries()`命令，可以将需要链接的库与特定目标关联起来。这些库将在链接过程中被连接到目标中，以解析目标代码中对这些库的函数和符号引用。

链接的库可以是静态库（`.a`文件）或动态库（`.so`、`.dll`等文件）。可以链接系统提供的标准库，也可以链接第三方库。

需要注意的是，`target_link_libraries()`命令应在`add_library()`或`add_executable()`命令之后使用，以确保正确地将库与目标进行关联。同时，还可以使用其他命令（如`target_sources()`、`target_include_directories()`等）来进一步配置和管理目标。


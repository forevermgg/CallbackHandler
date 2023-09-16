### CMAKE_CXX_STANDARD
`CMAKE_CXX_STANDARD`是一个`CMake`变量，用于指定所使用的 C++ 标准。

通过设置`CMAKE_CXX_STANDARD`变量的值，可以告诉`CMake` 采用特定的 C++ 标准进行编译。常见的取值包括 "98"、"11"、"14"、"17"、"20" 等，代表了不同的`C++`标准版本。

在`CMakeLists.txt`文件中，通常会使用类似如下的语句来设置`CMAKE_CXX_STANDARD`：

```cmake
set(CMAKE_CXX_STANDARD 11)
```
这样就将`C++`标准设置为`C++11`。

```
if(NOT CMAKE_CXX_STANDARD)
  # This project requires C++11.
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif(NOT CMAKE_CXX_STANDARD)
```

### CMAKE_CXX_STANDARD_REQUIRED
`CMAKE_CXX_STANDARD_REQUIRED`是一个`CMake`变量，用于指定是否要求编译器严格遵循所设置的`C++`标准。

当`CMAKE_CXX_STANDARD_REQUIRED`的值为`ON`时，意味着强制要求编译器使用`CMAKE_CXX_STANDARD`所指定的`C++`标准进行编译。如果编译器不支持该标准，则会产生错误。

当`CMAKE_CXX_STANDARD_REQUIRED`的值为`OFF`时，表示对编译器的`C++`标准版本没有强制要求。如果编译器不支持所设置的`C++`标准,`CMake`会尽力选择相近的兼容版本进行编译。

在`CMakeLists.txt`文件中，可以使用如下语句来设置 `CMAKE_CXX_STANDARD_REQUIRED`：

```cmake
set(CMAKE_CXX_STANDARD_REQUIRED ON)
```
这样就会要求编译器严格遵循所设置的`C++`标准。

### CMAKE_CXX_EXTENSIONS
`CMAKE_CXX_EXTENSIONS`是一个`CMake`变量，用于指定是否启用编译器的特定扩展。

当`CMAKE_CXX_EXTENSIONS`的值为`ON`时，表示启用编译器的特定扩展。这些扩展可能会导致代码在不同的编译器上产生不一致的行为或结果。常见的扩展包括`GNU`扩展和`Microsoft`扩展。

当`CMAKE_CXX_EXTENSIONS`的值为 OFF 时，表示禁用编译器的特定扩展。这样可以确保代码在不同的编译器上具有更好的可移植性和一致性。

在`CMakeLists.txt`文件中，可以使用如下语句来设置 `CMAKE_CXX_EXTENSIONS`：

```cmake
set(CMAKE_CXX_EXTENSIONS OFF)
```
这样就会禁用编译器的特定扩展。

### CMAKE_CXX_COMPILER_ID
`CMAKE_CXX_COMPILER_ID`是一个`CMake`变量，用于表示当前所使用的`C++`编译器的标识符。
该变量的值通常是根据所使用的编译器自动设置的，例如：`GCC`、`Clang`、`MSVC` 等。通过读取 `CMAKE_CXX_COMPILER_ID` 变量的值，可以在`CMake`构建过程中根据不同的编译器采取相应的操作或配置。

+ CMAKE_CXX_COMPILER_ID STREQUAL "MSVC"


```cmake
if (NOT "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    message(WARNING "Fuzzing builds are only supported with Clang")
  endif (NOT "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
```

### CMAKE_CXX_WARNING_LEVEL
`CMAKE_CXX_WARNING_LEVEL`是一个`CMake`变量，用于指定编译器的警告级别。

通过设置`CMAKE_CXX_WARNING_LEVEL`变量的值，可以控制编译器在编译过程中产生的警告信息的详细程度。不同的编译器和构建系统可能支持不同的警告级别选项。

在`CMakeLists.txt`文件中，可以使用如下语句来设置 `CMAKE_CXX_WARNING_LEVEL`：

```cmake
set(CMAKE_CXX_WARNING_LEVEL 3)
```
这样就将编译器的警告级别设置为 3。

### CMAKE_CXX_FLAGS
`CMAKE_CXX_FLAGS`是一个`CMake`变量，用于指定`C++`编译器的额外编译选项。

通过设置`CMAKE_CXX_FLAGS`变量的值，可以向编译器传递额外的选项，例如编译标志、优化级别、预处理宏定义等。这些选项可以影响编译过程和生成的目标文件的行为。
```cmake
  # Use -Wall for clang and gcc.
  if(NOT CMAKE_CXX_FLAGS MATCHES "-Wall")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  endif(NOT CMAKE_CXX_FLAGS MATCHES "-Wall")

  # Use -Wextra for clang and gcc.
  if(NOT CMAKE_CXX_FLAGS MATCHES "-Wextra")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
  endif(NOT CMAKE_CXX_FLAGS MATCHES "-Wextra")

  # Use -Werror for clang only.
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if(NOT CMAKE_CXX_FLAGS MATCHES "-Werror")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
    endif(NOT CMAKE_CXX_FLAGS MATCHES "-Werror")
  endif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")

  # Disable C++ exceptions.
  string(REGEX REPLACE "-fexceptions" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")

  # Disable RTTI.
  string(REGEX REPLACE "-frtti" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
```
以上代码段包含了多个条件语句和`set`命令。根据条件不同，对`CMAKE_CXX_FLAGS`变量进行不同的操作。

首先，通过条件判断，检查是否已经在`CMAKE_CXX_FLAGS`中包含了`-Wall`和`-Wextra`编译选项。如果没有，则将它们分别添加到`CMAKE_CXX_FLAGS`的末尾。

接着，通过条件判断，检查当前的编译器是否为`Clang`，并且在`CMAKE_CXX_FLAGS`中是否已经包含了`-Werror`编译选项。如果满足条件，则将 -Werror 添加到`CMAKE_CXX_FLAGS`的末尾。

然后，使用正则表达式替换的方式，从`CMAKE_CXX_FLAGS`中移除`-fexceptions`和 `-frtti`编译选项，并分别添加`-fno-exceptions`和`-fno-rtti`编译选项到 `CMAKE_CXX_FLAGS`的末尾。

因此，以上代码的作用是对`CMAKE_CXX_FLAGS`进行一系列条件判断和设置，以实现特定的编译选项配置。

```cmake
  if(NOT CMAKE_CXX_FLAGS MATCHES "-fsanitize=address")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
  endif(NOT CMAKE_CXX_FLAGS MATCHES "-fsanitize=address")

  if(NOT CMAKE_CXX_FLAGS MATCHES "-fsanitize=fuzzer-no-link")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=fuzzer-no-link")
  endif(NOT CMAKE_CXX_FLAGS MATCHES "-fsanitize=fuzzer-no-link")
```
这段代码的作用是检查`${CMAKE_CXX_FLAGS}`变量中是否包含`-fsanitize=address`和`-fsanitize=fuzzer-no-link`编译选项。如果不包含，则将这两个选项添加到`${CMAKE_CXX_FLAGS}`变量中。
首先，第一个条件语句检查`${CMAKE_CXX_FLAGS}`变量是否包含`-fsanitize=address`选项。如果不包含，意味着该选项尚未添加到`${CMAKE_CXX_FLAGS}`中。在这种情况下，使用`set()`命令将$`{CMAKE_CXX_FLAGS}`扩展为当前值加上`-fsanitize=address`选项。

接下来，第二个条件语句检查`${CMAKE_CXX_FLAGS}`变量是否包含`-fsanitize=fuzzer-no-link`选项。如果不包含，意味着该选项尚未添加到`${CMAKE_CXX_FLAGS}`中。在这种情况下，同样使用`set()`命令将$`{CMAKE_CXX_FLAGS}`扩展为当前值加上`-fsanitize=fuzzer-no-link`选项。

通过这两个条件语句，如果编译选项尚未包含在`${CMAKE_CXX_FLAGS}`变量中，将会将其添加进去。这可以确保在构建过程中正确地设置了所需的编译选项，以便实现相应的功能`（如地址检查和模糊测试)`

### CMAKE_EXPORT_COMPILE_COMMANDS
`CMAKE_EXPORT_COMPILE_COMMANDS`是一个`CMake`的变量，用于控制是否生成编译命令导出文件`（compile_commands.json）`。

当设置`CMAKE_EXPORT_COMPILE_COMMANDS`为`ON`时，`CMake`会在构建过程中生成一个名为`compile_commands.json`的文件，该文件记录了每个源文件的编译命令。这个文件可以被一些代码编辑器、静态分析工具和其他构建工具用来提供更好的代码补全、跳转和分析功能。

要启用编译命令导出，可以在`CMakeLists.txt`文件中添加以下行：

```cmake
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
```
或者，在`CMake`生成的构建系统的命令行中使用：

```bash
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON <path_to_source_directory>
```
需要注意的是，编译命令导出功能可能依赖于`CMake`版本和所使用的构建系统。某些较旧的`CMake`版本或非`Unix`样式的构建系统可能不支持此功能。

生成的`compile_commands.json`文件通常位于构建目录的根目录中，但也可以通过设置`CMAKE_EXPORT_COMPILE_COMMANDS`变量来自定义输出路径：

```cmake
set(CMAKE_EXPORT_COMPILE_COMMANDS "<output_path>/compile_commands.json")
```
请注意，如果`CMAKE_EXPORT_COMPILE_COMMANDS`未设置为ON，编译命令导出文件将不会生成。


### CMAKE_BUILD_TYPE
```cmake
if(CMAKE_BUILD_TYPE STREQUAL RelWithDebInfo)
    # From NDK 23, Android uses CMake "RelWithDebInfo" instead of "Release", but we create the JNI
    # headers from Gradle using the `debug`/`release` variant names, so rename
    set(buildTypeCap "Release")
endif()
```
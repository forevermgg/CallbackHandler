### CMAKE_SOURCE_DIR
`CMAKE_SOURCE_DIR`是一个`CMake`预定义的变量，表示当前`CMakeLists.txt`文件所在的源代码根目录的绝对路径。

这个变量的值是在`CMake`运行时自动设置的，用于指定源代码的根目录。它通常用于构建过程中生成文件的路径和其他相对于项目根目录的操作。

例如，如果你的项目结构如下：

- project
  |- CMakeLists.txt
  |- src
  |  |- main.cpp
  |- include
  |- header.h
  在`CMakeLists.txt`文件中，可以使用`CMAKE_SOURCE_DIR`来引用项目的根目录路径：

```cmake
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(myapp
${CMAKE_SOURCE_DIR}/src/main.cpp
)
```
这样就能够使用 `${CMAKE_SOURCE_DIR}`来构建正确的文件路径，确保项目中的源文件和资源能够被正确地引用和访问。

需要注意的是，在使用`CMAKE_SOURCE_DIR`之前，确保`CMakeLists.txt`文件已经被正确加载，并且 CMake 已经启动了配置和生成过程。否则，该变量可能不会被正确设置。

此外，除了`CMAKE_SOURCE_DIR`，还有其他一些预定义的`CMake`变量，如`CMAKE_BINARY_DIR`、`CMAKE_CURRENT_SOURCE_DIR`等，用于指定不同的路径和目录。你可以根据具体需要选择合适的变量来操作文件路径和目录。

### CMAKE_CURRENT_SOURCE_DIR
`CMAKE_CURRENT_SOURCE_DIR`是一个`CMake`预定义的变量，表示当前正在处理的`CMakeLists.txt`文件所在的源代码目录的绝对路径。

与`CMAKE_SOURCE_DIR`变量不同的是，`CMAKE_CURRENT_SOURCE_DIR`表示的是当前正在处理的`CMakeLists.txt`所在的目录，而不是整个项目的根目录。

这个变量的值是在`CMake`运行时自动设置的，在使用`CMake`构建过程中非常有用。它可以用于指定相对于当前`CMakeLists.txt`文件的路径，或者作为源文件、资源文件等的基准路径。

例如，如果你的项目结构如下：

- project
  |- CMakeLists.txt
  |- src
  |  |- main.cpp
  |- include
  |- header.h
  在`CMakeLists.txt`文件中，可以使用`CMAKE_CURRENT_SOURCE_DIR`来引用当前`CMakeLists.txt`文件所在目录的路径：

```cmake
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(myapp
${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)
```
这样就能够使用 `${CMAKE_CURRENT_SOURCE_DIR}`来构建相对于当前`CMakeLists.txt`文件的正确的文件路径。

需要注意的是，在使用`CMAKE_CURRENT_SOURCE_DIR`之前，确保`CMakeLists.txt`文件已经被正确加载，并且`CMake`已经启动了配置和生成过程。否则，该变量可能不会被正确设置。

此外，除了`CMAKE_CURRENT_SOURCE_DIR`，还有其他一些预定义的`CMake`变量，如`CMAKE_BINARY_DIR`、`CMAKE_CURRENT_BINARY_DIR`等，用于指定不同的路径和目录。你可以根据具体需要选择合适的变量来操作文件路径和目录。
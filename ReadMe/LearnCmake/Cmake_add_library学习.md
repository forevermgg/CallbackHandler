### add_library
`add_library`命令用于生成一个库。它的语法如下：
```cmake
add_library(<name> [STATIC | SHARED | MODULE]
[source1] [source2] [...])
```
其中，`name`是库的名称，可以是任何有效的标识符。`STATIC`、`SHARED`和`MODULE`是可选的参数，用于指定生成的库的类型。如果不指定这些参数，则默认生成一个静态库。`source1`、`source2`等是库的源文件，它们将被编译并链接成一个库。
`add_library`命令可以用于生成静态库、共享库和对象库。静态库是在编译时链接到可执行文件中的库，共享库是在运行时动态链接的库，对象库是一种特殊的库，它只包含目标文件，而不包含符号表和重定位信息。

### add_library(snappy "")
`add_library()`函数用于向`CMake`构建系统添加一个库。在这个例子中，我们添加了名为`snappy`的库。
这行代码中的空字符串 "" 是作为库源文件列表的占位符。可以将源文件列表填充到括号内，以指定实际的源文件。如果没有源文件列表，可以使用空字符串来创建一个空的库。
这将创建一个空的库目标，并将其命名为`snappy`。具体的源文件列表和编译选项可以在后续的代码中通过`target_sources()`和`target_compile_options()`等命令进行添加和配置。
请注意，这行代码只是创建了一个空的库目标，需要在后续的代码中添加源文件并进行相关的配置和链接操作，才能最终生成可用的库文件。

### add_library STATIC
```
add_library(utils STATIC IMPORTED)
set_target_properties(utils PROPERTIES IMPORTED_LOCATION
        ${FILAMENT_DIR}/lib/${ANDROID_ABI}/libutils.a)


add_library(archive STATIC archive.cpp zip.cpp lzma.cpp)        
```
### add_library SHARED
```
add_library(filament-jni SHARED
    src/main/cpp/*.cpp
    src/main/cpp/*.cc
    src/main/cpp/*.h
)
```
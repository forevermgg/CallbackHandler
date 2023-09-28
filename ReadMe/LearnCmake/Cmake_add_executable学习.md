### add_executable
`add_executable`命令用于生成一个可执行文件。它的语法如下：
```cmake
add_executable(<name> [source1] [source2] ...)
```
其中，`name`是可执行文件的名称，`source1`、`source2`等是可执行文件的源文件。可以指定多个源文件，它们将被编译并链接成一个可执行文件。例如：
```cmake
add_executable(my_program main.cpp helper.cpp)
```
这将生成一个名为`my_program`的可执行文件，它包含`main.cp`和`helper.cpp`两个源文件。
```cmake
# set minimum cmake version
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# project name and language
project(recipe-01 LANGUAGES C)

add_executable(hello-world hello-world.c)
```
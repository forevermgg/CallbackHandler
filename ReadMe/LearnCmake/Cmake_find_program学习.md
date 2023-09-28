### find_program()
`find_program()`是`CMake`中的一个命令，用于在系统路径中查找指定的可执行程序。
```cmake
find_program(FIREBASE_PYTHON_EXECUTABLE
  NAMES python3 python
  DOC "The Python interpreter to use, such as one from a venv"
  REQUIRED
)
```

### find javah
```cmake
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
```
### find_package

首先，定义CMake最低版本和项目名称。注意，这里不需要任何语言支持:
```cmake
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(recipe-01 LANGUAGES NONE)
```
然后，使用find_package命令找到Python解释器:
```cmake
find_package(PythonInterp REQUIRED)
```
然后，执行Python命令并捕获它的输出和返回值:
```cmake
execute_process(
COMMAND
${PYTHON_EXECUTABLE} "-c" "print('Hello, world!')"
RESULT_VARIABLE _status
OUTPUT_VARIABLE _hello_world
ERROR_QUIET
OUTPUT_STRIP_TRAILING_WHITESPACE
)
```
最后，打印Python命令的返回值和输出:
```cmake
message(STATUS "RESULT_VARIABLE is: ${_status}")
message(STATUS "OUTPUT_VARIABLE is: ${_hello_world}")
```

```cmake
find_package(PythonInterp REQUIRED)
find_package(PythonLibs ${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR} EXACT REQUIRED)
```
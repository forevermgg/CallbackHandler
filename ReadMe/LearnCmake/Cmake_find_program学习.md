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


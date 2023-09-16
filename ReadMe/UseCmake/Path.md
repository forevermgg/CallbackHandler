```cmake
if(NOT CMAKE_SOURCE_DIR STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    # is being built as part of another project, likely an SDK
endif()

# 其他主项目的构建逻辑
```
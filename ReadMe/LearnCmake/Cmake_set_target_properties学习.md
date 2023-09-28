### set_target_properties
set_target_properties命令用于设置目标的属性。它的语法如下：
```cmake
set_target_properties(<target> PROPERTIES <prop1> <value1> <prop2> <value2> ...)
```
其中，target是目标的名称，可以是可执行文件、库或自定义目标。prop1、prop2等是属性名称，value1、value2等是属性值。可以设置多个属性。

set_target_properties命令可以用于设置各种属性，例如输出名称、库类型、编译选项、链接选项等。

### 1.设置目标的属性
`set_target_properties(my_target PROPERTIES CXX_VISIBILITY_PRESET hidden)` 是一个 CMake 命令，用于设置目标（target）的 C++ 可见性（visibility）预设属性。

在 C++ 中，可见性是指符号（如函数、变量等）在编译单元之间的可见程度。CMake 的 `CXX_VISIBILITY_PRESET` 属性用于指定目标的可见性规则。而 `hidden` 是其中的一种可见性预设选项。

具体来说，`CXX_VISIBILITY_PRESET` 属性有以下几个可选值：

- `default`：使用编译器的默认可见性规则。
- `hidden`：将目标中的符号设置为隐藏可见性，即对外部不可见，只能在目标内部访问。
- `internal`：将目标中的符号设置为内部可见性，即对外部不可见，但可以在同一编译单元内的其他目标中访问。
- `protected`：将目标中的符号设置为受保护可见性，即对外部不可见，但可以在派生类中访问。

在上述示例中，`set_target_properties(my_target PROPERTIES CXX_VISIBILITY_PRESET hidden)` 将名为 `my_target` 的目标的 C++ 可见性预设属性设置为 `hidden`，意味着该目标中的符号对外部是隐藏的，只能在目标内部访问。

通过设置目标的可见性预设属性，可以控制符号的可见程度，从而增强代码的封装性和安全性。不同的可见性选项适用于不同的应用场景，开发者可以根据需要选择合适的可见性规则。


### 2.设置目标的属性
`set_target_properties(Storage PROPERTIES OUTPUT_NAME "realm")` 是一个 CMake 命令，用于设置目标（target）的属性。

在这个示例中，命令设置名为 "Storage" 的目标的属性。其中，`OUTPUT_NAME` 属性被设置为 "realm"。

`OUTPUT_NAME` 属性用于指定生成的目标文件的输出名称。在这种情况下，通过将 `OUTPUT_NAME` 设置为 "realm"，生成的目标文件将以 "realm" 作为其输出名称。

例如，可以使用以下命令将生成的共享库的输出名称设置为message：
```cmake
set_target_properties(message-shared PROPERTIES OUTPUT_NAME "message")
```
这将设置message-shared库的OUTPUT_NAME属性为message。在生成共享库时，输出文件的名称将为libmessage.so（在Linux上）或message.dll（在Windows上）。
### 3.设置目标的属性
`set_target_properties(Realm2JSON PROPERTIES OUTPUT_NAME "realm2json" DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})` 是一个 CMake 命令，用于设置目标（target）的属性。

在这个示例中，命令设置名为 "Realm2JSON" 的目标的属性。其中，`OUTPUT_NAME` 属性被设置为 "realm2json"，`DEBUG_POSTFIX` 属性被设置为 `${CMAKE_DEBUG_POSTFIX}`。

- `OUTPUT_NAME` 属性用于指定生成的目标文件的输出名称。在这种情况下，通过将 `OUTPUT_NAME` 设置为 "realm2json"，生成的目标文件将以 "realm2json" 作为其输出名称。

- `DEBUG_POSTFIX` 属性用于在调试模式下附加到目标文件名称的后缀。`${CMAKE_DEBUG_POSTFIX}` 是一个 CMake 变量，它包含了用于调试构建的后缀字符串（如 `_d`）。将该变量赋值给 `DEBUG_POSTFIX` 属性，可以确保在调试构建时生成的目标文件具有正确的后缀。


### 4.设置导入目标的属性
`set_target_properties(LibUV::LibUV PROPERTIES
IMPORTED_LOCATION "${LibUV_LIBRARY}"
INTERFACE_INCLUDE_DIRECTORIES "${LibUV_INCLUDE_DIRS}"
)` 是一个 CMake 命令，用于设置导入目标（imported target）"LibUV::LibUV" 的属性。

在这个示例中，命令设置名为 "LibUV::LibUV" 的导入目标的属性。其中，`IMPORTED_LOCATION` 属性被设置为 `${LibUV_LIBRARY}`，`INTERFACE_INCLUDE_DIRECTORIES` 属性被设置为 `${LibUV_INCLUDE_DIRS}`。

- `IMPORTED_LOCATION` 属性用于指定导入目标的位置。`${LibUV_LIBRARY}` 是一个 CMake 变量，它表示 LibUV 库的位置。将该变量赋值给 `IMPORTED_LOCATION` 属性，可以告知 CMake 导入目标的位置。

- `INTERFACE_INCLUDE_DIRECTORIES` 属性用于指定导入目标的接口头文件包含目录。`${LibUV_INCLUDE_DIRS}` 是一个 CMake 变量，它表示 LibUV 头文件的包含目录。将该变量赋值给 `INTERFACE_INCLUDE_DIRECTORIES` 属性，可以让依赖于该导入目标的其他目标能够正确地包含 LibUV 的头文件。

```
if(ANDROID)
  include_directories(${TARGET_DIR}/include)
  add_library(target SHARED IMPORTED)
  set_target_properties(target PROPERTIES IMPORTED_LOCATION ${TARGET_DIR}/Android/${ANDROID_ABI}/libtarget.so)
endif()
```


### 5.设置目标的属性
`set_target_properties(snappy PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR})` 是一个 CMake 命令，用于设置目标（target）"snappy" 的属性。

在这个示例中，命令设置名为 "snappy" 的目标的属性。其中，`VERSION` 属性被设置为 `${PROJECT_VERSION}`，`SOVERSION` 属性被设置为 `${PROJECT_VERSION_MAJOR}`。

- `VERSION` 属性用于指定目标的版本号。`${PROJECT_VERSION}` 是一个 CMake 变量，它表示项目的版本号。将该变量赋值给 `VERSION` 属性，可以设置目标的版本号为项目的版本号。

- `SOVERSION` 属性用于指定共享库的兼容版本号。`${PROJECT_VERSION_MAJOR}` 是一个 CMake 变量，它表示项目的主要版本号。将该变量赋值给 `SOVERSION` 属性，可以设置共享库的兼容版本号为项目的主要版本号。
```
#define SNAPPY_MAJOR ${PROJECT_VERSION_MAJOR}
#define SNAPPY_MINOR ${PROJECT_VERSION_MINOR}
#define SNAPPY_PATCHLEVEL ${PROJECT_VERSION_PATCH}
#define SNAPPY_VERSION \
    ((SNAPPY_MAJOR << 16) | (SNAPPY_MINOR << 8) | SNAPPY_PATCHLEVEL)
```
### 6.混淆 LINK_DEPENDS
libfilament-jni.map
```text
LIBFILAMENT {
  global:
    Java_com_google_android_filament_*;
    JNI*;
    *filament*BufferObject*;
    *filament*Camera*;
    *filament*Color*;
    *filament*Exposure*;
    *filament*Skybox*;
    *filament*Engine*;
    *filament*RenderableManager*;
    *filament*Aabb*;
    *filament*IndirectLight*;
    *filament*LightManager*;
    *filament*Renderer*;
    *filament*RenderTarget*;
    *filament*Scene*;
    *filament*ToneMapper*;
    *filament*Transform*;
    *filament*Material*;
    *filament*IndexBuffer*;
    *filament*VertexBuffer*;
    *filament*View*;
    *filament*Texture*;
    *filament*geometry*;

  local: *;
};
```
libfilament-utils-jni.symbols
```text
_Java_com_google_android_filament_*
```
```cmake
set(VERSION_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/libfilament-jni.map")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} -Wl,--version-script=${VERSION_SCRIPT}")
# Force a relink when the version script is changed:
set_target_properties(filament-jni PROPERTIES LINK_DEPENDS ${VERSION_SCRIPT})

set_target_properties(filament-utils-jni PROPERTIES LINK_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/libfilament-utils-jni.symbols)
```













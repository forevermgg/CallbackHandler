### abi
 + `ARMEABI`
 + `ARMEABI_V7A`
```cmake
# Hack the memmove bug on Samsung device.
if (ARMEABI OR ARMEABI_V7A)
    set(REALM_LINKER_FLAGS "${REALM_LINKER_FLAGS} -Wl,--wrap,memmove -Wl,--wrap,memcpy")
    set(REALM_COMMON_CXX_FLAGS "${REALM_COMMON_CXX_FLAGS} -DREALM_WRAP_MEMMOVE=1")
else()
    set(REALM_COMMON_CXX_FLAGS "${REALM_COMMON_CXX_FLAGS} -DREALM_WRAP_MEMMOVE=0")
endif()
```

### ANDROID_ABI
`ANDROID_ABI`是一个 `CMake` 变量，用于指定 `Android` 应用程序的目标 `ABI（Application Binary Interface`。

`ABI`定义了二进制代码在不同硬件和操作系统之间的接口规范，以确保应用程序能够在特定平台上正确运行。

对于`Android`应用程序，`ANDROID_ABI`用于指定应用程序所针对的 CPU 架构。常见的取值包括：

+ `armeabi-v7a`: 针对 `ARMv7` 架构的 32 位设备。
+ `arm64-v8a`: 针对 `ARMv8` 架构的 64 位设备。
+ `x86`: 针对 `x86` 架构的模拟器或物理设备。
+ `x86_64`: 针对 `x86_64` 架构的模拟器或物理设备。
你可以在 CMakeLists.txt 文件中使用以下方式设置 ANDROID_ABI 变量：

```cmake
set(ANDROID_ABI "armeabi-v7a")
```
或者，在命令行上使用 -D 参数来设置：

```bash
cmake -DANDROID_ABI=armeabi-v7a <path_to_source_directory>
```
设置`ANDROID_ABI`可以确保在构建过程中生成与目标架构兼容的二进制代码，并将其打包到相应的`APK`文件中。

需要注意的是，根据你的项目需求，可能需要同时支持多个`ABI`。在这种情况下，可以使用`ANDROID_ABI`变量的多个取值，例如：

```cmake
set(ANDROID_ABI "armeabi-v7a;x86")
```
这将生成适用于`ARMv7`和`x86`架构的二进制代码，并允许应用程序在相应的设备上运行。

请注意，在选择`ANDROID_ABI`时，应考虑目标设备的支持情况和性能需求，以确保应用程序在不同的`Android`设备上获得最佳的兼容性和性能。
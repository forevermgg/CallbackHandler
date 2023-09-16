### include(CheckIncludeFile)
`include(CheckIncludeFile)`是一个`CMake`命令，用于检查指定的头文件是否存在。

通过使用`include(CheckIncludeFile)`命令，可以在`CMake`构建过程中验证特定的头文件是否可用。这对于根据不同的操作系统或特定的功能要求来调整构建选项非常有用。

例如，假设我们要检查`<unistd.h>`头文件是否存在，可以使用以下方式：

```cmake
include(CheckIncludeFile)

check_include_file(unistd.h HAVE_UNISTD_H)
if(HAVE_UNISTD_H)
message("The unistd.h header file is found.")
else()
message("The unistd.h header file is not found.")
endif()
```
上述代码会调用 `check_include_file()` 函数来检查` <unistd.h> `头文件是否存在，并将结果存储在变量 `HAVE_UNISTD_H` 中。然后基于该变量进行进一步的条件判断和处理。
```cmake
check_include_file("sys/mman.h" HAVE_SYS_MMAN_H)
check_include_file("sys/resource.h" HAVE_SYS_RESOURCE_H)
check_include_file("sys/time.h" HAVE_SYS_TIME_H)
check_include_file("sys/uio.h" HAVE_SYS_UIO_H)
check_include_file("unistd.h" HAVE_UNISTD_H)
check_include_file("windows.h" HAVE_WINDOWS_H)
```

### include(CheckLibraryExists)
`include(CheckLibraryExists)`是一个`CMake`命令，用于检查指定的库文件是否存在。

通过使用`include(CheckLibraryExists)`命令，可以在`CMake`构建过程中验证特定的库文件是否可用。这对于根据不同的操作系统或特定的功能要求来调整构建选项非常有用。

例如，假设我们要检查数学库`libm`是否存在，可以使用以下方式：

```cmake
include(CheckLibraryExists)

check_library_exists(m pow "" HAVE_LIBM)
if(HAVE_LIBM)
message("The libm library is found.")
else()
message("The libm library is not found.")
endif()
```
上述代码会调用`check_library_exists()`函数来检查`libm`库是否存在，并将结果存储在变量 `HAVE_LIBM`中。然后基于该变量进行进一步的条件判断和处理。
```cmake
check_library_exists(z zlibVersion "" HAVE_LIBZ)
check_library_exists(lzo2 lzo1x_1_15_compress "" HAVE_LIBLZO2)
check_library_exists(lz4 LZ4_compress_default "" HAVE_LIBLZ4)
```

### include(CheckCXXCompilerFlag)
`include(CheckCXXCompilerFlag)`是一个`CMake`命令，用于检查编译器是否支持特定的`C++`编译选项。

通过使用`include(CheckCXXCompilerFlag)`命令，可以在`CMake`构建过程中验证编译器是否支持特定的编译选项。这对于根据不同的编译器或特定的功能要求来调整构建选项非常有用。

例如，假设我们要检查编译器是否支持`-std=c++11`标准，可以使用以下方式：

```cmake
include(CheckCXXCompilerFlag)

check_cxx_compiler_flag(-std=c++11 COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
    message("The compiler supports C++11 standard.")
else()
    message("The compiler does not support C++11 standard.")
endif()
```
上述代码会调用`check_cxx_compiler_flag()`函数来检查编译器是否支持`-std=c++11`标准，并将结果存储在变量`COMPILER_SUPPORTS_CXX11`中。然后基于该变量进行进一步的条件判断和处理。

```cmake
CHECK_CXX_COMPILER_FLAG("/arch:AVX" HAVE_VISUAL_STUDIO_ARCH_AVX)
CHECK_CXX_COMPILER_FLAG("/arch:AVX2" HAVE_VISUAL_STUDIO_ARCH_AVX2)
CHECK_CXX_COMPILER_FLAG("-mavx" HAVE_CLANG_MAVX)
CHECK_CXX_COMPILER_FLAG("-mbmi2" HAVE_CLANG_MBMI2)
```

### include(CheckCXXSourceCompiles)
`include(CheckCXXSourceCompiles)` 是一个`CMake`命令，用于检查是否能够成功编译给定的`C++` 源代码片段。

通过使用`include(CheckCXXSourceCompiles)`命令，可以在`CMake`构建过程中验证特定的`C++`源代码是否能够成功编译。这对于根据不同的操作系统或特定的功能要求来调整构建选项非常有用。

例如，假设我们要检查某个`C++`源代码片段是否能够成功编译，可以使用以下方式：

```cmake
include(CheckCXXSourceCompiles)

set(CODE "
#include <iostream>

int main() {
  std::cout << \"Hello, World!\" << std::endl;
  return 0;
}
")

check_cxx_source_compiles("${CODE}" COMPILATION_SUCCESSFUL)
if(COMPILATION_SUCCESSFUL)
message("The source code compiles successfully.")
else()
message("The source code does not compile successfully.")
endif()
```
上述代码将源代码片段存储在变量`CODE`中，并使用`check_cxx_source_compiles()`函数来尝试编译该代码。结果会存储在变量`COMPILATION_SUCCESSFUL`中，然后根据该变量进行进一步的条件判断和处理。

```cmake
include(CheckCXXSourceCompiles)
check_cxx_source_compiles("
int main() {
  return __builtin_expect(0, 1);
}" HAVE_BUILTIN_EXPECT)

check_cxx_source_compiles("
int main() {
  return __builtin_ctzll(0);
}" HAVE_BUILTIN_CTZ)

check_cxx_source_compiles("
__attribute__((always_inline)) int zero() { return 0; }

int main() {
  return zero();
}" HAVE_ATTRIBUTE_ALWAYS_INLINE)

check_cxx_source_compiles("
#include <tmmintrin.h>

int main() {
  const __m128i *src = 0;
  __m128i dest;
  const __m128i shuffle_mask = _mm_load_si128(src);
  const __m128i pattern = _mm_shuffle_epi8(_mm_loadl_epi64(src), shuffle_mask);
  _mm_storeu_si128(&dest, pattern);
  return 0;
}" SNAPPY_HAVE_SSSE3)

check_cxx_source_compiles("
#include <immintrin.h>
int main() {
  return _bzhi_u32(0, 1);
}" SNAPPY_HAVE_BMI2)

check_cxx_source_compiles("
#include <arm_neon.h>
int main() {
  uint8_t val = 3, dup[8];
  uint8x16_t v = vld1q_dup_u8(&val);
  vst1q_u8(dup, v);
  return 0;
}" SNAPPY_HAVE_NEON)
```

### include(CheckSymbolExists)
`include(CheckSymbolExists)`是一个`CMake`命令，用于检查指定的符号（函数、变量等）是否存在于给定的头文件中。

通过使用`include(CheckSymbolExists)`命令，可以在`CMake`构建过程中验证特定的符号是否在指定的头文件中存在。这对于根据不同的操作系统或特定的功能要求来调整构建选项非常有用。

例如，假设我们要检查头文件 `<math.h>`中是否存在符号`sqrt`，可以使用以下方式：

```cmake
include(CheckSymbolExists)

check_symbol_exists(sqrt math.h HAVE_SQRT)
if(HAVE_SQRT)
message("The symbol sqrt is found in math.h.")
else()
message("The symbol sqrt is not found in math.h.")
endif()
```
上述代码会调用`check_symbol_exists()`函数来检查`sqrt`符号是否存在于`math.h`头文件中，并将结果存储在变量`HAVE_SQRT`中。然后基于该变量进行进一步的条件判断和处理。

```cmake
check_symbol_exists("mmap" "sys/mman.h" HAVE_FUNC_MMAP)
check_symbol_exists("sysconf" "unistd.h" HAVE_FUNC_SYSCONF)
```


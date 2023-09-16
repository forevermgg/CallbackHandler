### JSONParser.cmake
https://github.com/sbellus/json-cmake

```
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/deps_lib_list.json deps_lib_json)

sbeParseJson(json_result deps_lib_json)

set (need_add_lib "")

foreach(var ${json_result})
    list(APPEND need_add_lib ${${var}})
endforeach()

sbeClearJson(json_result)
```
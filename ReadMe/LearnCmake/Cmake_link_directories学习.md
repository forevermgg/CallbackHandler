### 
`link_directories`是一个`CMake`函数，用于指定链接器在链接可执行文件或共享库时搜索库文件的路径。

具体来说，当使用`target_link_libraries`命令将库文件链接到可执行文件或共享库时，链接器需要知道库文件的位置。如果库文件不在默认搜索路径中，就需要使用`link_directories`命令指定库文件的搜索路径。

例如，以下代码将`/usr/local/lib`目录添加到链接器搜索路径中：
```cmake
link_directories(/usr/local/lib)
```
这样，在使用`target_link_libraries`命令链接库文件时，链接器就会在`/usr/local/lib`目录中搜索库文件。

需要注意的是，使用`link_directories`命令并不会自动将库文件链接到可执行文件或共享库中，它只是指定了链接器搜索库文件的路径。如果要将库文件链接到可执行文件或共享库中，还需要使用`target_link_libraries`命令。


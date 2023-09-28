### list
list命令用于操作CMake的列表变量。它的语法如下：
```cmake
list(<SUBCOMMAND> [<args>...])
```
其中，SUBCOMMAND是列表操作的子命令，args是子命令的参数。常用的子命令包括：

+ APPEND：将一个或多个值添加到列表的末尾。
+ INSERT：将一个或多个值插入到列表的指定位置。
+ REMOVE_ITEM：从列表中删除指定的值。
+ LENGTH：返回列表中的元素数。
+ GET：获取列表中指定位置的值。
+ SET：设置列表中指定位置的值。
例如，可以使用以下命令将值添加到列表中：
```cmake
list(APPEND my_list "value1" "value2" "value3")
```
这将把value1、value2和value3添加到名为my_list的列表的末尾。可以使用以下命令获取列表中的元素数：
```cmake
list(LENGTH my_list list_length)
```
这将把my_list列表的元素数存储在list_length变量中。可以使用以下命令获取列表中指定位置的值：
```cmake
list(GET my_list 0 first_value)
```
这将把my_list列表中第一个元素的值存储在first_value变量中。
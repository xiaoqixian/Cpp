---
author: lunar
date: Fri 25 Sep 2020 07:35:26 PM CST
---

### CMake 教程(基础篇)

>   本系列笔记均来自与github仓库: https://github.com/ttroy50/cmake-examples

#### CMake的使用流程

一般先写好一个CMakeLists.txt，然后创建一个build目录用于存放二进制文件。来到build目录，使用命令`cmake ..`生成makefile，最后运行`make all`执行makefile生成项目的可执行文件。

当然创建build目录的过程并不是必须的，但是一般都会讲build后的二进制文件置于build目录中以保持源码的整洁。

最简单的一个CMakeLists.txt如下：

```cmake
cmake_minimum_required(VERSION 3.10)

# set project name and version number
project(Tutorial VERSION 1.0)

add_executable(Tutorial turorial.cpp)
```

`add_executable`命令用于构建一个可执行文件，第一个参数是可执行文件的名字，其后的参数为该可执行文件所依赖的源文件。

一般在写中大型项目时，都会在当前项目的根目录写入CMakeLists.txt。而C/C++的include只能include当前目录和/usr/include里面的库。所以一般会设置一个include的起始路径：

```cmake
target_include_directories(Tutorial PUBLIC "${DIR}")
```

#### 设置CMake变量

CMake有很多用于指示编译的variable或者说flag，进行设置的语法格式为

```cmake
set(variable value)
```

具体有哪些变量呢？

1. 设置C++标准：`CMAKE_CXX_STANDARD`

    C++存在多个标准，如C++98，C++11。这个变量就可以用于指示编译时遵循的C++标准。

    同时还要将变量`CMAKE_CXX_STANDARD_REQUIRED`设为True

2. cmake下的根目录：`CMAKE_BINARY_DIR`，运行cmake命令的那个目录就是cmake下的根目录。

这些都是内置变量，也可以通过同样的命令创建变量。

#### 使用CMake变量

通过`set`命令创建的变量可以通过`${variable}$`的格式进行使用。

#### file命令

`file`命令支持通过正则表达式将一类文件作为变量，如将src目录下所有的cpp文件作为sources，可以用

```cmake
file(GLOB SOURCES "src/*.cpp")
```

#### 加入路径

在直接使用命令行进行C/C++程序编译时可以使用`-I`选项添加源代码的目录，而在CMakeLists.txt 中我们可以借助`target_include_directories()`函数来完成这一点，语法格式为：

```cmake
target_include_directories(<target> [SYSTEM] [BEFORE]
  <INTERFACE|PUBLIC|PRIVATE> [items1...]
  [<INTERFACE|PUBLIC|PRIVATE> [items2...] ...])
```

示例：

```cmake
target_include_directories(target
	PRIVATE ${PROJECT_SOURCE_DIR}/include
)
```

target必须是之前通过`add_executable`或者`add_library`函数创建的可执行对象。

`INTERFACE,PUBLIC,PRIVATE`三个修饰符具体使用哪个决定于你头文件的包含关系，具体还请看这篇[文章](https://zhuanlan.zhihu.com/p/82244559)。

#### 添加静态库

`add_library()`函数用于根据一些源文件创建库

```cmake
add_library(hello_lib STATIC
	src/hello.cpp
)
```

这个函数将会创建一个名为`libhello_lib.a`的静态库。

#### 添加动态库

动态库，也被称为共享库(shared library). 动态库与静态库最大的区别在于静态库在编译期间就进行链接，而动态库在运行期间进行链接，并且可以被多个文件所共享。

从空间上来看，动态库更省空间；从运行时间来看，静态库更快。

添加动态库的函数与添加静态库一样，只要将`STATIC`改为`SHARED`。

#### 为库添加路径

也可以直接在创建库时添加一个路径下的所有文件进去：

```cmake
target_include_directories(hello_lib 
	PUBLIC ${PROJECT_SOURCE_DIR}/include
)
```

添加的路径下的文件将会在两个地方被用到：

- 当编译`hello_lib`库时
- 当编译任何链接了`hello_lib`库的target时

三个修饰符表示的作用域分别为：

- PRIVATE: 路径下的文件只对于该库可见
- INTERFACE：路径下的文件对于链接了该库的文件可见
- PUBLIC：两者都有

#### 链接库

通过`target_link_libraries()`函数进行库的链接

示例：

```cmake
add_executable(target
	src/main.cpp
)

target_link_libraries(target
	PRIVATE
		hello_lib
)
```

相当于编译器参数中的`-rdynamic`

#### 安装target

CMake允许用户通过`install`函数创建一个用户可以通过`make install`命令运行来安装的target。

示例：

```cmake
install(TARGETS install_binary
	DESTINATION bin
)
```

这个地址bin是一个根目录下的子目录，而这个根目录由运行cmake命令时由参数`-DCMAKE_INSTALL_PREFIX`进行指定。

比如：`cmake .. -DCMAKE_INSTALL_PREFIX=/install/location`

则会安装在/install/location/bin目录下。

**安装库**

动态库也可以根据该命令进行安装

```cmake
install(TARGETS install_library
	LIBRARY DESTINATION lib
)
```

如果没有设置`-DCMAKE_INSTALL_PREFIX`，默认的安装路径为/usr/local

#### Build Type

cmake有一系列内置变量Build Type（我也不知道这个翻译对不对）来控制编译的level。这个和用命令行编译时用的一些参数是对应的，对应结果如下：

-   Release: 如果是要发布的包，相当于`-o3 -DNDEBUG`参数；
-   Debug: 如果是用于debug，`-g`；
-   MinSizeRel: 使得编译出来的包最小，`-Os -DNDEBUG`；
-   RelWithDebInfo: 发布带有debug信息的包，`-O2 -g -DNDEBUG`

这个`-DNDEBUG`参数，根据我的项目经验，如果使用了则会被检测到定义了一个名为`NO_DEBUG`的宏。所以在源代码中想要打印debug信息的话，就可以检测这个宏。如果没有定义这个宏，就打印一些东西；否则就什么都不做。

这些Build Type在运行cmake时可以这样设置：

`cmake .. -DCMAKE_BUILD_TYPE=Release`

**设置默认Build Type**

cmake原本的默认Build Type是不带任何编译器参数用于优化，你可以在CMakeLists.txt中设置默认：

```cmake
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message("Setting build type to 'RelWithDebInfo' as none was specified")
	set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()
```

#### 设置编译参数flags

有两种方法用于设置flags：

-   通过`target_compile_definition()`函数；
-   使用内置变量`CMAKE_C_FLAGS`(C语言)和`CMAKE_CXX_FLAGS`(C++)；

在现代CMake中设置C ++标志的推荐方法是使用按目标标志，可以通过`target_compile_definitions()`函数将其填充到其他目标。`target_compile_definitions()`函数用于给一个target定义一个宏，其语法格式为

```cmake
target_compile_definitions(<target>
  <INTERFACE|PUBLIC|PRIVATE> [items1...]
  [<INTERFACE|PUBLIC|PRIVATE> [items2...] ...])
```

如果在item前面加上`-D`字样，该宏将会被移除。

三个限定符则用于指定作用域。

**设置默认的C++编译参数**

可以以如下的格式设置参数

```cmake
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DEX2" CACHE STRING "Set C++ Compiler Flags" FORCE)
```

#### 导入第三方库

CMake通过`find_package()`函数来寻找第三方库，这个函数会在`CMAKE_MODULE_PATH`下寻找格式为"FindXXX.cmake"的模块。在linux下，`CMAKE_MODULE_PATH`的默认地址为`/usr/local/cmake/Modules`

函数示例：

`find_package(Boost 1.46.1 REQUIRED COMPONENTS filesystem system)`

参数说明：

-   Boost: 库名
-   1.46.1：最小版本
-   REQUIRED: 如果找不到这个库则build失败
-   COMPONENTS：The list of libraries to find

大多数成功导入的库会设置一个名为`XXX_FOUND`的变量（库名+FOUND），可以通过判断这个变量是否为true来判断库是否导入成功。


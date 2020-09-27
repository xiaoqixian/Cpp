---
author: lunar
date: Fri 25 Sep 2020 07:35:26 PM CST
---

## CMake 教程

### CMake 教程

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

学习到这一阶段，可以给出一个小的demo了。

某工程的目录树如下：






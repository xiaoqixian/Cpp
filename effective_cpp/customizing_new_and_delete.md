---
author: lunar
date: Sat 03 Oct 2020 06:41:56 PM CST
---

### 定制 new 和 delete

#### 条款49: Understand the behavior of the new-handler.

当operator new 无法满足某一项内存分配需求时，它会抛出异常。某些老的编译期会返回一个NULL指针。

当 operator new 抛出异常以反映一个未获满足的内存需求之前，它会先调用一个客户指定的错误处理函数。为了指定这个函数，客户必须调用`set_new_handler`，定义于标准库的`<new>`:

```c++
namespace std {
    typedef void (*new_handler)();
    new_handler set_new_handler(new_handler p) throw();
}
```

可见，`new_handler`是一个不接收任何参数，也不返回任何东西的函数指针。

`set_new_handler`的参数是个指针，指向`operator new`无法分配内存时该被调用的函数。其返回值也是个指针，指向`operator new`被调用前正在执行的那个`new_handler`函数。当`set_new_handler`第一次被调用时则会返回一个NULL。

一个设计良好的`new_handler`应该做到以下事情：

-   让更多内存可以用；

-   安装另一个`new_handler`，或许另一个`new_handler`有办法分配到更多内存。方法也是通过`set_new_handler`函数；

-   卸除`new_handler`

    如果没有安装`new_handler`，operator new会在内存分配不成功时抛出异常；

-   直接抛出`std::bad_alloc`异常，或者abort, exit

#### 条款50: Understand when it makes sense to replace new and delete.


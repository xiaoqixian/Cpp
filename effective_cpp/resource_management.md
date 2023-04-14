---
author: lunar
date: Sun 13 Sep 2020 09:27:37 AM CST
---

## 资源管理

### 条款13: Use objects to manage resource

由于C++面向对象中析构函数的存在, 可以作为一种自动释放资源的方式.

通过对象管理资源, 当控制流离开对象时, 就可以通过析构函数来完全释放资源.

标准程序库提供的`auto_ptr`正是针对这种形势而设计的. `auto_ptr`是个"类指针"对象, 也叫智能指针. 通过类指针管理资源后, 当指针的内存被释放时, 指针所指向的内存也会被释放.

由于智能指针的这个特点, 所以**不要让多个智能指针指向同一个对象**. C++为了预防这一点, 在用户通过copy函数或者copy assignment操作符拷贝一个智能指针时, 原智能指针会变成null.

需要注意的一点是: `auto_ptr`和`tr1::shared_ptr`两者都在其析构函数内做`delete`而不是`delete[]`, 意味着动态分配的数组无法被回收, 尽管这么做可以通过编译.

### 条款14: Think carefully about copying behavior in resource-managing classes.

可以采用以下两种方法:
- 禁止复制: 具体方法参见条款06, 即将copying函数定义为private.
- 对底层资源使用"引用计数法": 底层资源直到没有被其它对象所引用时才会被释放.

### 条款15: Provide access to raw resources in resource-managing classes.

使用资源管理类的好处是将资源封装了起来, 但是有时需要对原始资源直接进行处理, 所以需要提供从资源管理类到原始资源的转换.

为此, `tr1::shared_ptr`和`auto_ptr`都提供了get成员函数, 用来进行显式转换.

此外, 这两个智能指针还提供了对于操作符(opetator->和operator\*)的重载, 使得可以如同直接使用原始资源一样通过指针或"."获取资源.

### 条款16: User the same form in corresponding uses of new and delete.

在通过指针释放对象时, 我们需要考虑: 即将别删除的那个指针, 所指的是单一对象还是对象数组? 这个问题决定着将会有多少个对象的析构函数被调用.

如果在构造函数中有通过new动态分配内存且有多个构造函数的话, 则必须在每个构造函数内用`new`还是用`new[]`保持统一. 因为析构函数只有一个, 这样的话就无法确定到底是用`delete`还是`delete[]`.

在使用`typedef`自定义时也应该确认清楚, 原本的类型到底是对象还是对象数组.

### 条款17: Store newed objects in smart pointers in **standalone statements**.

这个条款的意思是在被管理资源实例化之后必须马上放入智能指针之中, 确保中间不会运行任何可能抛出异常的语句. 否则就可能直接运行捕捉异常的语句了, 而被管理资源被实例化后没有被放入智能指针, 导致内存泄露.



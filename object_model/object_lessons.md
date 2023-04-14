---
author: lunar
date: Fri 09 Oct 2020 07:49:50 PM CST
---

## 第一章 关于对象

### 1.1 C++ 对象模式

```c++
class Point {
public:
    Point(float xval);
    virtual ~Point();
   	float x() const;
    static int pointCount();
protected:
    virtual ostream& print(ostream& os) const;
    float _x;
    static int _point_count;
};
```

这样一个类在内存中会怎样布局呢？

#### 简单对象模型 (A simple object model)

在简单模型中，一个object是一系列的slots，每一个slot都指向对象的一个member。这样只需要考虑member的数量来分配内存，而无需考虑类型。

这是一种舍弃效率换空间的模型。

#### 表格驱动对象模型 (A table-driven object model)

该模型将所有与members有关的信息抽取出来，分别放在 data member table和member function table之中，然后在object中内含指向这两个table的指针。member function table中也是一系列的函数指针，data member table则是实实在在的数据。

#### C++ 对象模型 (The C++ object model)

在此模型中，non-static data members 被配置于每一个 class object之内，static data members则被放在所有的 class object 之外。static 和 non-static member function 也被放在所有的 class object 之外。

至于 virtual functions:

1.  每一个 class 产生一堆指向 virtual functions 的指针，放在表格中，作为虚表；
2.  每一个 class object 被添加一个指向虚表的指针，称为vptr。每一个 class 所关联的 *type_info* object（用以支持 runtime type identification）也经由虚表被指出来，通常放在表格的第一个slot处。

#### 加上继承 （Adding inheritance）

C++支持单一继承，也支持多重继承。还支持虚继承，所谓虚继承，即不管父类被派生多少次，永远只会存在一个实体。

在class内部存在一种类似于虚表的结构，通过一个指针指向一个base class table。缺点就是随着继承的增加会导致取地址的负担增加。
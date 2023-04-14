---
author: lunar
date: Wed 16 Sep 2020 03:14:38 PM CST
---

## 6. 继承和面向对象程序设计

### 条款32: Make sure public inheritance models "is-a".

如果你令`class D`以public形式继承`class B`, 你便是告诉编译器每一个类型为D的对象同时也是一个类型为B的对象.

"public继承"意味着**is-a**, 适用于base class的每一件事也要适用于derived class身上.

### 条款33: Avoid hiding inherited names.

这是一个关于作用域的问题. 当编译器处理一个变量时, 它会现在local作用域内查找该变量. 

对于继承了base class和derived class, 其作用域嵌套在base class内. 对于在derived class的函数`mf4`中调用的一个函数`mf2`, 编译器会首先在`mf4`的作用域内查找. 然后在derived class内查找, 然后去base class内查找. 然后在包含base class的namespace内查找. 最后在global作用域内查找.

当子类需要重载父类的成员函数时, 则需要重载所有的同名函数. 如果只是重载一个, 则其它同名函数将会被覆盖, 无法被子类继承. 即使参数不同也一样.

这类问题的解决办法可以借助`using`关键字来完成, 例如下面的代码, 子类就可以正常使用父类的方法.

```c++
#include <iostream>
using namespace std;

class Base {
public:
    void func(int x) {
        cout << "base func" << endl;
    }
};

class Derived: public Base {
public:
    using Base::func;
    void func() {
        cout << "derived func" << endl;
    }
};

int main() {
    Derived d;
    d.func(1);
    return 0;
}
```

### 条款34: Differentiate between inheritance of interface and inheritance of implementation.

对于base class的成员函数的声明和定义, 我们希望有多种不同的继承方式:
1. 只继承base class的函数声明: 将相应接口设为纯虚函数可以实现.
2. 同时继承函数声明和定义: 使用非纯虚函数可以实现

### 条款35：Consider alternatives to virtual functions.

#### 藉由 Non-Virtual Interface 手法实现 *Template Method* 模式

有这样一种场景：对于所有动物的父类Animal，所有动物都要定义“吃”这个动作，但是每个子类的实现方式又有不同。我们需要在每次调用“吃”的API时都要打印一次日志，这个日志对于所有不同实现的子类都是相同的。

我们可以这样实现：将具体吃的实现函数定义为`private virtual`类型的函数，而日志打印的函数放在一个`wrapper`函数里面，这个函数是non-virtual的，在这个函数里可以调用具体的实现函数。

这种手法称为*non-virtual interface* (NVI)

#### 藉由 Function Pointers 实现 *Strategy* 模式

利用函数指针将进行实际操作，在构造函数中将这个函数指针传入。

#### 藉由 tr1::function 完成 *Strategy* 模式

std::tr1::function 是一种通用、多态的函数封装，tr1::function的实例可以对任何可调用的对象进行存储、复制和调用操作。

在实例化时，其接受一个函数的类型作为模板参数。比如对于接受一个`const int`，返回`int`的函数来说：

```c++
std::tr1::function<int (const int)> instance;
```

可以将这个`instance`指向任何可调用对象的地址，然后就可以像普通函数一样进行调用。

> 这个条款看不下去了，看得人头大。

### 条款36：Never define an inherited non-virtual function.

这个是很显而易见的，如果你想要重载父类的函数的话，最好还是使用virtual函数。因为non-virtual函数都是静态绑定的，你用哪种类型的变量调用成员函数，就只会调用该种类型的成员函数，不会管你对象具体是哪种类型的。

### 条款37：Never redefine a function's inherited default parameter value.

由于条款36，只需要考虑要不要重定义继承而来的带有缺省参数值的virtual函数。

这种情况下，本条款成立的理由是：**virtual函数系动态绑定，而缺省参数值是静态绑定的。**

也就是说，对于`Base* b = new Derived()`类似形式定义的变量，当b调用函数时，函数的具体实现虽然会使用`Derived`类里面定义的，但是默认参数却会使用`Base`中定义的。

如果实在想要为函数带一个默认参数，可以考虑采用条款35所提到的NVI 风格的写法。

### 条款38：Model "has-a" or "is-implemented-in-terms-of" through composition.

中文意思是：通过复合塑模出 has-a 或 “根据某物实现出”。

复合是类型之间的一种关系，当某种类型的对象内含其它类型的对象，便是这种关系。

程序中的对象其实相当于你所塑造的世界中的某些事物，例如人、汽车、一帧帧视频画面等等，这样的对象属于应用域（application domain）。另一些对象则完全是实现细节上的人工制品，比如互斥锁，二叉树，缓冲区等等，这类对象属于实现域（implementation domain）。

应用域的对象之间的关系则是has-a，而实现域之间的关系则是“根据某物实现出”。例如，你要实现一个自己版本的set，你想通过STL的list 来实现，但是你不能直接继承list，因为前面的条款讲过：直接public继承会导致两个对象之间的关系变为is-a。所以只能在底层用list来装载数据，就如同list底层通过数组来装载数据。这样两者之间的关系就是“根据某物实现出”。

### 条款39: Use private inheritance judiciously.

private继承意味着两个主要的规则：

1. 编译器不会自动将derived class对象转换为base class对象；
2. 由private class继承而来的所有成员，在derived class中都会变成private属性，即使它们在base class中属于protected甚至public属性。

private继承意味着 implemented-in-terms-of 的关系。如果你让class D private继承class B，你的用意是为了采用class B内已经备妥的特性，而不是B对象和D对象存在任何观念上的关系。

复合的意义也是这样，尽可能使用复合，只有当protected成员或者virtual函数牵扯进来的时候才使用private继承。

给个示例：

一个Widget类型在实现中需要使用到Timer的某个函数并重写一次，想要重写就只能继承。重写时放在private领域内，因为这是内部实现时要用到的接口，不是可以对外开放的接口。

还有一种写法，在Widget内声明一个嵌套式`private class WidgetTimer`，后者以public的形式继承`Timer`。

有两点理由证明下面这种写法更好：

1. `Widget`类可能会被别的类继续继承，这样这个virtual函数又可能会被重写。而下面的写法就不存在这种问题，因为private领域内定义的类是不对子类可见的。
2. 编译依存性的问题，如果`Widget`继承自`Timer`，则势必会和`Timer`产生编译依存性。而后者可以选择将`WidgetTimer`移出，然后带一个简单的`WidgetTimer`声明式。

### 条款40：Use multiple inheritance judiciously.

> C++其实并不建议使用多重继承。

如果你继承的类也继承了其它的类的话，这时来自于基类的数据会同时存在你的父类和本类中，造成了空间的浪费。这时可以使用虚继承。

对于虚继承的建议：

1. 非必要不适用；
2. 如果要使用virtual base classes，极可能避免在其中放置数据。


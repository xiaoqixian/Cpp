---
author: lunar
date: Sat 12 Sep 2020 08:18:14 PM CST
---

## 构造/析构/赋值运算

### 条款05: Functions that C++ silently writes and calls.

如果你定义一个空类, 不做任何处理的话, 编译器会为其定义一下四个函数:
1. 无参构造函数
2. 析构函数
3. copy构造函数
4. copy assignment操作符

```c++
class Empty {
public:
    Empty() {...}
    ~Empty() {...}
    Empty(const Empty& rhs) {...}
    Empty& operator=(const Empty& rhs) {...}
};
```

编译器产出的析构函数是non-virtual的.

### 条款06: Explicitly disallow the use of compiler-generated functions you do not want.

有时不希望一个对象被拷贝, 就需要明确不让编译器自动生成拷贝函数.

正确的做法就是定义这两个拷贝函数, 但是将修饰符设为`private`, 这样编译器既不会自动生成, 也无法被调用.

### 条款07: Declare destructors virtual in polymorphic base classes.

在写一个工厂模式时, 通常的情况是返回的类型是父类, 而实际返回的对象是子类. 当对这个子类进行销毁时, 由于类型是父类的, 所以只能调用父类的析构函数. 然后在C++中, 调用父类的析构函数只能释放那些从父类继承的资源, 而子类的derived的成分没被销毁, 造成内存泄露.

最简单的解决办法是: 给父类定义一个virtual析构函数, 这样就允许子类重写析构函数, 完全释放资源.

如果class不含任何virtual函数, 说明它并不会将被作为一个base class, 那么将其析构函数强行定义为virtual将是个馊主意. 因为所有定义了虚函数的class都会被塞进一个虚函数表, 导致内存占用增加. 因此不要强行使用virtual.

所以一方面, 我们在定义base class时, 要将析构函数定义为虚函数. 另一方面, 我们不要继承那些没有定义virtual析构函数的类, 包括所有STL容器都是这样. C++无法像Java一样从语法层面禁止一个类的派生, 所以需要程序员自己注意. 而如果base class带有任何virtual函数, 它就应该拥有一个virtual析构函数.

### 条款08: Prevent exceptions from leaving destructors.

C++并不禁止在析构函数中抛出异常, 但是并不鼓励这种行为.

因为在抛出多个异常时, 可能会导致C++的不明确行为.

如果一个析构函数调用的函数可能抛出异常, 则应该捕捉所有异常, 并对异常进行处理或者强行终止程序.

或者提供一个函数让客户对该异常进行处理.

### 条款09: Never call virtual funtions during construction or destruction.

因为在子类实例化时, 会首先调用父类的构造函数, 如果在此期间调用了virtual函数, 此时的对象是父类对象, 所以虚函数不会下降到子类的重写的虚函数, 导致父类的虚函数被调用.

这种结果是必然的, 因为父类的构造函数先于子类调用, 如果虚函数下降到子类, 可能会调用子类的成员变量, 而子类还没有初始化, 从而带来风险. 这在C++中是不被允许的.

析构函数同样如此, 父类的析构函数后于子类的析构函数被调用, 父类的虚函数自然更不会下降到子类的虚函数.

### 条款10: Have assignment operators return a reference to \*this.

要让赋值操作符返回一个(\*this)的引用.

### 条款11: Handle assignment to self in operator=

在定义=操作符时, 要处理一下自己给自己赋值的情况.

最好的方法是做一下**证同测试(identity test)**.
```c++
Widget& Widget::operator=(const Widget& rhs) {
    if (this == &rhs) { //identity test
        return *this;
    }
}
```

### 条款12: Copy all parts on an object.

在程序员自定义拷贝函数时, 如果有成员变量没有被拷贝, 编译器并不会提醒程序员. 一般会认为是程序员有意而为之.

如果需要拷贝的函数继承了父类的话, 情况将变得更加麻烦. 如果在成员初值列里没有调用父类的构造函数, 那么福利的内容将不被复制.

所以需要在成员初值列中调用父类的构造函数, 并将拷贝函数的参数作为参数传入父类的构造函数中.

操作符的定义同样也是.

所以需要记住:
1. 复制所有的local变量
2. 调用所有base class内适当的拷贝函数



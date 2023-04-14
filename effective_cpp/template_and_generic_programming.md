---
author: lunar
date: Thu 01 Oct 2020 10:46:38 AM CST
---

### 模板与泛型编程

#### 条款41: Understand implicit interfaces and compile-time polymorphism

>   隐式接口受知乎用户"invalid s"启发，感谢。

在面向对象编程中，一类重要的思想是：显式接口（explicit interfaces）和运行期多态（runtime polymorphism）。

显式接口即在源码中可见，而运行期多态即因为virtual函数的存在而不得不等到运行期才能决定具体调用哪个函数。

而在模板编程中，隐式接口（implicit interfaces）和编译期多态（compile-time polymorphism）反而变得更重要。

编译期多态和运行期多态的区别类似于“哪一个重载函数被调用”（发生在编译期）和“哪一个virtual函数被绑定”（发生在运行期）之间的差异。

而这个隐式接口到底是什么意思呢？

首先讲一下显式接口，所谓显式接口，即根据参数类型就可以明确知道参数可以完成哪些动作，都是明确的。

而隐式接口，其参数类型都是模板，看似任何类型都可以传入，没有任何接口可言。但是如果在代码里面写明了两个模板类型的变量相乘，则该函数就侧面拒绝了所有无法相乘的参数类型。这种隐形约束，也被称为隐形接口。

所以在书里才会说“隐式接口由表达式组成”（不得不说，这里讲得太隐晦了）

#### 条款42: Understand the two meanings of typename.

在规定一个函数为模板函数时，常会在顶部声明：

```c++
template<typename T>
```

但有时也会看到这种写法：

```c++
template<class T>
```

这两种写法其实没有区别，typename和class都是告诉编译期：参数T是一种类型。

但是这两者在其它地方都有别的用处。

class就不用说了，用于定义一个类。

而typename可以用于声明某个变量是一种类型，在某些地方必须要用到。比如：

在C++中，template内出现的名称如果依赖于某个template参数，则我们称之为从属名称（dependent names）。如果从属名称在class内呈嵌套状（即T::XXX的这种形式），我们称之为嵌套从属名称（nested dependent names）。

嵌套从属名称可能会导致解析困难，比如对于下面这段代码：

```c++
template<typename T>
void func(const T& container) {
    T::iterator* p;
    ...
}
```

可能你只是想要定义一个iterator的指针。然而，编译器并不知道iterator到底是T 内的一个static成员变量还是一种类型。如果是变量，可以当作乘法来解决；如果是类型，才会被解析为指针。

而在缺省情况下，编译器就不会把它当作一种类型进行处理，从而代码与你的预想出现偏差。

于是，typename有了它的用武之地。前面讲了，可以用typename关键字来声明后面的这个变量是一种类型。

```c++
template<typename T>
void func(const T& container) {
    typename T::iterator* p;
    ...
}
```

**注意**：typename只能作为嵌套从属名称的前缀，你不要随随便便就添加。而且，就算是在嵌套从属名称前面，也有不能添加typename的例外：**typename不能出现在base classes list内的嵌套从属名称之前，也不可以在member initialization list中出现**。

例如：

```c++
template<typename T>
class Derived: public Base<T>::Nested {//基类列表中不允许出现typename
public:
    explicit Derived(int x):
    Base<T>::Nested(x) {//member initialization list中也不允许出现typename
        typename Base<T>::Nested temp;
    }
};
```

#### 条款43: Know how to access names in templatized base classes.

#### 条款44: Factor paramete-independent code out of templates.

本条款讲解如何避免模板类和模板函数在编译后变得膨胀。

引入示例：

```c++
template<typename T, std::size_t n>
class SquareMatrix {
public:
    void invert(); //求逆矩阵
};
```

没错，非类型参数size_t可以放在模板内，完全合法。

而下面这些代码：

```c++
SquareMatrix<double,5> sm1;
sm1.invert();
SquareMatrix<double,10> sm2;
sm2.invert();
```

一个惊人的事实是在上面的代码中invert函数会有两种实现，而两种实现的区别仅仅是size_t的不同而已。这就是属于典型的编译后的代码膨胀了。

因非类型模板参数而造成的代码膨胀，往往可以消除，做法是以函数参数或class成员变量替换template参数。

#### 条款45: Use member function templates to accept "all compatible types."

指针相较于智能指针的一个优势在于：指针支持隐式转换（implicit conversions）。例如Derived class指针可以隐式转换为 base class指针。

而对于智能指针，封装了具有父子关系的指针的两个智能指针完全没有任何关系。所以向下面这样进行隐式转换肯定是无法成功的。

```c++
SmartPtr<Base> pt2 = SmartPtr<Derived>(new Derived()); 
```

所以为了满足我们的需求，我们应该关心的是如何编写智能指针的构造函数来满足我们的转型需要。

我们需要的是一个构造模板，其作用是为class生成函数：

```c++
template<typename T>
class SmartPtr {
public:
    template<typename U>
    SmartPtr(const SmartPtr<U>& other);
};
```

这一类构造函数根据对象 U 创建对象 T ，而U和T是同一个template的不同具现体，称之为泛化copy构造函数。

然而，我们需要的不仅如此。上述代码对于任何两种类型都可以进行转换，而我们所需求的隐式转换对于两个类是有要求的。比如我们不希望Base被隐式转换为Derived。

答案如下：

```c++
template<typename T>
class SmartPtr {
public:
    template<typename U>
    SmartPtr(const SmartPtr<U>& other): heldPtr(other.get()) {
        ...
    }
    T* get() const {
        return heldPtr;
    }
private:
    T* heldPtr;
}
```

这样，如果U无法转型为T的话就不会通过编译。

声明了泛化的copy构造函数后，编译期还是会自动声明一个常规的copy构造函数。

#### 条款46: Define non-member functions inside templates when type conversions are desired.

We talked about why only non-member functions are capable for implementing implicit type conversions on all arguments.

在这个条款中，我们将条款24中的两个例子模板化了。

```c++
template<typename T>
class Rational {
public:
    Rational(const T& numerator=0, const T& denominator=1);
    ...
};

template<typename T>
const Rational<T> operator* (const Rational<T>& lhs, const Rational<T>& rhs) {
    ...
}
```

或许我们奢望能够依靠和条款24相同的办法来解决模板化后的问题。

问题是不行，如果我们写出下面的代码：

```c++
Rational<int> onehalf(1,2);
Rational<int> result = onehalf * 2;
```

将会编译不通过。

原因在于在条款24内，第二行代码中的2可以通过implicit构造函数构造出一个临时的Rational对象。

比如对于下面的这段代码：

```c++
/**********************************************
  > File Name		: test16.cpp
  > Author			: lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Thu 01 Oct 2020 02:51:55 PM CST
 **********************************************/

#include <iostream>
using namespace std;

// 测试隐形转换调用构造函数

class Rational {
public:
    Rational(int nominator = 0, int denominator = 1):
    nominator(nominator),denominator(denominator)
    {
        cout << "call constructor" << endl;
        cout << "nominator = " << nominator << endl;
        cout << "denominator = " << denominator << endl;
    }
    int get_nominator() const {
        return nominator;
    }
    int get_denominator() const {
        return denominator;
    }
private:
    int nominator;
    int denominator;
};

const Rational operator*(const Rational& lhs, const Rational& rhs) {
    return Rational(lhs.get_nominator()*rhs.get_nominator(), lhs.get_denominator()*rhs.get_denominator());
}

int main() {
    Rational oneFourth(1,4);
    Rational result = 2 * oneFourth;
    cout << "result: " << result.get_nominator() << "/" << result.get_denominator() << endl;
    return 0;
}
```

其运行结果为：

```c++
call constructor
nominator = 1
denominator = 4
call constructor
nominator = 2
denominator = 1
call constructor
nominator = 2
denominator = 4
result: 2/4
```

可以看出，对于右值2，编译器还是自动调用了构造函数构造了一个临时的Rational对象进行运算。当然这是在非explicit的情况下完成的，如果我们给构造函数加上explicit关键字就无法通过编译了。

同样的代码我们给它模板化

```c++
/**********************************************
  > File Name		: test16.cpp
  > Author			: lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Thu 01 Oct 2020 02:51:55 PM CST
 **********************************************/

#include <iostream>
using namespace std;

// 测试隐形转换调用构造函数
//

template<typename T>
class Rational {
public:
    Rational(const T& nominator = 0, const T& denominator = 1):
    nominator(nominator),denominator(denominator)
    {
        cout << "call constructor" << endl;
        cout << "nominator = " << nominator << endl;
        cout << "denominator = " << denominator << endl;
    }
    const T get_nominator() const {
        return nominator;
    }
    const T get_denominator() const {
        return denominator;
    }
private:
    T nominator;
    T denominator;
};

template<typename T>
const Rational<T> operator*(const Rational<T>& lhs, const Rational<T>& rhs) {
    return Rational<T>(lhs.get_nominator()*rhs.get_nominator(), lhs.get_denominator()*rhs.get_denominator());
}

int main() {
    Rational<int> oneFourth(1,4);
    Rational<int> result = 2 * oneFourth;
    cout << "result: " << result.get_nominator() << "/" << result.get_denominator() << endl;
    return 0;
}
```

结果编译时会显示报错"no match for operator\*"，你无法指望编译器通过non-explicit构造函数将整数进行转化。

解决办法是将operator函数令为友元函数。声明为友元函数的好处就是当模板类Rational的类型被确定之后，operator\*的类型也确定了，这样就可以在调用时使用隐式转换函数了。

但是这个友元函数必须在类体内就被定义：

```c++
/**********************************************
  > File Name		: test16.cpp
  > Author			: lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Thu 01 Oct 2020 02:51:55 PM CST
 **********************************************/

#include <iostream>
using namespace std;

// 测试隐形转换调用构造函数
//

template<typename T>
class Rational {
public:
    Rational(const T& nominator = 0, const T& denominator = 1):
    nominator(nominator),denominator(denominator)
    {
        cout << "call constructor" << endl;
        cout << "nominator = " << nominator << endl;
        cout << "denominator = " << denominator << endl;
    }
    const T get_nominator() const {
        return nominator;
    }
    const T get_denominator() const {
        return denominator;
    }
    friend const Rational operator*(const Rational& lhs, const Rational& rhs) {
        return Rational(lhs.get_nominator()*rhs.get_nominator(), lhs.get_denominator()*rhs.get_denominator());
    }
private:
    T nominator;
    T denominator;
};

int main() {
    Rational<int> oneFourth(1,4);
    Rational<int> result = oneFourth * 2;
    cout << "result: " << result.get_nominator() << "/" << result.get_denominator() << endl;
    return 0;
}
```

否则会无法被连接器找到，这样就可以实现混合运算了。

再次解释一下这个代码为什么可以运行成功，因为它看起来和往常的友元函数看起来都不一样：之所以定义为友元函数，是因为为了让类型转换可以发生在所有实参身上；之所以将友元函数定义在内部，是为了使这个函数自动具现化。

所以请记住：当我们编写一个class template，而它所提供之“与此template相关的” 函数支持“所有参数之隐式类型转换”时，请将那些函数定义为**“class template 内部的friend函数"。**

#### 条款47: Use traits classes for information about types.

Traits 并不是一个关键字或一个预先定义好的构件；而是一种技术，一个C++程序员共同遵守的协议。Traits的作用是使你能够在编译期间取得某些类型信息。

C++11提供的\<type_traits\>库中提供了大量判断类型信息的函数，比如判断是否为指针的函数：

```c++
template<class T>
struct is_pointer;
```

返回一个struct，这个sturct的结构如下：

```c++
template<class T, T v>
struct integral_constant {
    static const T value = v;
    typedef T value_type;
    typedef integral_constant<T,v> type;
    operator value_type() {
        return value;
    }
}
```

可以通过判断`std::is_xxx::value`是否为true来判断目标类型是否为某种目标类型。

使用：

```c++
if (std::is_pointer<int*>::value)
```

注意在使用时是传入一个类型，而不是传入一个变量。

**判断两个类型是否相同**

```c++
if (std::is_same<int,int>::value)
```

**判断两个类型是否具有继承关系**

```c++
if (std::is_base_of<A,B>::value)
```

为true时表示B是A的子类。

**判断一种类型能否转换为另一种类型**

```c++
if (std::is_convertible<A*,B*>::value)
```

#### 条款48: Be aware of template metaprogramming.

Template metaprogramming(模板元编程)是编写template-based C++ program 并执行于compile-time的过程。

这样的结果是：较小的可执行文件、较短的运行期、较少的内存需求。并且一部分错误也可以在编译期就检查出来。

我们称那些在编译期就已经确定了的变量为编译期常量。有些编译器可以根据编译期常量进行优化，比如一个if语句，如果为true还是false在编译期就可以算出来，那么编译器就可以优化掉不会被选的那条路。这样就不用在运行期去判断一遍。当然，在编译期就可以得出结果的if语句还是比较少的。

常见的编译期常量包括：

-   数组的size。要想静态分配一个数组，那么数组的大小在编译期就必须知道。

    `int arr[size] = {};`中的size肯定是编译期常量。

-   模板中的编译期常量。

    模板参数除了是类型参数，还包括非类型参数。因此可以用来验证某个变量的类型。

    ```c++
    template<int i>
    struct S {};
    
    S<num> s;
    ```

    上面的代码便可以在编译期就验证num的类型是否为int

-   Case labels

    switch语句的每个case对应的变量也是在编译期就必须知道的。

**利用模板进行编译期运算**

考虑下面的例子

```c++
template<unsigned N>
struct Fibonacci;

template<>
struct Fibonacci<0> {
    static unsigned const value=0;
}

template<>
struct Fibonacci<1> {
    staitc unsigned const value = 1;
}

template<unsigned N>
strucxt Fibnoncci {
    static unsigned const value = Fibonacci<N-1>::value + Fibonacci<N-2>::value;
}
```

这就是模板元编程的雏形，这种编程方式被证实是图铃完备的。但编译器往玩会在递归的时候设置一个最大深度来避免无穷次的编译期递归。

**constexpr关键字**

`constexpr`的出现使得函数既能在编译期运行，又能在运行期运行。

对于constexpr修饰的函数，如果传进去的是编译期常量，就可以得到一个编译期常量。

比如，对于上面的代码就可以修改为

```c++
constexpr unsigned fibnoncci(unsigned i) {
    return (i <= 1u) ? i : fiboncci(i-1) + fiboncci(i-2);
}
```

constexpr修饰函数的限制：

在C++11中有着非常严格的限制，在C++14中放宽了许多。最严格的包括：

-   不能有try catch块
-   不能有static变量
-   不能有局部线程变量




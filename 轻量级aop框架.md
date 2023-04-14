---
author: lunar
date: Sat 24 Oct 2020 04:34:30 PM CST
location: Shanghai
---

### 使用C++11实现一个轻量级AOP框架

> 代码和思想来自《深入应用C++11》，代码放在最后。

#### 什么是AOP框架

AOP，即面向切面编程（Aspect Oriented Programming）。这是一种在面向对象的基础上进一步在代码之间解耦的编程思想。

书中以这样一个例子开头，对于每一个业务，都可以分为核心代码和辅助代码。核心代码，即某个特定对象或函数所需要完成的事情。而辅助代码，是没那么重要，甚至只是为了帮助开发者观察系统状态的代码，最常见的就是打印日志的代码。

辅助代码常常在不同的对象和函数中都体现为一个相同的模板，于是就需要进行解耦。我们希望能够将辅助代码像是一个切面一样可以直接插入核心代码之前或者之后或者两者都有。而这个插入的过程只需要一行代码就可以完成。

这就是面向切面编程的思想，有一种即插即用的美感。

#### 代码分析

在下面这个AOP框架中，虽然只有百来行代码。但是作者就C++11的特性（尤其是可变参数模板和右值引用，所以确保你有这两个的基础再来看代码）和AOP的思想体现的淋漓尽致。

首先，作者定义了一个宏，这个定义了一个struct（众所周知在C++里面struct和class没啥区别）。这个struct只有一个成员变量，是一个枚举，枚举只有一个值。

这个struct的作用其实就是用于检测某个类是否具有Before和After函数，这两个类分别在核心代码之前和之后起作用。在后面的类中，需要根据是否有定义这两个方法来决定是否进行调用，否则会编译报错。

`decltype` 是一种根据括号类型来得到一种类型，经常用作函数的后置返回类型。但很多人不知道的是：当decltype接收两个参数时，如果前面那个没有定义，则返回后面的默认类型。当然很多人说decltype并不是接收两个参数，而是一个表达式和一个返回类型。但我是作为接收两个参数理解的。

`declval` 返回某种类型的右值引用。

#### Aspect类

在Aspect类中定义了三个`Invoke`成员函数，分别对应三种情况：有Before没After,有After没Before和两者都有。

这个`Invoke`并不是重载，因为`enable_if`这个元函数决定了只有一个`Invoke`被定义。

随后才重载了一个`void Invoke`,这个模板函数负责接收切面，通过递归的方式解开可变参数模板的参数包。在递归之前调用每次递归的切面的Before方法，递归之后调用After方法。

两个重载函数的区别在于一个接收可变数量的切面，另一个只接收一个切面。所以在递归到只剩下最里层的那个切面时，会调用上面的`Invoke`。

上面的`Invoke`通过`enable_if`可以接收三种不同的切面，但是作者不知道为什么下面的`Invoke`没有延续这种做法，只定义了一个`Invoke`。这就导致只能是最里面的切面可以随意定义，外面几层的切面必须定义Before和After成员函数。

感兴趣的可以将接收多层切面的`Invoke`也改造成上面那种形式。

此外，这个AOP框架另一个最大的问题就是其核心代码的参数和切面的参数必须保持一致，这就导致缺少了很多实用价值。没办法，可变参数模板的递归调用其参数是共享的，你传入多个切面的话根本无法区分哪里到哪里是某个切面的参数。这是一个原理上的缺陷。

但是这个AOP框架的很多思想还是值得学习的。

附上原书源代码:

```c++
/**********************************************
  > File Name		: aop.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Wed 14 Oct 2020 11:53:12 PM CST
 **********************************************/

#include <functional>
// write an AOP framework with C++11

// Tip: when decltype receives two args, it returns the second argument when the first one is not compilable.
// This struct is to check if a class has the member function Before or After.
#define HAS_MEMBER(member)\
template<typename T, typename... Args> struct has_member_##member{\
private:\
    template<typename U> static auto Check(int) -> decltype(std::declval<U>().member(std::declval<Args>()...), std::true_type());\
    template<typename U> static std::false_type Check(...);\
public:\
    enum{value = std::is_same<decltype(Check<T>(0)), std::true_type>::value};\
};\

HAS_MEMBER(Foo)
HAS_MEMBER(Before)
HAS_MEMBER(After)

//#include <Noncopyable.hpp>
template <typename Func, typename... Args>
struct Aspect {
    Aspect(Func& f): m_func(std::forward<Func>(f)) {

    }

    template <typename T>
    typename std::enable_if<has_member_Before<T, Args...>::value && has_member_After<T, Args...>::value>::type Invoke(Args&&... args, T&& aspect) {
        aspect.Before(std::forward<Args>(args)...); //aspect before the core code
        m_func(std::forward<Args>(args)...); //core code
        aspect.After(std::forward<Args>(args)...); //aspect after the core code
    }

    template <typename T>
    typename std::enable_if<has_member_Before<T, Args...>::value && !has_member_After<T, Args...>::value>::type Invoke(Args&&... args, T&& aspect) {
        aspect.Before(std::forward<Args>(args)...);
        m_func(std::forward<Args>(args)...);
    }

    template <typename T>
    typename std::enable_if<!has_member_Before<T, Args...>::value && has_member_After<T, Args...>::value>::type Invoke(Args&&... args, T&& aspect) {
        m_func(std::forward<Args>(args)...);
        aspect.After(std::forward<Args>(args)...);
    }

    //to insert multiple aspects
    template <typename Head, typename... Tail>
    void Invoke(Args&&... args, Head&& headAspect, Tail&&... tailAspect) {
        headAspect.Before(std::forward<Args>(args)...);
        Invoke(std::forward<Args>(args)..., std::forward<Tail>(tailAspect)...);
        headAspect.After(std::forward<Args>(args)...);
    }

private:
    Func m_func;
};
template <typename T>
using identity_t = T;

template <typename... AP, typename... Args, typename Func>
void Invoke(Func&& f, Args&&... args) {
    Aspect<Func, Args...> asp(std::forward<Func>(f));
    asp.Invoke(std::forward<Args>(args)..., identity_t<AP>()...);
}

#include <iostream>
using namespace std;
struct AA {
    void Before() {
        cout << "AA Before" << endl;
    }

    void After() {
        cout << "AA After" << endl;
    }
};

struct BB {
    void Before() {
        cout << "BB Before" << endl;
    }

    void After() {
        cout << "BB After" << endl;
    }
};

void core_func() {
    cout << "core function called" << endl;
}

int main() {
    std::function<void()> f = std::bind(core_func);
    Invoke<AA, BB>(f);
    return 0;
}

```

---
author: lunar
date: Wed 23 Sep 2020 06:31:43 PM CST
---

### 左值引用与右值引用

左值：可以取地址的，有名字的，非临时的就是左值；

右值：不能取地址，没有名字的，临时的就是右值。

左值的英文缩写为"lvalue", 右值的英文缩写为"rvalue"。这并非很多人认为的"left value"和"right value"。而是"localtor value"和"read value"，分别表示可以在内存中找到的值和只能提供数值的值。

左值引用即我们常见的定义引用的方式，引用就相当于变量的别名。

左值引用要求右值必须能够取地址，如果不能取地址，则必须为常引用。

因为左值引用本质上是将地址赋给左值，所以下面的这种语句不能出现：
`int &b = a + 1;`

因为`a+1`不能被认为一个在内存中存在地址的变量，它只是一个数。当然如果加上const修饰符就可以了。

又或者下面这种:
```c++
int func() {
    return 0;
}

int &i = func();
```
函数`func()`的返回值也是右值，在内存中没有具体的地址。

那如果我们想要保存一个函数的返回值的话，要么通过常引用，要么不适用引用。前者导致引用不可修改，后者常常会带来大代价的拷贝函数调用。

如果我们即想引用可以修改，又想不带来内存拷贝的代价的话，就可以使用右值引用了。

右值引用的定义格式如下：
`类型 &&引用名 = 右值表达式;`

右值引用可以延长临时变量的生存周期，避免了无谓的内存复制操作。

#### 引用折叠

再来看一些稍微复杂的引用情况。

```c++
int x = 1;
int&& r1 = x;
auto&& r2 = r1;
```

问r2属于哪种类型的引用？

实际上，这种需要进行类型推断的“右值引用”并不是真的属于右值引用，具体属于哪一种引用需要通过初始化后进行决定，这之前只能称之为"universal references"。进行推断之后右值引用也可能变成左值引用。

这种情况称为**引用折叠**。其规则如下：

1.  所有的右值引用叠加到右值引用上仍然是一个右值引用；
2.  所有其他类型之间的叠加将会使得变成一个左值引用

对于上面这段代码，由于一开始r1将右值引用类型指向一个左值，所以r1就是一个左值引用。而r2属于对于左值引用的叠加，也是属于左值引用。

常见的其它的需要进行类型推导的还有：模板T，`decltype()`等。

#### 移动语义

需要注意的是，右值引用的右端并不能是左值，比如下面的操作是不被允许的：
```c++
int d = 1;
int &&r = d;
```

如果想要实现这样的操作，可以借助标准库里面的`move`函数。
```c++
int &&r = std::move(d);
```

你可能觉得这样做没有必要，明明使用左值引用就可以解决的事情，为什么还要用右值引用呢？

那是在你知道右端是左值的情况下。如果右端传入的数不确定是左值还是右值的话，`std::move`函数的应用场景就显现了，这样就省去了你判断右端是左值还是右值的功夫。

而且move几乎没有代价，只是转移了资源的控制权。尽量可以用。

#### forward和完美转发

>   本部分示例代码来源于《深入应用C++11》

看下面这个例子：

```c++
void func(int& i) {
    cout << "lvalue: " << i << endl;
}

void func(int&& i) {
    cout << "rvalue: " << i << endl;
}

void forward(int&& i) {
    func(i);
}

int main() {
    forward(1);
}
```

想必你已经可以猜到程序的运行结果了。由于经过了`forward`函数的一手中间转发，原本传入的右值因为具有了名称而被编译器当作左值处理。然后调用了左值为参数的`func`。

为了解决这个问题，C++推出了完美转发（Perfect Forwarding），即在函数模板中，完全按照模板的参数的类型（保持参数的左值、右值特征）进行参数的传递。

我们可以将上述代码修改为：

```c++
void func(int& i) {
    cout << "lvalue: " << i << endl;
}

void func(int&& i) {
    cout << "rvalue: " << i << endl;
}

void forward(int&& i) {
    func(std::forward<int>(i));
    //这个的效果与下面这个是一致的
    //但是下面这个始终只会调用第二个func
    func(std::move(i));
}

int main() {
    forward(1);
}
```

利用完美转发，我们可以写一个万能的函数wrapper

```c++
template<class Function, class... Args>
inline auto func_wrapper(Function&& f, Args&&... args) -> decltype(f(std::forward<Args>(args)...)) {
    return f(std::forward<Args>(args)...);
}
```

需要应用到可变数量的模板参数的知识。

#### emplace_back 减少内存拷贝和移动

我们知道，在C++11之前，往vector容器的底部添加一个元素可以使用push_back函数。现在我们多了一个选择：emplace_back

这两者的区别是什么呢？

首先我们要搞懂push_back向vector添加元素的原理：

对于下面这段代码：

```c++
/**********************************************
  > File Name		: test14.cpp
  > Author			: lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Thu 01 Oct 2020 09:22:03 AM CST
 **********************************************/

#include <iostream>
using namespace std;
#include <vector>

class Test {
public:
    Test(int num):num(num) {
        cout << "call constructor" << endl;
    }

    Test(const Test& a):num(a.num) {
        cout << "call copy constructor" << endl;
    }

    Test(Test&& a):num(a.num) {
        cout << "call move constructor" << endl;
    }
private:
    int num;
};

int main() {
    vector<Test> v;
    cout << "push_back" << endl;
    cout << "rvalue reference" << endl;
    v.push_back(10);
    cout << "lvalue reference" << endl;
    Test t(1);
    v.push_back(t);
}
```

如图，自己定义了class Test的拷贝构造函数和移动构造函数。再来看运行结果：

```c++
push_back
rvalue reference
call constructor //根据右值10构造一个右值Test对象
call move constructor //通过移动构造函数直接将对象传入vector
lvalue reference
call constructor 
call copy constructor //在将对象t作为参数传入时需要调用一次拷贝构造函数
call copy constructor //在push_back将t加入到vector时需要调用一次拷贝构造函数
```

根据运行结果易知，如果是传入右值还好，传入一个左值的代价是很大的。

除了选择将一个左值通过`std::move`函数转为右值传入外。c++11提供了一个新的函数:emplace_back。

emplace_back函数最大的特点在于其可以根据传入的对应对象的构造函数的参数自动创建一个对象放在vector尾部。比上面代码的右值传入还少了一个调用移动构造函数的过程。

但如果是传入一个左值的话，好像就和push_back没有什么区别了。

所有的标准库容器（array除外，因为它的长度不可改变，无法插入元素）基本都配备了类似的方法：emplace, emplace_hint, emplace_front, emplace_after, emplace_back。

我们应该尽量使用emplace来提高性能。当然这是C++11之后才出现的方法，如果要兼容以前的代码的话就没法使用了。以及，emplace是直接调用构造函数来构造对象，所以需要确保相应类或结构体有定义相应构造函数，否则会报错。


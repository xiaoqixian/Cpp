---
author: lunar
date: Fri 25 Sep 2020 02:44:31 PM CST
---

## C++ 智能指针

智能指针是C++11新引入的行为类似于指针的类对象，智能指针最大的特点是可以帮助管理动态内存分配的智能指针模板。

如果在一个类中定义了指针，我们知道在这个类不再使用时编译器会自动调用其析构函数以释放内存，所以该指针所占用的内存也会被释放。然而，该指针所指向的内存不会被释放，从而造成内存泄露。

解决的办法之一是在类的析构函数中写上所有动态分配内存的释放代码。而C++11的智能指针为我们提供了一种新的解决问题的办法：当类的析构函数被调用时，我们知道这个类中所声明的其它类型对象的析构函数也会被调用，从而释放其它对象内存。我们也想用这个功能来管理指针，然后指针只是指针，不是对象。所以C++11提供了对于指针的包装——智能指针。

#### 使用智能指针

C++提供了三种智能指针类型：`auto_ptr,unique_ptr,shared_ptr`

三种类型的智能指针的定义方式都类似与下面这种：

```c++
std::auto_ptr<std::string> ps(new std::string("str"));
```

而`ps`的使用方式也和普通指针一样。

#### shared_ptr的基本用法

**初始化**

可以通过构造函数、std::make_shared\<T\>辅助函数和reset方法来初始化shared_ptr。

```c++
std::shared_ptr<int> p(new int(1));
std::shared_ptr<int> p2;
p2.reset(new int(1));
```

**获取原始指针**

可以通过get方法来获取原始指针。

**指定删除器**

```c++
void deletePtr(int* p) {
    delete p;
}
std::shared_ptr<int> p(new int(1), deletePtr);
```

也可以直接使用lambda表达式来制定删除器：

```c++
std::share_ptr<int> p(new int(1), [](int* p){delete p;});
```

当使用shared_ptr管理数组指针时，必须要指定删除器，因为默认的删除器不支持删除数组对象。

```c++
std::share_ptr<int> p(new int[10], [](int* p){delete[] p;});
```

**返回this指针的注意事项**

我们在返回一个this指针的时候，如果希望能够通过shared_ptr进行包装，不要直接包装this指针后返回。即下面这种示例：

```c++
struct A {
    shared_ptr<A> getSelf() {
        return shared_ptr<A>(this);
    }
};
shared_ptr<A> sp1(new A);
shared_ptr<A> sp2 = sp1->getSelf();
```

这样的两个智能指针都是通过this裸指针构造的，所以两者毫无关系，会导致重复析构。正确的做法为继承std::enable_shared_from_this类，然后调用成员函数shared_from_this

```c++
class A: public std::enable_shared_from_this<A> {
    std::shared_ptr<A> getSelf() {
        return shared_from_this();
    }
};
```

#### unique

#### 有关智能指针的注意事项

现在一般不建议使用`auto_ptr`，编译使用了`auto_ptr`的代码编译器甚至会给出警告，为什么呢？

对于下面的语句：

```c++
auto_ptr<string> ps(new string("str"));
auto_ptr<string> vocation;
vocation = ps;
```

如果编译运行这段代码会发现有 "free(): invalid pointer" 的报错。这是因为，这样会有两个智能指针指向同一个string对象，就会导致两次string对象的释放。

解决这样的办法有多种策略：

1. 重定义`=`运算符，使得`=`时会自动构造一个副本；
2. 建立所有权概念，在`vocation = ps`时`vocation`将剥夺`ps`对于string对象的指针所有权。保证只有一个智能指针对象管理一个指针。这就是`unique_ptr`的策略；
3. 引进垃圾回收中计数的概念，当有新的智能指针管理同一个指针时，计数器加1，当有智能指针被回收时，相应的计数器减1。只有在计数器减到0后，该指针指向的内存才被回收。这就是`shared_ptr`的策略。

但是如果你使用`unique_ptr`重新写一遍上述代码时，发现还是无法实现。这时因为虽然`vocation`虽然接管了string对象的所有权，但是`ps`还是有可能使用，而`ps`不再指向有效的数据，带来了潜在的风险。

因此，编译器规定：**如果等式的右端是一个右值的话，这种操作是成立的，因为右值是临时的，很快就会被销毁，不会被别人使用。如果是左值的话，这种操作将不被允许。**

**注意：**使用 new 分配内存时，才能使用`auto_ptr`(当然这个最好也不要用)和`shared_ptr`，使用 new[] 分配内存时，要使用`unique_ptr`。

#### weak_ptr

weak_ptr实际上并不是一种智能指针，其常用于搭配shared_ptr一起使用。weak_ptr绑定的对象并不是一个指针，而是一个shared_ptr。将weak_ptr绑定一个shared_ptr并不会增加shared_ptr的计数，并且即使有wrak_ptr指向，这块内存也可以被销毁，这种绑定关系称为弱共享。相应的，也无法通过weak_ptr访问到这块内存。

那么weak_ptr有什么用呢？

weak_ptr更多的是作为shared_ptr的一个观察者的身份存在。

**weak_ptr的实例化**

weak_ptr接收一个shared_ptr作为参数进行实例化。

**weak_ptr判定一个对象是否被释放**

weak_ptr的成员函数lock可以用于判定对象是否存在，如果存在，返回一个指向共享对象的shared_ptr，否则返回一个空的shared_ptr。

**weak_ptr会延长shared_ptr计数器生命周期**

注意哦，这里说的是shared_ptr计数器的生命周期。相应对象的生命周期还是在计数器归零的时候就完结了。这里的目的是为了保证weak_ptr向计数器索取绑定数目时不会指向一个被释放的内存而导致错误。

所以，只要还有指向该shared_ptr的weak_ptr存在，相应的shared_ptr就不会被释放。

#### shared_ptr 内存分配探秘

前面讲到shared_ptr通过计数器来判定内存引用数。但这个计数器并不是定义在shared_ptr内部的（废话，要不然怎么能被这么多shared_ptr共享）。这个计数器其实是定义在堆上的。

当一块内存第一次被shared_ptr绑定时，编译器会在堆上创建一个计数器，这个计数器可以被所有的shared_ptr所共享。

这就产生了一个问题，对于下面这段代码：

```c++
auto* ptr = new Object();
shared_ptr<Objet> sp{ptr};
```

这段代码其实涉及到两次内存分配，一次分配Object，另一次分配shared_ptr。这其实是不够高效的。所以我们万能的C++11准备了std::make_shared函数，这个函数使得这两个过程合二为一，这个函数可以将两者的内存视作一个整体进行管理。

make_shared函数的语法格式为

```c++
template <class T, class... Args>
  shared_ptr<T> make_shared (Args&&... args);
```

所以上面的代码可以修改为

```c++
shared_ptr<Object> sp = make_shared();
```

使用make_shared有很多好处，首先就是减少了内存分配的次数，提高了效率。其次是两个内存放在一起减少了内存查找的次数（Cache查找的原理）。最后就是使得指针被创建之后立刻被智能指针绑定了，有效地减少了内存泄漏的可能。很多时候的内存泄漏都是因为指针被创建出来后还没来得及被智能指针绑定就抛出了异常，导致指针没人回收。

**shared_ptr坏处**

>   内容来源于知乎文章《如何优雅地构造shared_ptr》

凡事有好有坏，make_shared最大的一个坏处就是由于其对象内存和shared_ptr计数器内存都被绑定在一起。导致释放时也只能一起释放，而我们从上文知道：shared_ptr的计数器在被weak_ptr绑定的情况下并不随其指向对象一起被销毁。这就导致了下面这种情况：

当有很多weak_ptr指向shared_ptr时，计数器必须等待最后一个weak_ptr被释放自己才能释放。而计数器与对象的内存是共同管理的，所以对象的内存也无法得到释放。如果存在weak_ptr一直没有释放的话，简直相当于内存泄漏了。

所以make_shared函数不能乱用。当你需要绑定的对象比较大，并且可能有很多weak_ptr需要绑定的时候，还是不要使用这个函数了。

#### 各种智能指针类型的使用场景

> 下面各种观点是从各种博客和知乎回答搜集而来

首先如果是全局对象的话，完全可以使用纯指针，反正在程序运行期间内存都不会被释放。

对于`unique_ptr`，其内存几乎与纯指针一样大。但是不能被复制，如果对象不需要创建副本的话，尽量使用`unique_ptr`。

对于`shared_ptr`，其使用计数器，可以多个变量共享指针。缺点是内存起码是纯指针的两倍。同时在多线程的情况下，在计数器的操作上涉及到原子操作，增加CPU的负荷。

#### 为什么即使使用了智能指针依然存在内存泄露问题？

知乎有这样一个问题：C++ 的智能指针不就基本解决了野指针问题了吗?为什么还要吹捧rust的内存安全?

我从各个回答总结出以下几点：

1. 要非常清楚智能指针的生命周期

    或者说，要尽量遵循RAII的设计规范：对于已经被封装的东西，在封装范围之外尽量使用封装的对象。

    比如下面的这个例子：

    ```c++
    void process(std::shared_ptr<int> svp) {}
    int main(int argc, char** argv) {
      int* vp = new int(10);
      process(std::shared_ptr<int>(vp));
      std::cout << *vp << std::endl;  // pointer "vp" has already been released.
      return 0;
    }
    /*
    作者：Jason于航
    链接：https://www.zhihu.com/question/400093693/answer/1270543164
    来源：知乎
    著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
    */
    ```

    这个例子中使用智能指针基本只作为临时变量，因此当该临时变量被回收时，指针指向的内存也一齐被回收了。

    这个既可以说是不清楚智能指针的生命周期，也可以说不遵循封装对象的使用规范。

2. 循环引用问题

    所有通过计数器进行内存回收的算法都免不了一个问题：循环引用。

    所以`shared_ptr`也存在这个问题，在使用时就要特别注意。

3. 不当用法造成的潜在内存泄露

    ```c++
    bool complicatedCompute() { /* ... */ return true; }  // potential memory leak;
    auto process(std::shared_ptr<int>, bool) {}
    int main(int argc, char** argv) {
      process(std::shared_ptr<int>(new int(10)), complicatedCompute());
      return 0;
    }
    /*
    作者：Jason于航
    链接：https://www.zhihu.com/question/400093693/answer/1270543164
    来源：知乎
    著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
    */
    ```

    这个问题在《Effective C++》一书中也提到过，可能一开始还难以看出问题。问题出在`process`函数的参数上：因为这个传参不是直接传入一个变量，中间还要进行多步操作，每一步都有可能产生异常。造成动态内存的分配和与智能指针的绑定之间存在隔阂，带来潜在的内存泄露。

暂时就补充这么多吧，以后可能会继续更新，毕竟C++内存安全实在防不胜防。
---
author: lunar
date: Tue 13 Oct 2020 06:43:01 PM CST
---

## C++11 多线程开发

C++11之前，C++对于并发编程并没有提供语言级别的支持。只能像C语言那样使用操作系统提供的POSIX提供的多线程库pthread。

C++11之后，C++通过一系列语法支持使得多线程开发变得更容易。

温馨提示：如果你是在命令行通过g++编译，需要添加-lpthread参数链接pthread库。否则会报"undefined reference pthread_create"的错。

### 线程

首先复习一下操作系统的部分知识：
- 一个进程内的多个线程共享进程的地址空间、内存资源。
- 每个线程所单独拥有的仅为：线程状态、寄存器、程序计数器、堆栈。

#### 线程的创建

C++11中线程的创建非常简单，就是调用std::thread的构造函数。

```c++
#include <thread>

void func() {
    //do something
}

int main() {
    std::thread t(func);
}
```

**线程对象创建即启动**，并需要像java那样通过一个start函数进行启动。

如果func是一个有参函数的话，func后面还可以添加func的参数。

当然，构造函数的参数并不只能是函数，任何可调用对象包括lambda表达式都可以作为参数构造线程。

**注意**：这样创建的线程最大的缺点就是新建线程的生命周期不得长于std::thread变量的生命周期。否则std::thread变量在出了作用域后会自动调用析构函数，导致新建线程退出。

#### 线程的阻塞和分离

在运行一个线程后，如果我们阻塞当前线程来等待新建线程运行完成。

```c++
int main() {
    std::thread t(func);
    t.join();
}
```

这样，主线程就会被阻塞，直到func函数调用返回才继续运行。

我们也可以通过detach函数让新建线程与当前线程进行分离，让新建线程到后台运行。新建线程与主线程分离后其生命周期自然也不再受到std::thread生命周期的限制。

```c++
int main() {
    std::thread t(func);
    t.detach();
    //keep running
}
```

#### 线程的移动

通过std::move函数可以移动一个线程变量

```c++
std::thread t(func);
std::thread t1(std::move(t));
```

移动之后，t变量无法再使用。

#### 获取当前线程

通过std::this_thread变量可以指向当前线程。

### 互斥量

互斥量是一种用于线程同步的手段。

4种语义的互斥量被提供:
- std::mutex
- std::timed_mutex: 带超时的互斥量
- std::recursive_mutex: 可递归互斥量
- std::recursive_timed_mutex

这里解释一下，什么叫可递归互斥量。

可递归互斥量，其实就是允许一个线程多次获取互斥量的互斥量。因为在一般的互斥量中，如果一个线程在已经获取了互斥量的情况下再次阻塞式地获取互斥量就会导致死锁。

而可递归互斥量允许多次获取互斥量的行为，避免了死锁。

#### 互斥量的使用

由于四种互斥量都提供了接近统一的接口

- 通过lock()函数阻塞式地获取互斥量。如果是设置的超时互斥量，则超过时间后就不会再阻塞。
- 通过unlock()函数释放互斥量
- 通过try_lock()函数非阻塞式地获取一个互斥量。如果成功获取就返回true，否则返回false。

#### 通过lock_guard类型进行互斥量的统一管理

与指针类似，互斥量这种需要手动获取和释放的资源可以通过RAII(Resource Acquisition is Initialization)技术进行统一管理。

lock_guard的初始化方法为：
```c++
std::lock_guard<std::mutex> locker(g_lock); //g_lock是std::mutex类型变量
```

locker变量出了作用域后就会自动释放互斥量。

### 条件变量

条件变量是另一种同步机制，经常与互斥量配合使用。

考虑这样一种场景，某一条线程需要临界资源满足某一种条件才对临界资源进行操作，如 抢票系统要在票卖完后进行加票。则加票的线程需要一直进行互斥锁的抢占，抢占后只是 检查票是否卖完了，但大部分时间是没卖完的，所以多了很多不必要的抢占。而在票真正 卖完后，加票线程又可能需要抢占多轮才能抢到互斥锁，导致程序的延误。这些都是只用互斥锁的程序很难解决的。

而如果我们引入条件变量，则可以转变下面这种场景：如果是取票线程抢占到了互斥量，则检查是否没票了，如果是，则令其等待一个条件变量m_notEmpty。如果是放票线程抢占到了互斥量，则检查是否还有票。如果是，则令其等待另一个条件变量m_empty。

为此，取票和放票的线程可以定义为：

```c++
void put(const T& x) {
    std::lock_guard<std::mutex> locker(m_mutex);
    while (!is_empty()) {
        cout << "票还有，等待..." << endl;
        //wait函数会阻塞当前线程，使得当前线程不参与抢占。
        m_empty.wait(m_mutex);
    }
    pool.push_back(x);
    //下面这个函数会唤醒所有等待m_notEmpty条件变量的线程
    m_notEmpty.notify_all();
}

void take(const T& x) {
    std::lock_guard<std::mutex> locker(m_mutex);
    while (is_empty()) {
        cout << "没票了，等待..." << endl;
        m_notEmpty.wait(m_mutex);
        m_empty.notify_one(); //唤醒放票线程
    }
    x = pool.front();
    pool.pop_front();
}
```

由这个例子看来，条件变量是用于当前线程主动放弃抢占的工具。当我们知道在不满足某些条件时，即使抢占到了互斥量也没用。这时该线程可选择主动阻塞，并联系到一个条件变量上。这样，其他没有阻塞在进行一系列操作后可以通过notify_one或者notify_all方法将当前线程重新唤醒。

wait函数也可以这样写
```c++
m_noEmpty.wait(m_mutex, [this]{return !is_empty();});
```

### 原子变量

C++11提供了一个原子变量类型 std::atomic\<T\>。

通常在修改一个变量时包含了三个步骤：读取，修改，回写。

在并发条件下，可能这三个步骤中掺杂了其他线程的操作。而原子变量可以保证这三个步骤在一个线程内同时完成。避免了互斥量的使用。

### 异步操作函数async

有时你可能需要通过另一个线程来完成一个简单的工作，但是又不想煞费苦心地创建一个线程。并且你可能还希望这个线程能够返回一个结果给你。

于是C++11给你提供了std::async函数，这个函数的使用就像调用一个普通函数一样，只是这个函数由另一个线程去执行。

函数原型为
```c++
std::future res = std::async(std::launch::async|std::launch::deferred|std::launch, func, args...);
```

第一个参数表示创建的时机，std::launch::async(默认)表示在调用async函数时就创建线程。std::launch::defeerred表示在返回值调用get或wait成员函数才创建线程。

第二个参数自然是函数或者说可调用对象都可以，第三个参数表示函数参数。

现在说一下返回值，返回值类型是std::future。future表示这个结果只有在未来的某时刻才会求出。future除了包含了线程的返回值外，还包含了线程的运行状态。包括三种状态：

- std::future_status::deferred: 异步操作还没开始
- std::future_status::ready: 异步操作已经完成
- std::future_status::timeout: 异步操作超时

#### 通过wait_for启动一个线程

前面说过，async函数运行的线程可以等到返回值调用wait_for或wait函数或get函数调用时再创建。

wait和wait_for函数的作用都差不多，就是阻塞当前线程来等待future对应的线程的运行结果。区别就是wait_for函数可以设置超时时间。

需要注意的是future类型变量不可被拷贝，只能被移动。

另外，async函数还是与协程有挺大的区别。协程是创建一个线程并将自己的CPU时间让给它，而async函数只是创建一个线程放到后台运行。不过听说C++20也开始支持协程了。

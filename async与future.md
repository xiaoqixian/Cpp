---
author: lunar
date: Fri 09 Oct 2020 06:21:14 PM CST
---

### C++ 异步编程

虽说C++也可以通过创建线程的方式来进行异步编程，但是过程较为繁琐。而std::async可以替你完成从创建线程，到绑定函数甚至到调用的三步过程。

std::async的原型为
```c++
async(std::launch::async|std::launch::deferred, func, args...)
```

第一个参数表示线程的创建时机：
- std::launch::async 在调用async时就创建线程;
- std::launch::deferred 调用future的get或wait时才创建线程

async函数的返回值是future类型，future包含了`get`和`wait_for`等方法。



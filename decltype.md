---
author: lunar
date: Thu 15 Oct 2020 07:57:17 PM CST
---

### decltype推导规则

decltype(exp)的推导规则如下：
1. exp是标识符、类访问表达式，decltype(exp)与exp的类型一致；
类似于下面这两种：
```c++
int n = 1;
volatile const int& x = n;
M::data; //static member in M class
```
分别属于标识符表达式和类访问表达式。
2. exp是函数调用，decltype(exp)与返回值的类型一致；
3. 若exp是一个左值，则decltype(exp)是一个左值引用；其它情况下与exp类型一致。

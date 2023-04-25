### 从模板偏特化看enable\_if的实现

​	`enable_if<bool, T>` 是\<type_traits\> 下的一个模板元编程工具，其可以通过第一个模板参数bool值是否为true来决定该模板函数/类是否被编译，第二个参数则是附带的类型，可以通过`enable_if<bool,T>::type` 获取，不使用的话可以为void。

​	`enable_if` 本质上是一个结构体，并且定义非常简单：

```c++
template <bool B, typename T = void>
struct enable_if {};

template <typename T>
struct enable_if<true, T> {typedef T type;};
```

​	要介绍`enable_if`的实现原理，需要先介绍模板偏特化。

#### 模板偏特化

​	模板偏特化是一种类似于多态的技术，我们可以通过定义不同的函数参数来区分同一个名字的函数，类似的，我们可以通过不同的模板参数区分多个模板（不同模板可以有不同的实现）。但不同的是，多态函数的签名可以毫不相关，但是对于模板而言，除了第一个声明的模板，后面的模板都必须是第一个模板的特例，称为特化。

1. 首先确定一个最泛化的模板（为了满足需要，模板参数只能多不能少）

   以三个模板参数为例：

   ```c++
   template <typename, typename, typename>
   struct A {
       void func() {
           printf("declaration1\n");
       }
   };
   ```

2. 接着定义A的特化，以两个模板参数为例：

   ```c++
   template <typename T, typename U>//这里的模板参数不再重要，只是指示T和U是两个泛型
   struct A<bool, T, U> {//这里的特化方式更加重要，这里将第一个类型特化为bool值
       void func() {
           printf("declaration2\n");
       }
   };
   ```

   **这时候定义的模板参数就已经不再重要，只需要看对于A的特化方式**。不管是多少个模板参数，只要特化方式一致， 编译器就会看作同一个声明。

   ```c++
   template <typename T, typename U, typename K, typename V>//与前面的声明本质相同，如果同时出现会报错。
   struct A<bool, T, U> {
       void func() {
           printf("declaration2\n");
       }
   };
   ```

3. 出现存在偏特化的类型对象的声明或者函数的调用时，编译器会优先匹配所有的特化模板，如果都不能匹配才会匹配第一个泛化模板，还不能匹配就会报错。

   所以对于上面的类型A，声明对象并进行调用：

   ```c++
   A<bool, int, double> a;//第一个参数为bool，优先匹配特化模板
   a.func();//调用特化模板的函数实现，输出declaration2
   ```

​	需要强调的是，特化并不仅仅出现于对于某一个泛型的类型特化，也可以是对于某个具体类型的值特化。

​	回到 `enable_if` 的实现，`enable_if` 结构体的主模板定义为：

```c++
template <bool, typename T = void>
struct enable_if {};
```

主模板并没有定义任何东西，这时对于第一个bool值进行值特化：

```c++
template <typename T>
struct enable_if<true, T> {typedef T type;};
```

因此当出现 `enable_if<true, A>` 之类的类型时，编译器会根据特化选择下面的定义，因此可以通过 `type` 获取类型T。当bool值非true时，根据SFIANE原则，获取type值所在的函数或类型不会被编译，因此实现了根据bool值来决定类/函数是否编译的效果。

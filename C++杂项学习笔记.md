### C++杂项学习笔记

1.  new对于内置类型和自定义类型的区别

    对于内置类型而言，new仅仅是分配内存。除非在后面加上()，否则不会调用它的构造函数。因此相应的值是随机值。

    而对于自定义类型，不管是否加上()，都会调用其构造函数。

    此外，内置类型的变量，它的初始值由定义的位置决定。定义在函数外的变量被初始化为0，定义在函数体内的变量不被初始化，为随机值。

2.  静态变量，全局变量和局部变量内存释放的先后性 

    有如下测试代码：

    ```c++
    /**********************************************
      > File Name		: test25.cpp
      > Author		    : lunar
      > Email			: lunar_ubuntu@qq.com
      > Created Time	: Sun 11 Oct 2020 10:34:21 AM CST
     **********************************************/
    
    #include <iostream>
    using namespace std;
    
    //测试全局变量和静态变量释放内存的先后顺序
    
    class T {
    public:
        T(string s):s(s) {
    
        }
        ~T() {
            cout << s << " call destructor" << endl;
        }
    private:
        string s;
    };
    
    static T e("e");
    T a("a");
    T b("b");
    static T f("f");
    
    int main() {
        T g("g");
        static T c("c");
        static T d("d");
        T h("h");
        return 0;
    }
    static T i("i");
    ```

    运行的结果为

    ```c++
    h call destructor
    g call destructor
    d call destructor
    c call destructor
    i call destructor
    f call destructor
    b call destructor
    a call destructor
    e call destructor
    ```

    从运行结果可以看出，对于单一类型的变量。其结果都是后定义的变量先释放，类似与栈的效果。对于不同类型的变量，其结果是局部变量被首先释放，而全局变量和静态变量没有什么先后性，哪个后定义哪个就先被释放。

3.  区分深/浅拷贝和赋值操作

    ```c++
    #include<iostream>
    using namespace std;
    class MyClass
    {
    public:
        MyClass(int i = 0)
        {
            cout << i;
        }
        MyClass(const MyClass &x)
        {
            cout << 2;
        }
        MyClass &operator=(const MyClass &x)
        {
            cout << 3;
            return *this;
        }
        ~MyClass()
        {
            cout << 4;
        }
    };
    int main()
    {
        MyClass obj1(1);
        MyClass obj3 = obj1;
        return 0;
    }
    ```

    对于这段代码，可能有很多人认为obj3执行的是赋值操作。但其实它执行的浅拷贝操作，所以调用的是拷贝构造函数。

    只有当代码如下定义时：

    ```c++
    MyClass obj3;
    obj3 = obj1;
    ```

    才会执行赋值操作，才会调用重载了=的函数。
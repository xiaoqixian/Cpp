/**********************************************
  > File Name		: test28.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Thu 15 Oct 2020 11:24:46 PM CST
 **********************************************/

#include <iostream>
using namespace std;

// 可变参数模板有多个参数包测试

template <typename... AP, typename... Args, typename T>
void func(T t, AP... ap, Args... args) {
    cout << t << endl;
    cout << "sizeof args: " << sizeof...(args) << endl;
    cout << "sizeof AP: " << sizeof...(ap) << endl;
}

struct A {

};

struct B {};

int main() {
    A a;
    B b;
    func<A, B>(12,a,b,12,13);
    return 0;
}

/**********************************************
  > File Name		: test10.cpp
  > Author			: lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Thu 24 Sep 2020 12:04:07 AM CST
 **********************************************/

#include <iostream>
using namespace std;

// 虚函数测试

class Base {
public:
    virtual void func(char c) {
        cout << "from base" << c << endl;
    }
};

class Derived: Base {
public:
    virtual void func(int i) {
        cout << "from Derived" << i << endl;
    }
};

int main() {
    Derived d;
    d.func('c');
    return 0;
}

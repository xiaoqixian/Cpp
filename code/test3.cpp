/**********************************************
  > File Name		: test3.cpp
  > Author			: lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Wed 16 Sep 2020 03:48:56 PM CST
 **********************************************/

#include <iostream>
using namespace std;

class Base {
public:
    void func(int x) {
        cout << "base func" << endl;
    }
};

class Derived: public Base {
public:
    using Base::func;
    void func() {
        cout << "derived func" << endl;
    }
};

int main() {
    Derived d;
    d.func(1);
    return 0;
}

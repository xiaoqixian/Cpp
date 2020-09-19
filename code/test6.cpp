/**********************************************
  > File Name		: test6.cpp
  > Author			: lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Sat 19 Sep 2020 04:36:04 PM CST
 **********************************************/

#include <iostream>
using namespace std;

class Base {
public:
    virtual void func(int d = 0) {
        cout << "base" << endl;
        cout << d << endl;
    }
};

class Derived: public Base {
public:
    virtual void func(int d = 1, int k = 2) {
        cout << "derived" << endl;
        cout << d << endl;
        cout << k << endl;
    }
};

int main() {
    Derived d;
    d.func();
    return 0;
}

/**********************************************
  > File Name		: test5.cpp
  > Author			: lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Sat 19 Sep 2020 04:20:31 PM CST
 **********************************************/

#include <iostream>
using namespace std;

class Base {
private:
    int d = 2;
public:
    virtual void func() {
        cout << "d = " << d << endl;
    }
};

class Derived:public Base {
private:
    int d = 4;
public:
    virtual void func() {
        cout << "d = " << d << endl;
    }
};

int main() {
    Derived d;
    d.func();
    Base* b = new Derived();
    b->func();
    return 0;
}

/**********************************************
  > File Name		: test4.cpp
  > Author			: lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Sat 19 Sep 2020 03:03:06 PM CST
 **********************************************/

#include <iostream>
using namespace std;

class Base {
private:
    virtual void func() {
        cout << "base private virtual function" << endl;
    }
public:
    void wrapper() {
        cout << "wrapper from base" << endl;
        func();
    }
};

class Derived: public Base {
private:
    virtual void func() {
        cout << "Derived private virtual function" << endl;
    }
};

int main() {
    Base* b = new Derived();
    b->wrapper();
    return 0;
}

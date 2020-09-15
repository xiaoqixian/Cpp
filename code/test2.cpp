/**********************************************
  > File Name		: test2.cpp
  > Author			: lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Tue 15 Sep 2020 07:41:23 PM CST
 **********************************************/

#include <iostream>
using namespace std;

class Test {
public:
    Test() {
        cout << "Test constructor was called" << endl;
    }

    virtual void func() {
        cout << "Test func" << endl;
    }
};

class CTest: public Test {
public:
    CTest() {
        cout << "CTest construct was called" << endl;
    }

    virtual void func() {
        static_cast<Test>(*this).func();
        cout << "CTest func" << endl;
    }
};

int main() {
    CTest().func();
    return 0;
}

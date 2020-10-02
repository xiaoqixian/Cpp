/**********************************************
  > File Name		: test15.cpp
  > Author			: lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Thu 01 Oct 2020 01:53:57 PM CST
 **********************************************/

#include <iostream>
using namespace std;
#include <memory>

// 测试weak_ptr是否可以延长shared_ptr的生命周期

class A {
public:
    A(int num):num(num) {
        cout << "call constructor" << endl;
    }
    ~A() {
        cout << "call destructor" << endl;
    }
private:
    int num;
};

void func() {
    weak_ptr<A> wp;
    A* a = new A(12);
    if (1) {
        shared_ptr<A> sp(a);
        wp = sp;
    } else {
        
    }
    cout << "func end" << endl;
    if (wp.lock()) {
        cout << "instance not deleted" << endl;
    }
}

int main() {
    func();
}

/**********************************************
  > File Name		: test14.cpp
  > Author			: lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Thu 01 Oct 2020 09:22:03 AM CST
 **********************************************/

#include <iostream>
using namespace std;
#include <vector>

class Test {
public:
    Test(int num):num(num) {
        cout << "call constructor" << endl;
    }

    Test(const Test& a):num(a.num) {
        cout << "call copy constructor" << endl;
    }

    Test(Test&& a):num(a.num) {
        cout << "call move constructor" << endl;
    }
    ~Test() {
        cout << "call destructor" << endl;
    }
private:
    int num;
};

int main() {
    vector<Test> v;
    cout << "push_back" << endl;
    cout << "rvalue reference" << endl;
    int i = 10;
    v.emplace_back(i);
    cout << "lvalue reference" << endl;
    Test t(1);
    v.emplace_back(t);
    cout << "end" << endl;
}

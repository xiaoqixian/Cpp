/**********************************************
  > File Name		: test11.cpp
  > Author			: lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Fri 25 Sep 2020 02:53:58 PM CST
 **********************************************/

#include <iostream>
using namespace std;

// 析构函数测试
// 证明一个类的析构函数会调用另一个类的析构函数

class T1 {
public:
    ~T1() {
        cout << "destructor of T1" << endl;
    }
};

class T2 {
private:
    T1 t1;
public:
    ~T2() {
        cout << "destructor of T2" << endl;
    }
};

int main() {
    T2* t2 = new T2();
    delete t2;
    return 0;
}

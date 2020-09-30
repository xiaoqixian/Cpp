/**********************************************
  > File Name		: test13.cpp
  > Author			: lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Wed 30 Sep 2020 09:28:48 PM CST
 **********************************************/

#include <iostream>
using namespace std;

void func(int& i) {
    cout << "lvalue: " << i << endl;
}

void func(int&& i) {
    cout << "rvalue: " << i << endl;
}

void forward(int&& i) {
    func(std::forward<int>(i));
}

int main() {
    forward(1);
}

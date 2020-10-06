/**********************************************
  > File Name		: test20.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Sat 03 Oct 2020 03:42:06 PM CST
 **********************************************/

#include <iostream>
using namespace std;

//编译期常量测试

constexpr int func() {
    cout << "func" << endl;
    return 1;
}

int main() {
    cout << "main function" << endl;
    int d = 0;
    switch (d) {
    case func():
        break;
    default:
        break;
           
    }
}

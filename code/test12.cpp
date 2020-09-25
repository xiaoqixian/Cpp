/**********************************************
  > File Name		: test12.cpp
  > Author			: lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Fri 25 Sep 2020 04:06:07 PM CST
 **********************************************/

#include <iostream>
#include <memory>
using namespace std;

// 三种智能指针测试


int main() {
    int i = 10;
    int* p = &i;
    unique_ptr<int> ap(p);
    unique_ptr<int> vocation;
    vocation = ap;
    cout << *ap << endl;
    cout << *vocation << endl;
    return 0;
}

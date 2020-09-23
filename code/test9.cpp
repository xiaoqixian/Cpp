/**********************************************
  > File Name		: test9.cpp
  > Author			: lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Wed 23 Sep 2020 07:09:24 PM CST
 **********************************************/

#include <iostream>
using namespace std;

// 左值引用与右值引用测试

int func() {
    return 4;
}

int main() {
    // 左值引用无法引用右值
    //string &s = func();
    //cout << s << endl;
    
    // 需要注意的是，右值引用并不能用左值赋值
    int d = 1;
    int && r = std::move(d);
    cout << r << endl;
    cout << d << endl;
    r = 2;
    cout << r << endl;
    cout << d << endl;
    
    int &&e = std::move(func());
    cout << e << endl;
    return 0;
}

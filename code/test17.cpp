/**********************************************
  > File Name		: test17.cpp
  > Author			: lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Fri 02 Oct 2020 03:44:18 PM CST
 **********************************************/

#include <iostream>
using namespace std;

//友元函数定义在类内部和外部的测试

class Test {
public:
    friend void func(Test t);
};
void func(Test t) {
    cout << "with parameter of T" << endl;
}

int main() {
    Test t;
    func(t);
    return 0;
}

//结论：如果友元函数在内部声明并且定义
//如果不带有类型Test的参数，则无法通过编译，显示这个函数没有被定义
//如果带有类型Test的参数，则可以通过编译。

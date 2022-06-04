/**********************************************
  > File Name		: test.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Thu 02 Jun 2022 09:52:17 PM CST
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/

class A {
public:
    typedef int function_type(int);
};

int func(int) {
    return -1;
}

#include <functional>
int main() {
    std::function<A::function_type> f = func;
}

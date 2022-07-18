/**********************************************
  > File Name		: bind.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Wed 01 Jun 2022 09:06:33 PM CST
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/

#include <functional>
#include <iostream>

void func(int a, int b) {
    std::cout << "a = " << a << ", b = " << b << std::endl;
}

int main() {
    auto switch_param = std::bind(func, std::placeholders::_2, std::placeholders::_1);
    switch_param(1, 2);

    auto partial_spec = std::bind(func, 1, std::placeholders::_1);
    partial_spec(2);
}

/**********************************************
  > File Name		: forward.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Wed 01 Jun 2022 11:32:04 AM CST
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/

#include <stdio.h>
#include <utility>

template<typename T>
void printT(T& t) {
    printf("lvalue\n");
}

template<typename T>
void printT(T&& t) {
    printf("rvalue\n");
}

template<typename T>
void testForward(T&& v) {
    printT(v);
    printT(std::forward<T>(v));
    printT(std::move(v));
}

int main() {
    int x = 1;
    testForward(std::forward<int&>(x));
}

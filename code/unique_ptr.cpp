/**********************************************
  > File Name		: unique_ptr.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Sat 04 Jun 2022 04:31:27 PM CST
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/

#include <memory>
#include <iostream>

struct A {
    int a;
    A(int a): a(a) {}
    ~A() {
        std::cout << "destructed" << std::endl;
    }

    friend std::ostream& operator<<(std::ostream& os, const A& a) {
        return os << a.a;
    }
};

int main() {
    std::unique_ptr<A> ptr(new A(1));
    std::cout << *ptr << std::endl;

    std::unique_ptr<A> other = std::move(ptr);
    std::cout << *other << std::endl;
    std::cout << ptr->a << std::endl;
}

/**********************************************
  > File Name		: generic.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Fri 08 Apr 2022 08:06:13 PM CST
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/

#include <iostream>

class Temp {
public:
    void eat() {
        std::cout << "Temp eat" << std::endl;
    }
};

template <typename T>
class Container {
private:
    T t;
public:
    void call_t() {
        t.eat();
    }
};

int main() {
    Container<Temp> c;
    c.call_t();
}

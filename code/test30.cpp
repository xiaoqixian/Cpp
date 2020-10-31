/**********************************************
  > File Name		: test30.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Sat 31 Oct 2020 07:45:17 PM CST
  > Location        : Shanghai
 **********************************************/
#include <iostream>
using namespace std;

template <typename T, typename U>
typename std::enable_if<std::is_base_of<T, U>::value>::type func() {
    cout << "This is int function" << endl;
}

template <typename T, typename U>
typename std::enable_if<!std::is_base_of<T, U>::value>::type func() {
    cout << "This is void function" << endl;
}

struct A {};

struct B:A {};

int main() {
    func<A, B>();
    func<B, A>();
    return 0;
}

/**********************************************
  > File Name		: test29.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Sat 31 Oct 2020 04:06:18 PM CST
  > Location        : Shanghai
 **********************************************/
#include <iostream>
using namespace std;

struct M {
    M() {
        cout << "default constructor" << endl;
    }

    M(const M& m) {
        cout << "copy constructor" << endl;
    }

    M(int&& i) {
        cout << "receive int&&" << endl;
    }

    template <typename T>
    M& operator=(const T& m) {
        cout << "operator =" << endl;
        return *this;
    }

    ~M() {
        cout << "destructor" << endl;
    }
};

int main() {
    M m;
    m = 1;
    return 0;
}

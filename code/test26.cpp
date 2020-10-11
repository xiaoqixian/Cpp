/**********************************************
  > File Name		: test26.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Sun 11 Oct 2020 11:09:44 AM CST
 **********************************************/

#include <iostream>
using namespace std;

//测试

class T {
public:
    T() {
        cout << "constructor" << endl;
    }
    ~T() {
        cout << "destructor" << endl;
    }
};

int main() {
    T t1;
    T t2;
    cout << "pause" << endl;
    t2 = t1;
    cout << "pause" << endl;
    T t3;
}

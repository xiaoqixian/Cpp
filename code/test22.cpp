/**********************************************
  > File Name		: test22.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Sun 11 Oct 2020 09:29:54 AM CST
 **********************************************/

#include <iostream>
using namespace std;

class T {
private:
    int d;
public:
    T(int d):d(d) {
        cout << "call constructor" << endl;
        cout << "d = " << d << endl;
    }
};

int main() {
    T* p1 = new T[5];
    cout << "p2" << endl;
    T* p2 = new T[5](1);
}

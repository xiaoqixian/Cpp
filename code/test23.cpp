/**********************************************
  > File Name		: test23.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Sun 11 Oct 2020 09:40:21 AM CST
 **********************************************/

#include <iostream>
using namespace std;
#include <stdio.h>

class T {
private:
    int d;
public:
    T(int d):d(d) {
        
    }
    ~T() {
        cout << d << " is calling destructor" << endl;
    }
};

T c(3);

int main() {
    T* pa = new T(1);
    T b(2);
    static T d(4);
    delete pa;
}

/**********************************************
  > File Name		: test25.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Sun 11 Oct 2020 10:34:21 AM CST
 **********************************************/

#include <iostream>
using namespace std;

//测试全局变量和静态变量释放内存的先后顺序

class T {
public:
    T(string s):s(s) {

    }
    ~T() {
        cout << s << " call destructor" << endl;
    }
private:
    string s;
};

static T e("e");
T a("a");
T b("b");
static T f("f");

int main() {
    T g("g");
    static T c("c");
    static T d("d");
    T h("h");
    return 0;
}
static T i("i");

/**********************************************
  > File Name		: test18.cpp
  > Author			: lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Fri 02 Oct 2020 07:25:36 PM CST
 **********************************************/

#include <iostream>
using namespace std;

// type traits测试
//

#include <type_traits>

int main() {
    if (is_pointer<int*>::value) {
        cout << "this is a pointer" << endl;
    } else {
        cout << "other type" << endl;
    }
}

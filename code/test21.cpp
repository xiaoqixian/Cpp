/**********************************************
  > File Name		: test21.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Mon 05 Oct 2020 02:26:46 PM CST
 **********************************************/

#include <iostream>
using namespace std;

// sizeof对于指针的长度测量测试

int main() {
    int* p = {1,2,3,4};
    cout << sizeof(p) << endl;
    double* i = new double(12);
    cout << sizeof(i) << endl;
    return 0;
}

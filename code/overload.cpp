/**********************************************
  > File Name		: overload.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Thu 02 Jun 2022 09:42:34 PM CST
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/

#include <stdio.h>

class Test {
    int a;
    Test(int a): a(a) {
        printf("constructor\n");
    }

    bool operator()(int a) const {
        printf("overload: %d\n");
        return false;
    }
};

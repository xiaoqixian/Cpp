/**********************************************
  > File Name		: v.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Fri 30 Jul 2021 11:13:15 AM CST
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/

#include <vector>
#include <stdio.h>
using namespace std;

int main() {
    vector<vector<int>> a(5);
    a[1] = {1,2,3};
    a[0].push_back(1);
    for (auto v: a) {
        printf("a vector size:%d\n", v.size());
    }
}

/**********************************************
  > File Name		: list.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Thu Aug 11 11:25:17 2022
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/

#include <list>
#include <iostream>
using namespace std;


int main() {
    list<string> ls;
    auto it = ls.end();
    ls.insert(it, "hello");

    it--;
    cout << (it == ls.cbegin()) << endl;
}

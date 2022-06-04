/**********************************************
  > File Name		: test.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Thu 02 Jun 2022 09:52:17 PM CST
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/
#include <iostream>
#include <algorithm>
#include <vector>
int main() {
    std::vector<int> v = {1,2,3,4,5};
    std::vector<int> v1(v.size());

    auto it = std::copy_if(v.begin(), v.end(), v1.begin(), [](int i){return i%2!=0;});
    v1.resize(std::distance(v1.begin(), it));

    std::cout << "v1 size: " << v1.size() << std::endl;
}

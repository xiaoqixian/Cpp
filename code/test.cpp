/**********************************************
  > File Name		: test.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Mon Jul 18 12:07:37 2022
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/

#include <unordered_set>
using namespace std;

int main() {
    unordered_set<int> s = {1,2,3,4,5};
    auto it = s.begin();

    it++;
    printf("%d\n", *it);
    s.erase(it);
    printf("%d\n", *it);
    it++;
    printf("%d\n", *it);
}

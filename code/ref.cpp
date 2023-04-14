/**********************************************
  > File Name		: ref.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Tue Aug  2 14:31:29 2022
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/

#include <vector>
using namespace std;

void func(vector<int>& v) {
    vector<int> dup = v;
    dup.pop_back();
    v = dup;
}

int main() {
    vector<int> v = {1,2,3,4};
    func(v);
    printf("%d\n", v.back());
}

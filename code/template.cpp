/**********************************************
  > File Name		: template.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Thu Sep 15 16:59:48 2022
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/

#include <vector>

struct Row {
    uint16_t times[26];
    Row* next[26];
};

int main() {
    Row* r = new Row();

    printf("%d\n", r->next[0] == nullptr);
    printf("%d\n", r->times[0] == 0);
}

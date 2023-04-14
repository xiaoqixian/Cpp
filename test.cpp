/**********************************************
  > File Name		: test.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Fri 03 Jun 2022 04:50:23 PM CST
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/

int func(int a) const {
    return a + 1;
}

int main() {
    int a = func(1);
    a++;
}

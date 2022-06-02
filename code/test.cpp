/**********************************************
  > File Name		: test.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Thu 02 Jun 2022 09:52:17 PM CST
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/

struct S {int a; S(int a): a(a) {}};

int main() {
    S s(1);
    new (&s) S(1);
}

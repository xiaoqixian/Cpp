/**********************************************
  > File Name		: align.c
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Fri 13 Aug 2021 09:09:56 PM CST
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/

#include <stdio.h>

struct A {
    int a;
    char c;
    short s;
    int i;
};

#pragma pack(4)
struct B {
    int a;
    char c;
    char d;
};

int main() {
    struct A d;
    struct A e;
    printf("d.c = 0x%p\n", &d.c);
    printf("d.s = 0x%p\n", &d.s);
    printf("d.i = 0x%p\n", &d.i);
    printf("0x%d\n", sizeof(struct A));
    printf("da = 0x%p\n", &d);
    printf("ea = 0x%p\n", &e);

    struct B b;
    printf("b.c = 0x%p\n", &b.c);
    printf("b.a = 0x%p\n", &b.a);
    printf("b.d = 0x%p\n", &b.d);
}

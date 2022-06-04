/**********************************************
  > File Name		: temp.c
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Sat 10 Jul 2021 11:19:22 PM CST
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/

#include <stdio.h>

void func(void* a) {
    if (a == NULL) {
        printf("null");
    } else {
        printf("not null");
    }
}

int main() {
    int a = -1;
    unsigned short us = a;
    printf("%d\n", us);
}

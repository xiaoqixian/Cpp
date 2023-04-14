/**********************************************
  > File Name		: test.c
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Sun 10 Oct 2021 03:16:29 PM CST
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/
#include <stdio.h>

int strncmp(char* buf1, char* buf2, int size) {
    for (int i = 0; i < size; i++) {
        if (buf1[i] != buf2[i]) {
            return 1;
        }
    }
    return 0;
}

int main() {
    int c[1] = {0, 1};
    printf("%d\n", c[1]);
}

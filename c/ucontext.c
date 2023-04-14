/**********************************************
  > File Name		: ucontext.c
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Tue Jun 14 23:31:30 2022
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/

#include <ucontext.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int idx = 0;
    ucontext_t ctx1;
    getcontext(&ctx1);

    printf("%d\n", idx);
    idx++;
    sleep(1);
    setcontext(&ctx1);
    return 0;
}

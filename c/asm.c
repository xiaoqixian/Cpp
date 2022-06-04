/**********************************************
  > File Name		: asm.c
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Fri 27 Aug 2021 08:39:09 PM CST
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/

int main() {
    int a = 1, b = 2;
    while (a < 5) {
        a++;
    }

    if (a >= 5) {
        func();
    }
}

void func() {
    int a = -4;
    int* b = &a;
    do {
        a++;
    } while (a < 0);
}

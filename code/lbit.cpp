/**********************************************
  > File Name		: lbit.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Mon Aug  8 19:54:28 2022
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/

void unset_lowest_bit(int& val) {
    val &= ~(val & -val);
}

#include <stdio.h>
int main() {
    int a = 0b111, b = 0b110;

    unset_lowest_bit(a);
    unset_lowest_bit(b);

    printf("%d, %d\n", a, b);
}

/**********************************************
  > File Name		: memset.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Sat Aug  6 23:57:51 2022
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/

#include <stdio.h>
#include <string.h>

int d[100];

int main() {
  char str[] = "almost every programmer should know memset!";
  memset (str,'-',6);
  puts (str);
    memset(d, 10, 100*sizeof(int));
    
    printf("%d\n", d[0]);
    printf("%d\n", d[1]);
    printf("%d\n", d[99]);
}

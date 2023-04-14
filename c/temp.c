/**********************************************
  > File Name		: temp.c
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Sat 10 Jul 2021 11:19:22 PM CST
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/
#include <stdio.h>
int main() {
    int sum = 0;
    char n[100];
    scanf("%s", &n);
    while (n) {
        sum += (n % 10);
        n /= 10;
    }
    printf("sum = %d\n", sum);
    
    char* pinyin[10] = {"ling", "yi", "er", "san", "si", "wu", "liu", "qi", "ba", "jiu"};
    int index[100] = {0}, count = 0;
    while (sum) {
        index[count++] = (sum % 10);
        sum /= 10;
    }
    
    while (count--) {
        printf("%s ", pinyin[index[count]]);
    }
    printf("\n");
    return 0;
}


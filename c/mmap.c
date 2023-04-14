/**********************************************
  > File Name		: mmap.c
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Tue 04 Jan 2022 11:46:44 AM CST
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>

void read_by_size(char* buf, size_t size) {
    printf("read: ");
    for (int i = 0; i < size; i++) {
        printf("%c", buf[i]);
    }
    printf("\n");
}

int main() {
    int fd = open("test.db", O_RDWR, 0664);
    char* map = mmap(NULL, 500, PROT_READ, MAP_SHARED, fd, 0);
    
    read_by_size(map, 23);
    
    char buf[] = "this is new content";
    lseek(fd, 23, SEEK_SET);
    int write_size = write(fd, buf, sizeof(buf));
    printf("write %d\n", write_size);

    read_by_size(map, 23 + write_size);
}

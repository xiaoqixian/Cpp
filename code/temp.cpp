/**********************************************
  > File Name		: test6.cpp
  > Author			: lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Sat 19 Sep 2020 04:36:04 PM CST
 **********************************************/

#include <stdio.h>
#include <atomic>

struct foo { int* a; int* b; };
std::atomic<struct foo*> foo_head[10];
int foo_array[10][10];
 
// consume operation starts a dependency chain, which escapes this function
[[carries_dependency]] struct foo* f(int i) {
    return foo_head[i].load(std::memory_order_consume);
}
 
// the dependency chain enters this function through the right parameter
// and is killed before the function ends (so no extra acquire operation takes place)
int g(int* x, int* y [[carries_dependency]]) {
    return std::kill_dependency(foo_array[*x][*y]);
}

class A {
public:
    A(): a(21) {printf("A init\n");}
    int a;
    virtual void func() {
        printf("A func\n");
    }
};

class B: public A {
public:
    B(): b(32) {printf("B init\n");}
    int b;
    virtual void func() override {
        printf("B func\n");
    }
};

int main() {
    B b;
    A* ap = (A*)&b;
    ap->func();
    printf("%d\n", ap->a);
}

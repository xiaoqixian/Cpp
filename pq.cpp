/**********************************************
  > File Name		: pq.cpp
  > Author			: lunar
  > Mail			: lunar_ubuntu@qq.com
  > Created Time	: Thu 16 Jul 2020 08:52:11 AM CST
 ****************************************/

#include <iostream>
using namespace std;

#include <queue>

/*
 * 本代码旨在实现一个根据自定义优先级的优先队列
 * 自定义优先级在一个struct里面重载<运算符实现
 * 
 * 类似与下面这种结构体，可以用于判断两种优先级。
 * 先判断steps的大小，只有在两个变量的steps属性相同时才比较number的大小，
 * 并且都是升序排列。
 */

struct comp {
    int steps;
    int number;
    bool operator < (const comp &a) const {
        if (steps == a.steps) {
            return number < a.number;
        }
        return steps < a.steps;
    }
};

int main() {
    priority_queue<comp> q;
    struct comp c1 = {12, 7}, c2 = {13, 6};
    q.push(c1);
    q.push(c2);
    struct comp f = q.top();
    cout << f.steps << endl;
    return 0;
}

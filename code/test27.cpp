/**********************************************
  > File Name		: test27.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Tue 13 Oct 2020 07:09:08 PM CST
 **********************************************/

#include <iostream>
using namespace std;
#include <unistd.h>
#include <thread>

//多线程测试
//观察新建线程与当前线程脱离后，如果std::thread对象被析构了，是否会发生错误。
void func() {
    for (int i = 0; i < 5; i++) {
        cout << "thread running" << endl;
        sleep(1);
    }
}

int main() {
    cout << "CPU core number: " << thread::hardware_concurrency() << endl;
        return 0;
}

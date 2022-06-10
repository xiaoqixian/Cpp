/**********************************************
  > File Name		: test.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Thu 02 Jun 2022 09:52:17 PM CST
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/
#include <type_traits>
template <typename Func>
typename std::result_of<Func()>::type wrapper(Func&& func) {
    auto res = func();
    return res;
}

int func() {
}

int main() {
    auto res = wrapper(func);
    
}

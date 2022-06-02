/**********************************************
  > File Name		: task.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Fri 03 Jun 2022 12:00:14 AM CST
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/

#include <iostream>
#include <type_traits>
#include <functional>
#include <utility>

template<typename T>
class Task;

template<typename Ret, typename... Args>
class Task<Ret(Args...)> {
private:
    std::function<Ret(Args...)> fn;
public:
    Task(std::function<Ret(Args...)>&& f): fn(std::move(f)) {}
    Task(std::function<Ret(Args...)>& f): fn(f) {}

    Ret invoke(Args&&... args) {
        return this->fn(std::forward<Args>(args)...);
    }

    template<typename Fn>
    auto then(Fn&& f) -> Task<typename std::result_of<Fn(Ret)>::type(Args...)> {
        using return_type = typename std::result_of<Fn(Ret)>::type;
        auto func = std::move(this->fn);

        return Task<return_type(Args...)>([func, &f](Args&&... args) {
            return f(func(std::forward<Args>(args)...));
        });
    }
};

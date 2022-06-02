/**********************************************
  > File Name		: lazy.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Mon 14 Mar 2022 07:11:11 PM CST
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/

#include "optional.hpp"
#include <functional>
#include <type_traits>
#include <utility>

template<typename T>
class Lazy {
private:
    std::function<T()> m_func;
    Optional<T> m_value;

public:
    template<typename Func, typename... Args>
    Lazy(Func& f, Args&&... args) {
        this->m_func = [&f, &args...]{return f(args...);};
    }

    T& value() {
        if (!this->m_value.isInit()) {
            this->m_value = this->m_func();
        }
        return *this->m_value;
    }

    bool isValueCreated() const {
        return this->m_value.isInit();
    }
};

template<typename Func, typename... Args>
Lazy<typename std::result_of<Func(Args...)>::type> lazy(Func&& func, Args&&... args) {
    return Lazy<typename std::result_of<Func(Args...)>::type>(std::forward<Func>(func), std::forward<Args>(args)...);
}

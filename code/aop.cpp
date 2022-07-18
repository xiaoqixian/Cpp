/**********************************************
  > File Name		: aop.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Wed 14 Oct 2020 11:53:12 PM CST
 **********************************************/

#include <functional>
#include <iostream>
#include <type_traits>
// write an AOP framework with C++11

// Tip: when declval receives two args, it returns the second argument when the first one is compilable.
// This struct is to check if a class has the member function before or after.
#define HAS_MEMBER(member)\
template<typename T, typename... Args> struct has_member_##member{\
private:\
    template<typename U> static auto check(int) -> decltype(std::declval<U>().member(std::declval<Args>()...), std::true_type());\
    template<typename U> static std::false_type check(...);\
public:\
    enum{value = std::is_same<decltype(check<T>(0)), std::true_type>::value};\
};\

HAS_MEMBER(before)
HAS_MEMBER(after)

//#include <Noncopyable.hpp>
template <typename Func, typename... Args>
struct Aspect {
    typedef typename std::result_of<Func(Args...)>::type ReturnType;
    Aspect(Func& f): func(std::forward<Func>(f)) {

    }

    //with return value
    template <typename T>
    typename std::enable_if<has_member_before<T, Args...>::value && has_member_after<T, Args...>::value && !std::is_same<ReturnType, void>::value, ReturnType>::type invoke(Args&&... args, T&& aspect) {
        aspect.before(std::forward<Args>(args)...); //aspect before the core code
        auto res = this->func(std::forward<Args>(args)...); //core code
        aspect.after(std::forward<Args>(args)...); //aspect after the core code
        return res;
    }

    //no return value
    template <typename T>
    typename std::enable_if<has_member_before<T, Args...>::value && has_member_after<T, Args...>::value && std::is_same<ReturnType, void>::value>::type invoke(Args&&... args, T&& aspect) {
        aspect.before(std::forward<Args>(args)...); //aspect before the core code
        this->func(std::forward<Args>(args)...); //core code
        aspect.after(std::forward<Args>(args)...); //aspect after the core code
    }

    //with return value
    template <typename T>
    typename std::enable_if<has_member_before<T, Args...>::value && !has_member_after<T, Args...>::value && !std::is_same<ReturnType, void>::value, ReturnType>::type invoke(Args&&... args, T&& aspect) {
        aspect.before(std::forward<Args>(args)...);
        return this->func(std::forward<Args>(args)...);
    }

    //no return value
    template <typename T>
    typename std::enable_if<has_member_before<T, Args...>::value && !has_member_after<T, Args...>::value && std::is_same<ReturnType, void>::value>::type invoke(Args&&... args, T&& aspect) {
        aspect.before(std::forward<Args>(args)...);
        this->func(std::forward<Args>(args)...);
    }

    //with return value
    template <typename T>
    typename std::enable_if<!has_member_before<T, Args...>::value && has_member_after<T, Args...>::value && !std::is_same<ReturnType, void>::value, ReturnType>::type invoke(Args&&... args, T&& aspect) {
        auto res = this->func(std::forward<Args>(args)...);
        aspect.after(std::forward<Args>(args)...);
        return res;
    }

    template <typename T>
    typename std::enable_if<!has_member_before<T, Args...>::value && has_member_after<T, Args...>::value && std::is_same<ReturnType, void>::value>::type invoke(Args&&... args, T&& aspect) {
        this->func(std::forward<Args>(args)...);
        aspect.after(std::forward<Args>(args)...);
    }
    
    //to insert multiple aspects
    template <typename Head, typename... Tail>
    typename std::enable_if<has_member_before<Head, Args...>::value && has_member_after<Head, Args...>::value && !std::is_same<ReturnType, void>::value, ReturnType>::type
    invoke(Args&&... args, Head&& headAspect, Tail&&... tailAspect) {
        headAspect.before(std::forward<Args>(args)...);
        auto res = this->invoke(std::forward<Args>(args)..., std::forward<Tail>(tailAspect)...);
        headAspect.after(std::forward<Args>(args)...);
        return res;
    }
    //no return value
    template <typename Head, typename... Tail>
    typename std::enable_if<has_member_before<Head, Args...>::value && has_member_after<Head, Args...>::value && std::is_same<ReturnType, void>::value>::type
    invoke(Args&&... args, Head&& headAspect, Tail&&... tailAspect) {
        headAspect.before(std::forward<Args>(args)...);
        this->invoke(std::forward<Args>(args)..., std::forward<Tail>(tailAspect)...);
        headAspect.after(std::forward<Args>(args)...);
    }

    template <typename Head, typename... Tail>
    typename std::enable_if<!has_member_before<Head, Args...>::value && has_member_after<Head, Args...>::value && !std::is_same<ReturnType, void>::value, ReturnType>::type
    invoke(Args&&... args, Head&& headAspect, Tail&&... tailAspect) {
        auto res = this->invoke(std::forward<Args>(args)..., std::forward<Tail>(tailAspect)...);
        headAspect.after(std::forward<Args>(args)...);
        return res;
    }
    template <typename Head, typename... Tail>
    typename std::enable_if<!has_member_before<Head, Args...>::value && has_member_after<Head, Args...>::value && std::is_same<ReturnType, void>::value>::type
    invoke(Args&&... args, Head&& headAspect, Tail&&... tailAspect) {
        this->invoke(std::forward<Args>(args)..., std::forward<Tail>(tailAspect)...);
        headAspect.after(std::forward<Args>(args)...);
    }

    template <typename Head, typename... Tail>
    typename std::enable_if<has_member_before<Head, Args...>::value && !has_member_after<Head, Args...>::value && !std::is_same<ReturnType, void>::value, ReturnType>::type
    invoke(Args&&... args, Head&& headAspect, Tail&&... tailAspect) {
        headAspect.before(std::forward<Args>(args)...);
        return this->invoke(std::forward<Args>(args)..., std::forward<Tail>(tailAspect)...);
    }
    template <typename Head, typename... Tail>
    typename std::enable_if<has_member_before<Head, Args...>::value && !has_member_after<Head, Args...>::value && std::is_same<ReturnType, void>::value>::type
    invoke(Args&&... args, Head&& headAspect, Tail&&... tailAspect) {
        headAspect.before(std::forward<Args>(args)...);
        this->invoke(std::forward<Args>(args)..., std::forward<Tail>(tailAspect)...);
    }

private:
    Func func;
};
template <typename T>
using identity_t = T;

//template <typename Func, typename... AP, typename... Args>
template <typename... AP, typename... Args, typename Func>
typename std::enable_if<std::is_same<typename std::result_of<Func(Args...)>::type, void>::value>::type invoke(Func&& f, Args&&... args) {
    Aspect<Func, Args...> asp(std::forward<Func>(f));
    asp.invoke(std::forward<Args>(args)..., AP()...);
    //asp.invokeqie mian(std::forward<Args>(args)..., AP()...);
}

template <typename... AP, typename... Args, typename Func>
typename std::enable_if<!std::is_same<typename std::result_of<Func(Args...)>::type, void>::value, typename std::result_of<Func(Args...)>::type>::type invoke(Func&& f, Args&&... args) {
    Aspect<Func, Args...> asp(std::forward<Func>(f));
    return asp.invoke(std::forward<Args>(args)..., AP()...);
}

struct AA {
    void before() {
        std::cout << "AA before" << std::endl;
    }

    
    void after() {
        std::cout << "AA after" << std::endl;
    }
};

struct BB {
    void before() {
        std::cout << "BB before" << std::endl;
    }

    
    void after() {
        std::cout << "BB after" << std::endl;
    }
};

void core_func() {
    std::cout << "core function called" << std::endl;
}

int core_func2() {
    std::cout << "core function 2" << std::endl;
    return 0;
}

struct Word {
    static int func() {
        return 0;
    }
};

int main() {
    //std::function<void()> f = std::bind(core_func);
    //invoke<AA, BB>(f);
    auto res = invoke<AA, BB>(Word::func);
    std::cout << res << std::endl;
    return 0;
}

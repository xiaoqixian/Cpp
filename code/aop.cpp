/**********************************************
  > File Name		: aop.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Wed 14 Oct 2020 11:53:12 PM CST
 **********************************************/

#include <functional>
// write an AOP framework with C++11

// Tip: when decltype receives two args, it returns the second argument when the first one is compilable.
// This struct is to check if a class has the member function Before or After.
#define HAS_MEMBER(member)\
template<typename T, typename... Args> struct has_member_##member{\
private:\
    template<typename U> static auto Check(int) -> decltype(std::declval<U>().member(std::declval<Args>()...), std::true_type());\
    template<typename U> static std::false_type Check(...);\
public:\
    enum{value = std::is_same<decltype(Check<T>(0)), std::true_type>::value};\
};\

HAS_MEMBER(Foo)
HAS_MEMBER(Before)
HAS_MEMBER(After)

//#include <Noncopyable.hpp>
template <typename Func, typename... Args>
struct Aspect {
    Aspect(Func& f): m_func(std::forward<Func>(f)) {

    }

    template <typename T>
    typename std::enable_if<has_member_Before<T, Args...>::value && has_member_After<T, Args...>::value>::type Invoke(Args&&... args, T&& aspect) {
        aspect.Before(std::forward<Args>(args)...); //aspect before the core code
        m_func(std::forward<Args>(args)...); //core code
        aspect.After(std::forward<Args>(args)...); //aspect after the core code
    }

    template <typename T>
    typename std::enable_if<has_member_Before<T, Args...>::value && !has_member_After<T, Args...>::value>::type Invoke(Args&&... args, T&& aspect) {
        aspect.Before(std::forward<Args>(args)...);
        m_func(std::forward<Args>(args)...);
    }

    template <typename T>
    typename std::enable_if<!has_member_Before<T, Args...>::value && has_member_After<T, Args...>::value>::type Invoke(Args&&... args, T&& aspect) {
        m_func(std::forward<Args>(args)...);
        aspect.After(std::forward<Args>(args)...);
    }

    //to insert multiple aspects
    template <typename Head, typename... Tail>
    void Invoke(Args&&... args, Head&& headAspect, Tail&&... tailAspect) {
        headAspect.Before(std::forward<Args>(args)...);
        Invoke(std::forward<Args>(args)..., std::forward<Tail>(tailAspect)...);
        headAspect.After(std::forward<Args>(args)...);
    }

private:
    Func m_func;
};
template <typename T>
using identity_t = T;

template <typename... AP, typename... Args, typename Func>
void Invoke(Func&& f, Args&&... args) {
    Aspect<Func, Args...> asp(std::forward<Func>(f));
    asp.Invoke(std::forward<Args>(args)..., identity_t<AP>()...);
}

#include <iostream>
using namespace std;
struct AA {
    void Before() {
        cout << "AA Before" << endl;
    }

    void After() {
        cout << "AA After" << endl;
    }
};

struct BB {
    void Before() {
        cout << "BB Before" << endl;
    }

    void After() {
        cout << "BB After" << endl;
    }
};

void core_func() {
    cout << "core function called" << endl;
}

int main() {
    std::function<void()> f = std::bind(core_func);
    Invoke<AA, BB>(f);
    return 0;
}

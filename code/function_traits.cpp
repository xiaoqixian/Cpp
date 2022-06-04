/**********************************************
  > File Name		: function_traits.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Fri 03 Jun 2022 03:15:50 PM CST
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/

#include <functional>
#include <stddef.h>

template<typename T>
struct function_traits;

// for basic functions
template <typename Ret, typename... Args>
struct function_traits<Ret(Args...)> {
    enum { arity = sizeof...(Args) };
    typedef Ret function_type(Args...);
    typedef Ret return_type;

    using stl_function_type = std::function<function_type>;
    typedef Ret(*pointer)(Args...);

    template<size_t I>
    struct args {
        static_assert(I < arity, "index is out of range");
        // Provides compile-time indexed access to the types of the elements of the tuple
        using type = typename std::tuple_element<I, std::tuple<Args...>>::type;
    };
};

//for function pointers
template <typename Ret, typename... Args>
struct function_traits<Ret(*)(Args...)>: function_traits<Ret(Args...)> {};

//for std::function
template <typename Ret, typename... Args>
struct function_traits<std::function<Ret(Args...)>>: function_traits<Ret(Args...)> {};

//for struct member functions
//genera code by macro
#define FUNCTION_TRAITS(...)\
    template <typename ReturnType, typename ClassType, typename... Args>\
    struct function_traits<ReturnType(ClassType::*)(Args...) __VA_ARGS__>:\
        function_traits<ReturnType(Args...)> {};

FUNCTION_TRAITS()
FUNCTION_TRAITS(const)
FUNCTION_TRAITS(volatile)
FUNCTION_TRAITS(const volatile)

//callable objects
template <typename Callable>
struct function_traits: function_traits<decltype(&Callable::operator())> {};

#include <iostream>
#include <typeinfo>
template <typename T>
void print_type() {
    std::cout << typeid(T).name() << std::endl;
}

int main() {
    struct AA {
        int f(int a, int b) volatile {return a * b;}
        int operator()(int) const {return 0;}
    };
    
    print_type<function_traits<decltype(&AA::f)>::function_type>();
}

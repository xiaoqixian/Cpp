/**********************************************
  > File Name		: test6.cpp
  > Author			: lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Sat 19 Sep 2020 04:36:04 PM CST
 **********************************************/

#include <stdio.h>

//primary template
template <size_t, typename...>
class tuple_impl;

template <size_t index, typename T>
class tuple_impl<index, T> {
public:
    T value;
};

template <size_t index, typename T, typename... Ts>
class tuple_impl<index, T, Ts...>: public tuple_impl<index, T>, public tuple_impl<index+1, Ts...> {};

template <typename... Ts>
class tuple: public tuple_impl<0, Ts...> {};

//primary template
template <size_t, typename>
struct tuple_element;

//base case
template <typename T, typename... Ts>
struct tuple_element<0, tuple<T, Ts...>> {
    typedef T type;
};

//recursive case
template <size_t index, typename T, typename... Ts>
struct tuple_element<index, tuple<T, Ts...>>: public tuple_element<index-1, tuple<Ts...>> {};

template <size_t index, typename T>
static typename tuple_element<index, T>::type& get(T& t) {
    typedef typename tuple_element<index, T>::type type;
    return t.tuple_impl<index, type>::value;
}


template <typename, bool = false>
struct A {
    void func() {printf("main\n");}
};

template <typename T>
struct A<T, false> {
    int value;
    void func() {printf("specific\n");}
    explicit A() {}
};

template <typename T>
void test(T const&) {printf("fuck\n");}

template <typename T>
struct remove_cv {typedef T type;};
template <typename T>
struct remove_cv<const T> {typedef T type;};
template <typename T>
struct remove_cv<volatile T> {typedef T type;};
template <typename T>
struct remove_cv<const volatile T> {typedef T type; void func() {}};
//all
//true_type if all Pred is true
template <bool... Pred>
struct all_helper {};

#include <type_traits>
template <bool... Pred>
using all = std::_IsSame<all_helper<Pred...>, all_helper<((void)Pred, true)...>>;

void func() {}

struct is_referenceable {
    template <typename T> static T& test(int) {
        printf("referenable\n");
    }
    template <typename T> static void test(...) {
        printf("unreferenable\n");
    }
};


void bar() {throw 43;}
void foo() {}

int main() {
    printf("%d\n", noexcept(bar()));
    printf("%d\n", noexcept(true));
}

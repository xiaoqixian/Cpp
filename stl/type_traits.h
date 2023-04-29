/**********************************************
  > File Name		: type_traits.h
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Fri Apr 14 11:50:15 2023
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/
#ifndef _TYPE_TRAITS_H
#define _TYPE_TRAITS_H

#include "utility/declval.h"
#include "types.h"

namespace evo {

//true/false type
template <bool b>
struct bool_constant {
    static const bool value = b;
};

typedef bool_constant<true> true_type;
typedef bool_constant<false> false_type;

//integral_constant
template <typename T, T v>
struct integral_constant {
    static const T value = v;
    typedef T value_type;
    typedef integral_constant type;
    operator value_type() const {return value;}
    constexpr value_type operator ()() const {return value;}
};

//enable_if
template <bool, typename T = void>
struct enable_if {};
template <typename T>
struct enable_if<true, T> {typedef T type;};

//if two types is the same
template <typename T, typename U>
struct is_same: false_type {};
template <typename T>
struct is_same<T, T>: true_type {};

//and helper
//like multiple enable_if
template <typename...>
using expand_to_true = true_type;
//every Pred::value has to be true.
template <typename... Pred>
expand_to_true<enable_if<Pred::value>...> and_helper();
template <typename...>
false_type and_helper();

template <typename... Pred>
using And = decltype(and_helper<Pred...>());

//is_not_same
template <typename T, typename U>
struct is_not_same: bool_constant<!is_same<T, U>::value> {};

//Not
template <typename Pred>
struct Not: bool_constant<!Pred::value> {};

//Or
template <typename... Pred>
struct Or: evo::Not<evo::And<evo::Not<Pred>...>> {};

//is_referenceable
struct is_referenceable_impl {
    template <typename T> static T& test(int);
    template <typename T> static void test(...);
};

template <typename T>
struct is_referenceable: bool_constant<is_same<decltype(is_referenceable_impl::test<T>(0)), void>::value> {};

//add lvalue reference
template <typename T, bool = is_referenceable<T>::value>
struct add_lvalue_reference {
    typedef T type;
};
template <typename T>
struct add_lvalue_reference<T, true> {
    typedef T& type;
};

//add rvalue reference
template <typename T, bool = is_referenceable<T>::value>
struct add_rvalue_reference {
    typedef T type;
};
template <typename T>
struct add_rvalue_reference<T, true> {
    typedef T&& type;
};

//remove reference
template <typename T>
struct remove_reference {typedef T type;};
template <typename T>
struct remove_reference<T&> {typedef T type;};
template <typename T>
struct remove_reference<T&&> {typedef T type;};

//remove CV specifier
template <typename T>
struct remove_cv {typedef T type;};
template <typename T>
struct remove_cv<const T> {typedef T type;};
template <typename T>
struct remove_cv<volatile T> {typedef T type;};
template <typename T>
struct remove_cv<const volatile T> {typedef T type;};

//remove CV specifier and reference
template <typename T>
struct remove_cv_ref {
    typedef typename remove_cv<typename remove_reference<T>::type>::type type;
};

//conditional
template <bool b, typename If, typename Then>
struct conditional {typedef If type;};
template <typename If, typename Then>
struct conditional<false, If, Then> {typedef Then type;};

//add const
template <typename T>
struct add_const {typedef const T type;};

//all
//true_type if all Pred is true
template <bool... Pred>
struct all_helper {};

template <bool... Pred>
using all = is_same<all_helper<Pred...>, all_helper<((void)Pred, true)...>>;

//is_lvalue_reference
template <typename T>
struct is_lvalue_reference: bool_constant<__is_lvalue_reference(T)> {};

//make_integer_sequence
template <size_t... values>
struct integer_sequence {
    using type = integer_sequence;
    //using to_tuple_indices = evo::tuple_indices<values...>;
};

template <typename, typename>
struct concat_integer_sequence;
template <size_t... N1, size_t... N2>
struct concat_integer_sequence<integer_sequence<N1...>, integer_sequence<N2...>>: integer_sequence<N1..., (sizeof...(N1)+N2)...> {};

template <size_t N>
struct make_integer_sequence: concat_integer_sequence<integer_sequence<N/2>, integer_sequence<N-N/2>>::type {};
template <>
struct make_integer_sequence<0>: integer_sequence<> {};
template <>
struct make_integer_sequence<1>: integer_sequence<0> {};

//is_array
template <typename>
struct is_array: false_type {};
template <typename T>
struct is_array<T[]>: true_type {};
template <size_t N, typename T>
struct is_array<T[N]>: true_type {};

//is_void
template <typename>
struct is_void: false_type {};
template <>
struct is_void<void>: true_type {};

//is_constructible
template <typename, typename T, typename... Args>
struct is_constructible: false_type {};

template <typename T, typename... Args>
struct is_constructible<decltype(T(evo::declval<Args>()...)), T, Args...>: true_type {};

//is_nothrow_constructible
template <typename T, typename... Args>
struct is_nothrow_constructible: public bool_constant<__is_nothrow_constructible(T, Args...)> {};

//is_default_constructible
template <typename T, typename... Args>
struct is_default_constructible: public is_constructible<T, Args...> {};

//is_move_constructible
template <typename T>
struct is_move_constructible: public is_constructible<T, typename add_rvalue_reference<T>::type> {};

//is_copy_constructible
template <typename T>
struct is_copy_constructible: public is_constructible<T, typename add_lvalue_reference<typename add_const<T>::type>::type> {};

//is_nothrow_copy_constructible
template <typename T>
struct is_nothrow_copy_constructible: public is_nothrow_constructible<T, typename add_lvalue_reference<typename add_const<T>::type>::type> {};

//is_nothrow_default_constructible
template <typename T>
struct is_nothrow_default_constructible: public is_nothrow_constructible<T> {};

//is_nothrow_move_constructible
template <typename T>
struct is_nothrow_move_constructible: public is_nothrow_constructible<
                                      typename add_lvalue_reference<T>::type,
                                      typename add_rvalue_reference<T>::type> {};

//is_implicitly_default_constructible
template <typename T>
void test_implicit_default_constructible(T const&);

template <typename T, typename = void, typename = typename is_default_constructible<T>::type>
struct is_implicitly_default_constructible: false_type {};

template <typename T>
struct is_implicitly_default_constructible<T, 
    decltype(test_implicit_default_constructible<T const&>({})), true_type>
        : true_type {};

template <typename T>
struct is_implicitly_default_constructible<T, 
    decltype(test_implicit_default_constructible<T const&>({})), false_type>
        : false_type {};

//is_convertible
template <typename T, typename U>
struct is_convertible: integral_constant<bool, __is_convertible_to(T, U)> {};

//is_assignable
template <typename T, typename U>
struct is_assignable: bool_constant<__is_assignable(T, U)> {};

//is_nothrow_assignable
template <typename T, typename Arg>
struct is_nothrow_assignable: public bool_constant<__is_nothrow_assignable(T, Arg)> {};

//is_move_assignable
template <typename T>
struct is_move_assignable: is_assignable<
                           typename add_lvalue_reference<T>::type,
                           typename add_rvalue_reference<T>::type> {};

//is_nothrow_move_assignable
template <typename T>
struct is_nothrow_move_assignable: public bool_constant<is_nothrow_constructible<
                                   typename add_lvalue_reference<T>::type, 
                                   typename add_rvalue_reference<T>::type
                                   >::value> {};

//is_copy_assignable
template <typename T>
struct is_copy_assignable: is_assignable<
                           typename add_lvalue_reference<T>::type, 
                           typename add_lvalue_reference<T>::type> {};

//is_nothrow_copy_assignable
template <typename T>
struct is_nothrow_copy_assignable: bool_constant<is_nothrow_constructible<
                                   typename add_lvalue_reference<T>::type,
                                   typename add_lvalue_reference<T>::type
                                   >::value> {};

//swappable
template <typename T>
struct is_swappable;
template <typename T>
struct is_nothrow_swappable;

//swap
template <typename T>
using swap_result_t = typename enable_if<is_move_constructible<T>::value && is_move_assignable<T>::value>::type;

//implementation in utility/swap.h
template <typename T>
inline swap_result_t<T> swap(T& x, T& y) noexcept(is_nothrow_move_constructible<T>::value && is_nothrow_move_assignable<T>::value);

template <typename T, size_t N>
inline typename enable_if<is_swappable<T>::value>::type swap(T (&a)[N], T(&b)[N])
    noexcept(is_nothrow_swappable<T>::value);

//swappable with
struct nat {
    nat() = delete;
    nat(nat const&) = delete;
    nat& operator=(nat const&) = delete;
    ~nat() = delete;
};

template <typename T, typename U = T, bool = !is_void<T>::value && !is_void<U>::value>
struct swappable_with {
    template <typename L, typename R>
    static decltype(swap(declval<L>(), declval<R>())) test_swap(int);
    template <typename, typename>
    static nat test_swap(long);

    typedef decltype(test_swap<T, U>(0)) swap1;
    typedef decltype(test_swap<U, T>(0)) swap2;

    static const bool value = is_not_same<swap1, nat>::value && is_not_same<swap2, nat>::value;
};

template <typename T, typename U>
struct swappable_with<T, U, false>: false_type {};

//nothrow_swappable_with
template <typename T, typename U = T, bool swappable = swappable_with<T, U>::value>
struct nothrow_swappable_with {
    static const bool value = 
        noexcept(swap(declval<T>(), declval<U>()))
    &&  noexcept(swap(declval<U>(), declval<T>()));
};

template <typename T, typename U>
struct nothrow_swappable_with<T, U, false>: false_type {};
}


#endif /* _TYPE_TRAITS_H*/

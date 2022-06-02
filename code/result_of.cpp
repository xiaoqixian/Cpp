/**********************************************
  > File Name		: result_of.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Thu 02 Jun 2022 03:53:53 PM CST
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/

#include <type_traits>
#include <iostream>

int fn(int) {return int();}

typedef int(&fn_ref)(int);
typedef int(*fn_ptr)(int);

struct fn_class {
    int operator()(int i) {
        return i;
    }
};

struct Person {int i;};

#include <map>
#include <vector>
#include <algorithm>
template <typename Fn>
std::multimap<typename std::result_of<Fn(Person)>::type, Person>
groupBy(const std::vector<Person>& vt, Fn&& keySelector) {
    // deduce the return type of keySelector.
    typedef typename std::result_of<Fn(Person)>::type key_type;
    std::multimap<key_type, Person> map;
    std::for_each(vt.begin(), vt.end(), [&](const Person& person) {
        map.insert(std::make_pair(keySelector(person), person));
    });
}

int main() {
    typedef std::result_of<decltype(fn)&(int)>::type A;
    typedef std::result_of<fn_ref(int)>::type B;
    typedef std::result_of<fn_ptr(int)>::type C;
    typedef std::result_of<fn_class(int)>::type D;

    std::cout << "A: " << std::is_same<int, A>::value << std::endl;
    std::cout << "B: " << std::is_same<int, B>::value << std::endl;
    std::cout << "C: " << std::is_same<int, C>::value << std::endl;
    std::cout << "D: " << std::is_same<int, D>::value << std::endl;

    return 0;
}


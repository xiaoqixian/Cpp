/**********************************************
  > File Name		: multi_args.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Thu 02 Jun 2022 08:18:46 PM CST
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/

template<typename... Args> struct Sum;

template<typename First, typename... Rest>
struct Sum<First, Rest...> {
    enum {value = Sum<First>::value + Sum<Rest...>::value};
};

template<typename Last> struct Sum<Last> {
    enum {value = sizeof(Last)};
};


template<int...>
struct IndexSeq {};

template<int N, int... Indexes>
struct MakeIndexes: MakeIndexes<N-1, N-1, Indexes...> {};

template<int... Indexes>
struct MakeIndexes<0, Indexes...> {
    typedef IndexSeq<Indexes...> type;
};

#include <iostream>
#include <typeinfo>
int main() {
    using T = MakeIndexes<3>::type;
    std::cout << typeid(T).name() << std::endl;
    return 0;
}

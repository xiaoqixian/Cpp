/**********************************************
  > File Name		: ps.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Sat Apr 15 21:08:01 2023
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/
#include <stdio.h>
#include <complex>
#include <vector>
#include <string>
//partial specialization

template <typename, typename, typename>
struct A {
    void func() {
        printf("declaration1\n");
    }
};

template <typename T, typename U>
struct A<bool, T, U> {
    void func() {
        printf("declaration2\n");
    }
};
template<int I, int J, int K> struct B {};
template<int I> struct B<I, I*2, 2> {}; // OK: first parameter is deducible
                                        

template <typename T>
struct identity {typedef T type;};
template <typename T>
void good(std::vector<T>, typename identity<T>::type) {}
template <typename T>
void bad(std::vector<T>, T) {}

std::vector<std::complex<double>> x;

template <typename T>
void f(std::vector<T>) {}

//void f(double) {printf("f1\n");}
class Solution {
public:
    int addMinimum(std::string word) {
        int res = 0, state = 0;
        for (const char c: word) {
            switch (state) {
                case 0: switch (c) {
                    case 'a': state = 1; break;
                    case 'b': res++; state = 2; break;
                    case 'c': res += 2; break;
                    default: break;
                } break;
                default: break;
            }
        }
    }
};

int main() {
    printf("%d\n", -1 % 3);
}


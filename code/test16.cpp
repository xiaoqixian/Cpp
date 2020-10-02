/**********************************************
  > File Name		: test16.cpp
  > Author			: lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Thu 01 Oct 2020 02:51:55 PM CST
 **********************************************/

#include <iostream>
using namespace std;

// 测试隐形转换调用构造函数
//

template<typename T>
class Rational {
public:
    Rational(const T& nominator = 0, const T& denominator = 1):
    nominator(nominator),denominator(denominator)
    {
        cout << "call constructor" << endl;
        cout << "nominator = " << nominator << endl;
        cout << "denominator = " << denominator << endl;
    }
    const T get_nominator() const {
        return nominator;
    }
    const T get_denominator() const {
        return denominator;
    }
    friend const Rational operator*(const Rational& lhs, const Rational& rhs) {
        return Rational(lhs.get_nominator()*rhs.get_nominator(), lhs.get_denominator()*rhs.get_denominator());
    }
private:
    T nominator;
    T denominator;
};
/*
template<typename T>
const Rational<T> operator*(const Rational<T>& lhs, const Rational<T>& rhs) {
    return Rational<T>(lhs.get_nominator()*rhs.get_nominator(), lhs.get_denominator()*rhs.get_denominator());
}
*/
int main() {
    Rational<int> oneFourth(1,4);
    Rational<int> result = oneFourth * 2;
    cout << "result: " << result.get_nominator() << "/" << result.get_denominator() << endl;
    return 0;
}

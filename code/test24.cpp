/**********************************************
  > File Name		: test24.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Sun 11 Oct 2020 10:15:11 AM CST
 **********************************************/

#include <iostream>
using namespace std;

#include<iostream>
using namespace std;
class MyClass
{
public:
    MyClass(int i = 0)
    {
        cout << i;
    }
    MyClass(const MyClass &x)
    {
        cout << 2;
    }
    MyClass operator=(const MyClass &x)
    {
        cout << 3;
        return *this;
    }
    ~MyClass()
    {
        cout << 4;
    }
};
int main()
{
    MyClass obj1(1), obj2(2);
    cout << "pause" << endl;
    MyClass obj3 = obj1;
    return 0;
}




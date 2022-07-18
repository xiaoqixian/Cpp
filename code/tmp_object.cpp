/**********************************************
  > File Name		: tmp_object.cpp
  > Author			: lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Sun 13 Sep 2020 04:03:22 PM CST
 **********************************************/

#include <iostream>
using namespace std;

//c++内的临时对象探寻
//


class TempObj {
public:
    TempObj(int m = 0, int n = 0);
    TempObj(TempObj& to)
    {
        cout << "Copy Constructor" << endl;
        m_Number = to.m_Number;
        m_Size = to.m_Size;
    }
    ~TempObj()
    {
        //cout << "Deconstructor" << endl;
    }
    int GetSum(TempObj &ts);
public:
    int m_Number;
    int m_Size;
};

TempObj::TempObj(int m,int n)
{
    cout << "Default constructor" << endl;
    m_Number = m;
    m_Size = n;
    cout << "m_Number=" << m_Number <<" "<< "m_Size=" << m_Size << endl;
}

int TempObj::GetSum(TempObj &ts)
{
    int tmp = ts.m_Number + ts.m_Size;
    ts.m_Number = 1000;
    return tmp;
}

int main()
{
    TempObj tm(10,20),sum;
    sum = 1000; //令我吃惊的是C++中竟然允许这种程度的类型转换.
    cout << "sum=" << tm.GetSum(sum) << endl;
}

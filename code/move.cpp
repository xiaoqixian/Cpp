/**********************************************
  > File Name		: move.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Wed 01 Jun 2022 11:25:07 AM CST
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/

#include <string>
#include <iostream>
using namespace std;

struct Item {
    string s;

    Item(const string&& s): s(s) {}

    ~Item() {
        cout << "Item(" << s << ") desctructed" << endl;
    }

    ostream& operator<<(ostream& os) {
        return os << this->s;
    }
};

struct Node {
    const Item* s;
    Node(const Item* s): s(s) {}
};

struct Rr {
    const Item s;
    Rr(const Item&& s): s(s) {}
    Rr(const Item& s): s(s) {}
};

pair<Node, Rr> func() {
    const Item s("hello");
    cout << "s location: " << &s << endl;
    Node n(&s);
    Rr r(std::move(s));
    
    cout << "p: " << n.s << endl;
    cout << "r: " << &r.s << endl;
    return make_pair(n, r);
}

int main() {
    auto a = func();
    //cout << *a.first.s << endl;
    //cout << a.second.s << endl;
    cout << "p: " << a.first.s << endl;
    cout << "r: " << &a.second.s << endl;
}

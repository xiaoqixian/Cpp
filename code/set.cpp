/**********************************************
  > File Name		: set.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Sat Jul 30 10:23:54 2022
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/

#include <set>
#include <string>
#include <iostream>
using namespace std;

struct Node {
    const string food;
    int rating;

    Node(string food, int rating): food(food), rating(rating) {}
    Node() {}

    bool operator()(const Node& n1, const Node& n2) const {
        if (n1.rating == n2.rating) {
            return n1.food < n2.food;
        } else return n1.rating > n2.rating;
    }
};

int main() {
    auto print_set = [](const set<Node, Node>& s) {
        for (const auto& n: s) {
            cout << "(" << n.food << ", " << n.rating << ")\n";
        }
    };
    set<Node, Node> s;
    s.insert(Node("a", 0));
    s.insert(Node("b", 0));
    auto it = s.find(Node("c", 0));
    assert(it != s.end());
}

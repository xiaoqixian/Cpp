/**********************************************
  > File Name		: tree_test.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Mon 05 Oct 2020 09:07:25 PM CST
 **********************************************/

#include <iostream>
using namespace std;

#include "AVLTree.h"

class node {
private:
    size_t _size;
    char* _start;
public:
    node(size_t _size);
    int operator -(const node& rhs);
    bool operator <(const node& rhs);
    bool operator >(const node& rhs);
    bool operator <=(const node& rhs);
    bool operator >=(const node& rhs);
    bool operator ==(const node& rhs);
};

node::node(size_t size):_size(size) {

}

int node::operator -(const node& rhs) {
    return static_cast<int>(_size - rhs._size);
}

bool node::operator <(const node& rhs) {
    return _size < rhs._size;
}

bool node::operator >(const node& rhs) {
    return _size > rhs._size;
}

bool node::operator<=(const node& rhs) {
    return _size <= rhs._size;
}

bool node::operator>=(const node& rhs) {
    return _size >= rhs._size;
}

bool node::operator==(const node& rhs) {
    return _size == rhs._size;
}

int main() {
    AVLTree<node> tree;
    node n1(12);
    node n2(1);
    node n3(10);
    
    tree.insert(n1);
    tree.insert(n2);
    tree.remove(n3);
    return 0;
}

/**********************************************
  > File Name		: pq.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Sat Jul 23 21:23:53 2022
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/

#include <queue>
#include <iostream>
#include <vector>

struct Comparator {
    int val;
    Comparator(int val): val(val) {}

    bool operator()(const Comparator* c1, const Comparator* c2) {
        return c1->val <= c2->val;
    }
};

int main() {
    typedef std::priority_queue<int, std::vector<int>, std::greater<int>> pq_type;
    pq_type pq;
    pq.push(2); pq.push(1); pq.push(3);

    while (!pq.empty()) {
        std::cout << pq.top() << ", ";
        pq.pop();
    }
    //typedef std::priority_queue<Comparator*, std::vector<Comparator*>, Comparator> pq_type;
    //pq_type pq(Comparator(0));

    //std::vector<int> v = {15, 13, 8, 7, 4,3,1};
    //std::vector<Comparator*> cc;

    //for (int i: v) {
        //Comparator* c = new Comparator(i);
        //cc.push_back(c);
        //pq.push(c);
    //}

    //std::cout << "top: " << pq.top()->val << std::endl;

    //cc[4]->val = 17;
    //Comparator* c = pq.top(); pq.pop();
    //pq.push(c);
    //std::cout << "top: " << pq.top()->val << std::endl;
}

/**********************************************
  > File Name		: test6.cpp
  > Author			: lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Sat 19 Sep 2020 04:36:04 PM CST
 **********************************************/

#include <stack>
#include <stdio.h>

struct TreeNode {
    int val;
    TreeNode* left, *right;
};

int main() {
    std::stack<TreeNode*> s;
    printf("%d\n", s.top()->val);
}

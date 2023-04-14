/**********************************************
  > File Name		: find.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Mon Feb 13 17:21:23 2023
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/

#include <string>
#include <vector>
using namespace std;

class Solution {
public:
    vector<vector<int>> substringXorQueries(string s, vector<vector<int>>&& queries) {
        auto make = [](int first, int second) {
            printf("first = %d, second = %d\n", first, second);
            int l1 = 0, l2 = 0;
            int temp = first;
            while (temp) {
                l1++;
                temp >>= 1;
            }
            temp = second;
            while (temp) {
                l2++;
                temp >>= 1;
            }
            printf("l1 = %d, l2 = %d\n", l1, l2);

            int l = max(l1, l2), rl = l;
            char* res = new char[l];
            while (first || second) {
                res[--l] = ((first & 1) ^ (second & 1)) + '0';
                printf("res[%d] = %c\n", l, res[l]);
                first >>= 1;
                second >>= 1;
                //printf("first = %d, second = %d\n", first, second);
            }
            //printf("fuck\n");
            //printf("substr: %s\n", res);
            return std::make_pair(res, rl);
        };

        vector<vector<int>> res;
        for (const auto& q: queries) {
            auto [substr, len] = make(q[0], q[1]);
            int find = s.find(substr);
            printf("len = %d\n", len);
            delete[] substr;
            if (find >= 0) {
                res.push_back({find, find + len});
            }
        }
        return res;
    }
};

int main() {
    Solution().substringXorQueries("101101", {{0,5},{1,2}});
}

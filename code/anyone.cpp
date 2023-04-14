/**********************************************
  > File Name		: anyone.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Tue Feb 14 17:59:46 2023
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/

static auto anyone = [](auto&& k, auto&&... args) -> bool {
    return ((args == k) || ...);
};

bool func(auto&& k, auto&&... args) {
    return ((args == k) || ...);
}

int main() {
    if (anyone('x', 'x', 'A', 'C')) {
        return 2;
    }
    return 1;
}

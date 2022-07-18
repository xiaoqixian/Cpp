/**********************************************
  > File Name		: flag.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Mon 27 Dec 2021 09:44:32 PM CST
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/

class Flags {
    unsigned int inner;

    Flags(unsigned int val): inner(val) {}

    unsigned int get_inner() {
        return this->inner;
    }
};

class PageFlags: public Flags {
    PageFlags(unsigned int val): inner(val) {}
};

int main() {
    PageFlags* pf = new PageFlags(23);
}

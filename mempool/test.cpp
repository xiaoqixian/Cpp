/**********************************************
  > File Name		: test.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Sat 03 Oct 2020 02:08:00 PM CST
 **********************************************/

#include <iostream>
using namespace std;
#include "mempool.h"
#include "debug.h"

class TestObject {
public:
    TestObject(int x) {
        if (this == NULL) {
            DEBUG("no memory for assignement");
            return ;
        }
        data = x;
    }

    void* operator new(size_t size);
    void operator delete(void* p);
    void* operator new[](size_t size);
    void operator delete[](void* p);
private:
    int data;
};

void* TestObject::operator new(size_t size) {
    return MemoryPool::get_instance()->allocate(size);
}

void* TestObject::operator new[](size_t size) {
    // continous memory allocate is not supported for now.
    return NULL;
}

void TestObject::operator delete(void* p) {
    MemoryPool::get_instance()->collect(p, (size_t)sizeof(TestObject));
}

void TestObject::operator delete[](void* p) {

}

int main(int argc, char** argv) {
    TestObject* to = new TestObject(12);
    TestObject* to1 = new TestObject(12);
    TestObject* to2 = new TestObject(12);
    TestObject* to3 = new TestObject(12);
    TestObject* to4 = new TestObject(12);
    TestObject* to5 = new TestObject(12);
    TestObject* to6 = new TestObject(12);
    TestObject* to7 = new TestObject(12);
    delete to;
    delete to1;
    TestObject* to8 = new TestObject(12);
    return 0;
}

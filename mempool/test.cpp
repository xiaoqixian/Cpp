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

    TestObject() {
        if (this == NULL) {
            DEBUG("no memory for assignement");
            return ;
        }
        data = 0;
    }

    void* operator new(size_t size);
    void operator delete(void* p);
    void* operator new[](size_t size);
    void operator delete[](void* p);
private:
    int data;
};

void* TestObject::operator new(size_t size) {
    void* p = MemoryPool::get_instance()->allocate(size);
    // TODO: ask for more memory from operating system when there is no enough memory in the pool.
    // currently it just throws bad_alloc() error.
    if (p) {
        return p;
    } else {
        throw bad_alloc();
    }
}

void* TestObject::operator new[](size_t size) {
    void* p = MemoryPool::get_instance()->allocate(size);
    if (p) {
        return p;
    } else {
        throw bad_alloc();
    }
}

void TestObject::operator delete(void* p) {
    MemoryPool::get_instance()->collect(p, sizeof(TestObject));
}

void TestObject::operator delete[](void* p) {
    //delete[] currently not supported.
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
    DEBUG("to deleted");
    delete to1;
    DEBUG("to1 deleted");
    TestObject* to8 = new TestObject(12);
    return 0;
}

/**********************************************
  > File Name		: mempool.h
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Fri 02 Oct 2020 09:11:16 PM CST
 **********************************************/

#ifndef _MEMPOOL_H
#define _MEMPOOL_H

struct mem_node {
    char* _start;
    size_t _size;
    mem_node* pre;
    mem_node* next;
    mem_node(char* start, size_t size, mem_node* pre = NULL, mem_node* next = NULL):_start(start),_size(size),pre(pre),next(next) {

    }
    /*
    int operator -(const mem_node& rhs) {
        return static_cast<int>(_size - rhs._size);
    }
    bool operator <(const mem_node& rhs) {
        return _size < rhs._size;
    }
    bool operator >(const mem_node& rhs) {
        return _size > rhs._size;
    }
    bool operator <=(const mem_node& rhs) {
        return _size <= rhs._size;
    }
    bool operator >=(const mem_node& rhs) {
        return _size >= rhs._size;
    }
    bool operator ==(const mem_node& rhs) {
        return _size == rhs._size;
    }*/
};

class MemoryPool {
public:
    void* allocate(size_t size);
    void collect(void* mem, size_t size);
    static MemoryPool* get_instance();
    void collect_node(mem_node* new_node);
private:
    char* _start;
    char* _top;
    char* _end;
    static size_t MAX_CAP;
    struct mem_node* header;
    static MemoryPool* instance;

    MemoryPool(size_t size);
    ~MemoryPool();
    void* find_in_list(size_t size);
    void* find_in_tree(size_t size);
    mem_node* merge_chunk(const mem_node* c1, const mem_node* c2);
};

#endif

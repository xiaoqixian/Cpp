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
    struct mem_node* next;
    struct mem_node* pre;
    mem_node(char* start, size_t size):_start(start),_size(size) {
        next = NULL;
        pre = NULL;
    }
    mem_node(char* start, size_t size, mem_node* next, mem_node* pre):
        _start(start),
        _size(size),
        next(next),
        pre(pre) {

    }
};

class MemoryPool {
public:
    void* allocate(size_t size);
    void collect(void* mem, size_t size);
    static MemoryPool* get_instance();
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
    mem_node* merge_chunk(const mem_node* c1, const mem_node* c2);
    void collect_node(mem_node* new_node);
};

#endif

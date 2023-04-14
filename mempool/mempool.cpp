/**********************************************
  > File Name		: mempool.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Sat 03 Oct 2020 09:38:22 AM CST
 **********************************************/

#include <mutex>
#include <stdlib.h>
#include <memory.h>
#include <future>
#include "debug.h"
#include "mempool.h"
#include "AVLTree.h"

MemoryPool* MemoryPool::instance = NULL;
size_t MemoryPool::MAX_CAP = 30; //default max capacity = 2Mb

void wrapper(MemoryPool* instance, mem_node* new_node) {
    instance->collect_node(new_node);
}

MemoryPool::MemoryPool(size_t size) {
    _start = (char*)malloc(size);
    if (_start == 0) {
        LOG_ERROR("malloc failed");
        exit(0);
    }
    _top = _start;
    _end = _start + size;
    header = new mem_node(NULL, -1);
}

MemoryPool::~MemoryPool() {
    free(_start);
    _start = NULL;
    _top = NULL;
    _end = NULL;
    while (header != NULL) {
        mem_node* temp = header->next;
        delete header;
        header = temp;
    }
}

MemoryPool* MemoryPool::get_instance() {
    if (instance == NULL) {
        DEBUG("Memory Pool initializing, capacity: %zd", MAX_CAP);
        instance =  new MemoryPool(MAX_CAP);
    }
    return instance;
}

/*
 * Search in the memory pool first.
 * If the size is not enough, then go to the linked list to find.
 * If success, return a pointer.
 * Else, return NULL
 */
void* MemoryPool::allocate(size_t size) {
    if (static_cast<size_t>(_end - _top) > size) {
        void* res = (void*)_top;
        _top += size;
        DEBUG("allocate from memory pool: %zd", size);
        return res;
    } else {
        return find_in_list(size);
    }
}

/*
 * Get a memory fragmentation in the linked list.
 */
void* MemoryPool::find_in_list(size_t size) {
    mem_node* temp = header->next;
    if (temp == NULL || size > temp->_size) {
        DEBUG("allocate failed");
        return NULL;
    }
    while (temp != NULL) {
        if (temp->next == NULL) {
            if (temp->_size == size) {
                temp->pre->next = NULL;
                void* res = (void*)(temp->_start);
                DEBUG("allocate from linked list: %zd", size);
                delete temp;
                memset(res, 0, size);
                return res;
            } else {
                mem_node* cut_node = new mem_node(temp->_start + size, temp->_size - size);
                temp->pre->next = cut_node;
                void* res = (void*)(temp->_start);
                DEBUG("allocate from linked list: %zd", size);
                delete temp;
                memset(res, 0, size);
                return res;
            }
        } else {
            if (size > temp->next->_size) {
                if (size == temp->_size) {
                    temp->pre->next = temp->next;
                    temp->next->pre = temp->pre;
                    DEBUG("allocate from linked list: %zd", size);
                    void* res = (void*)(temp->_start);
                    delete temp;
                    memset(res, 0, size);
                    return res;
                } else {
                    //mem_node* cut_node = new mem_node(temp->_start + size, temp->_size - size);
                    temp->pre->next = temp->next;
                    temp->next->pre = temp->pre;
                    DEBUG("allocate from linked list: %zd", size);
                    void* res = (void*)(temp->_start);
                    memset(res, 0, size);
                    temp->_start += size;
                    temp->_size -= size;
                    
                    // TODO:this step can be made asynchronous.
                    std::async(std::launch::async, wrapper, MemoryPool::get_instance(), temp);
                    
                    return res;
                }
            } else {
                temp = temp->next;
                continue;
            }
        }
    }
    DEBUG("allocate failed");
    return NULL;
}

/*
 * find in AVL tree
 * well, use AVL tree to manage memory fragmentations seems not a good idea for me now.
 */
void* MemoryPool::find_in_tree(size_t size) {
    return NULL;
}

/*
 * Memory collect
 */
void MemoryPool::collect(void* mem, size_t size) {
    DEBUG("collecting memory chunk");
    mem_node* new_node = new mem_node((char*)mem, size);
    collect_node(new_node);    
}


/*
 * insert a memory node into the linked list.
 */
void MemoryPool::collect_node(mem_node* new_node) {
    mem_node* temp = header->next;
    if (temp == NULL) {
        header->next = new_node;
        new_node->pre = header;
        return ;
    }
    if (new_node->_size > temp->_size) {
        mem_node* merged_node = merge_chunk(new_node, temp);
        if (merged_node != NULL) {
            DEBUG("merge two memory chunks");
            temp->pre->next = temp->next;
            delete new_node;
            delete temp;
            collect_node(merged_node);
            return ;
        }
        new_node->next = temp;
        new_node->pre = temp->pre;
        temp->pre->next = new_node;
        temp->pre = new_node;
        return ;
    }
    while (temp != NULL) {
        mem_node* merged_node = merge_chunk(new_node, temp);
        if (merged_node != NULL) {
            DEBUG("merge two memory chunks");
            temp->pre->next = temp->next;
            delete new_node;
            delete temp;
            collect_node(merged_node);
            return ;
        }

        if (temp->next == NULL) {
            temp->next = new_node;
            new_node->pre = temp;
            return ;
        } else {
            if (new_node->_size > temp->next->_size) {
                new_node->next = temp;
                new_node->pre = temp->pre;
                temp->pre->next = new_node;
                temp->pre = new_node;
                return ;
            } else {
                temp = temp->next;
            }
        }
    }
}

mem_node* MemoryPool::merge_chunk(const mem_node* chunk1, const mem_node* chunk2) {
    DEBUG("Try to merge two chunks.");
    DEBUG("chunk1: [%p, %zd], chunk2: [%p, %zd]", chunk1->_start, chunk1->_size, chunk2->_start, chunk2->_size);
    if (chunk1->_start + chunk1->_size == chunk2->_start) {
        mem_node* res = new mem_node(chunk1->_start, chunk1->_size+chunk2->_size);
        return res;
    } else if (chunk2->_start + chunk2->_size == chunk1->_start) {
        mem_node* res = new mem_node(chunk2->_start, chunk1->_size+chunk2->_size);
        return res;
    } else {
        return NULL;
    }
}

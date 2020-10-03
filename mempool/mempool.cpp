/**********************************************
  > File Name		: mempool.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Sat 03 Oct 2020 09:38:22 AM CST
 **********************************************/

#include <mutex>
#include <stdlib.h>
#include <memory.h>
#include "debug.h"
#include "mempool.h"

MemoryPool* MemoryPool::instance = NULL;
size_t MemoryPool::MAX_CAP = 30; //default max capacity = 2Mb

MemoryPool::MemoryPool(size_t size) {
    _start = (char*)malloc(size);
    if (_start == 0) {
        LOG_ERROR("malloc failed");
        exit(0);
    }
    _top = _start;
    _end = _start + size;
    header = new mem_node(0, -1);
}

MemoryPool::~MemoryPool() {
    free(_start);
    _start = NULL;
    _top = NULL;
    _end = NULL;

    //linked list resource collect
    mem_node* temp;
    while (header != NULL) {
        temp = header->next;
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
    if (_end - _top > size) {
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
            temp->pre->next = NULL;
            DEBUG("allocate from linked list: %zd", size);
            return (void*)(temp->_start);
        } else {
            if (size > temp->next->_size) {
                temp->pre->next = temp->next;
                temp->next->pre = temp->pre;
                DEBUG("allocate from linked list: %zd", size);
                return (void*)(temp->_start);
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
 * Memory collect
 */
void MemoryPool::collect(void* mem, size_t size) {
    DEBUG("collecting memory chunk");
    mem_node* new_node = new mem_node((char*)mem, size);
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
            temp->pre = temp->next;
            delete new_node;
            delete temp;
            collect((void*)merged_node->_start, merged_node->_size);
            delete merged_node;
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
            temp->pre = temp->next;
            delete new_node;
            delete temp;
            collect(merged_node->_start, merged_node->_size);
            delete merged_node;
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

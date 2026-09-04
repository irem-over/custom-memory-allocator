#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>
#include <stdint.h>

#define POOL_SIZE (4 * 1024 * 1024)
#define ALIGMENT 8

struct BlockHeader {
    size_t size;
    int is_free;
    struct BlockHeader *next;
};

int init_memory_pool(void);
void *my_malloc(size_t requested_size);
void my_free(void *target_pointer);
void dump_memory_state(void);

#endif
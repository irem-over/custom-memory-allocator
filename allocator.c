#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include "allocator.h"

static void *memory_pool_start = NULL;
static struct BlockHeader *free_list_head = NULL;

static size_t align_size(size_t raw_size) {
    return (raw_size + (ALIGMENT - 1)) & ~(ALIGMENT - 1);
}

int init_memory_pool(void) {
    memory_pool_start = mmap(
        NULL,
        POOL_SIZE,
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1,
        0
    );

    if (memory_pool_start == MAP_FAILED) {
        perror("mmap failed");
        return -1;
    }

    free_list_head = (struct BlockHeader *)memory_pool_start;
    free_list_head->size = POOL_SIZE - sizeof(struct BlockHeader);
    free_list_head->is_free = 1;
    free_list_head->next = NULL;

    return 0;
}

void *my_malloc(size_t requested_size) {
    if (requested_size == 0) {
        return NULL;
    }

    if (free_list_head == NULL) {
        if (init_memory_pool() == -1) {
            return NULL;
        }
    }

    size_t aligned_payload_size = align_size(requested_size);
    struct BlockHeader *current_block = free_list_head;

    while (current_block != NULL) {
        if (current_block->is_free && current_block->size >= aligned_payload_size) {
            size_t total_required = aligned_payload_size + sizeof(struct BlockHeader);
            
            if (current_block->size >= total_required + ALIGMENT) {
                uint8_t *block_byte_ptr = (uint8_t *)current_block;
                struct BlockHeader *new_split_block = (struct BlockHeader *)(block_byte_ptr + sizeof(struct BlockHeader) + aligned_payload_size);

                new_split_block->size = current_block->size - total_required;
                new_split_block->is_free = 1;
                new_split_block->next = current_block->next;

                current_block->size = aligned_payload_size;
                current_block->next = new_split_block;
            }

            current_block->is_free = 0;
            return (void *)((uint8_t *)current_block + sizeof(struct BlockHeader));
        }

        current_block = current_block->next;
    }

    return NULL;
}

void dump_memory_state(void) {
    struct BlockHeader *iterator = free_list_head;
    size_t block_counter = 0;

    printf("\n== Memory Pool State ==\n");

    while (iterator != NULL) {
        printf(
            "Block %zu | Addr: %p | Payload Size: %8zu bytes | Status: %s\n",
            block_counter,
            (void *)iterator,
            iterator->size,
            iterator->is_free ? "FREE" : "ALLOCATED"
        );

        iterator = iterator->next;
        block_counter++;
    }

    printf("=========================\n\n");
}


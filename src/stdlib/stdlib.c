#include "stdlib.h"

typedef struct BlockHeader {
    size_t size;
    int free;
    struct BlockHeader* next;
} BlockHeader;

#define ALIGN4(x) (((((x)-1)>>2)<<2)+4)

static uint8_t heap[HEAP_SIZE];
static size_t heap_used = 0;
static BlockHeader* free_list = NULL;

void* malloc(size_t size) {
    size = ALIGN4(size) + sizeof(BlockHeader);
    BlockHeader* curr = free_list;
    BlockHeader* prev = NULL;

    while (curr) {
        if (curr->free && curr->size >= size) {
            curr->free = 0;
            return (void*)(curr + 1);
        }
        prev = curr;
        curr = curr->next;
    }

    if (heap_used + size > HEAP_SIZE) {
        serial_write("Out of memory error! Free up some blocks", COM1);
        return NULL;
    }
    curr = (BlockHeader*)(heap + heap_used);
    curr->size = size;
    curr->free = 0;
    curr->next = NULL;
    heap_used += size;

    if (prev)
        prev->next = curr;
    else
        free_list = curr;

    return (void*)(curr + 1);
}

void free(void* ptr) {
    if (!ptr) return;
    BlockHeader* block = ((BlockHeader*)ptr) - 1;
    block->free = 1;
}
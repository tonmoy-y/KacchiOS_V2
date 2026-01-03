#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"   // 🔥 THIS WAS MISSING

#define STACK_SIZE 2048
#define MAX_STACKS 16

void  heap_init(uintptr_t start, size_t size);
void* kmalloc(size_t size);

void* stack_alloc(void);
void  stack_free(void* stack);

#endif

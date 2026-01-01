#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

#define STACK_SIZE 1024
#define MAX_STACKS 16

void* stack_alloc(void);
void stack_free(void* stack);

#endif

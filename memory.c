#include "memory.h"
#include <stdint.h>

static uint8_t stack_pool[MAX_STACKS][STACK_SIZE];
static int stack_used[MAX_STACKS] = {0};

void* stack_alloc(void) {
    for (int i = 0; i < MAX_STACKS; i++) {
        if (!stack_used[i]) {
            stack_used[i] = 1;
            return (void*)stack_pool[i];
        }
    }
    return NULL;
}

void stack_free(void* stack) {
    for (int i = 0; i < MAX_STACKS; i++) {
        if (stack == (void*)stack_pool[i]) {
            stack_used[i] = 0;
            return;
        }
    }
}

#include "memory.h"

static uint8_t stack_pool[MAX_STACKS][STACK_SIZE] __attribute__((aligned(16)));
static uint8_t stack_used[MAX_STACKS];

void* stack_alloc(void) {
	for (int i = 0; i < MAX_STACKS; i++) {
		if (!stack_used[i]) {
			stack_used[i] = 1;
			return stack_pool[i];
		}
	}
	return 0;
}

void stack_free(void* stack) {
	for (int i = 0; i < MAX_STACKS; i++) {
		if (stack_pool[i] == stack) {
			stack_used[i] = 0;
			return;
		}
	}
}

/* -------- heap -------- */
static uintptr_t heap_ptr;
static uintptr_t heap_end;

void heap_init(uintptr_t start, size_t size) {
	heap_ptr = start;
	heap_end = start + size;
}

void* kmalloc(size_t size) {
	if (heap_ptr + size >= heap_end)
		return 0;

	void* p = (void*)heap_ptr;
	heap_ptr += size;
	return p;
}

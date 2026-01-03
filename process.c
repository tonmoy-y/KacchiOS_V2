#include "process.h"
#include "memory.h"
#include <stddef.h>
#include <stdint.h>

static pcb_t process_table[MAX_PROCESS];
static pcb_t* current = NULL;
static int pid_counter = 1;

extern void scheduler_add(pcb_t* p);

void process_init() {
    for (int i = 0; i < MAX_PROCESS; i++) {
        process_table[i].pid = 0;
        process_table[i].state = TERMINATED;
    }
}

int create_process(void (*entry)()) {
    for (int i = 0; i < MAX_PROCESS; i++) {
        if (process_table[i].pid == 0) {
            pcb_t* p = &process_table[i];
            p->pid = pid_counter++;
            p->state = READY;
            p->stack = stack_alloc();

            p->context.sp = (uintptr_t)p->stack + STACK_SIZE;
            p->context.pc = (uintptr_t)entry;

            scheduler_add(p);
            return p->pid;
        }
    }
    return -1;
}

void terminate_process(int pid) {
    for (int i = 0; i < MAX_PROCESS; i++) {
        if (process_table[i].pid == pid) {
            process_table[i].state = TERMINATED;
            stack_free(process_table[i].stack);
            process_table[i].pid = 0;
            return;
        }
    }
}

pcb_t* get_current_process() {
    return current;
}

void set_current_process(pcb_t* p) {
    current = p;
}

pcb_t* get_process_table() {
    return process_table;
}

int get_max_process() {
    return MAX_PROCESS;
}
#include "process.h"
#include "memory.h"

static pcb_t process_table[MAX_PROCESS];
static pcb_t* current = 0;
static int next_pid = 1;

pcb_t* get_current_process(void) {
    return current;
}

void set_current_process(pcb_t* p) {
    current = p;
}

pcb_t* get_process_table(void) {
    return process_table;
}

pcb_t* create_process(void (*entry)(void)) {
    for (int i = 0; i < MAX_PROCESS; i++) {
        // Find empty slot (pid==0) or TERMINATED process that can be reused
        if (process_table[i].pid == 0 || process_table[i].state == PROC_TERMINATED) {

            void* stack = stack_alloc();
            if (!stack) return 0;

            pcb_t* p = &process_table[i];
            p->pid = next_pid++;
            p->state = PROC_READY;
            p->stack = stack;

            // stack grows downward, set initial sp to top of stack
            p->context.sp = (uintptr_t)stack + STACK_SIZE;
            p->context.pc = (uintptr_t)entry;

            return p;
        }
    }
    return 0;
}

void terminate_process(pcb_t* p) {
    if (!p) return;
    p->state = PROC_TERMINATED;
    if (p->stack) {
        stack_free(p->stack);
        p->stack = 0;
    }
    // Don't clear PID - keep it for ps to show TERMINATED state
    // PID will be overwritten when slot is reused
}

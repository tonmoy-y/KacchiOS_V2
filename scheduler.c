#include "scheduler.h"
#include "process.h"

static int last = -1;

extern void context_switch(uintptr_t* old_sp, uintptr_t new_sp, uintptr_t new_pc);

void schedule(void) {
    pcb_t* table = get_process_table();
    pcb_t* cur = get_current_process();

    for (int i = 1; i <= MAX_PROCESS; i++) {
        int idx = (last + i) % MAX_PROCESS;

        if (table[idx].pid != 0 && table[idx].state == PROC_READY) {
            pcb_t* next = &table[idx];
            last = idx;

            if (cur && cur->state == PROC_RUNNING) {
                cur->state = PROC_READY;
            }

            next->state = PROC_RUNNING;
            set_current_process(next);

            if (cur) {
                context_switch(&cur->context.sp,
                               next->context.sp,
                               next->context.pc);
            } else {
                context_switch(0,
                               next->context.sp,
                               next->context.pc);
            }
            return;
        }
    }

    /* idle */
    set_current_process(0);
}

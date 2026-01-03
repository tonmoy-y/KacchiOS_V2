#include "scheduler.h"
#include "process.h"

static int last = 0;

// context_switch(old_sp_ptr, new_sp, new_pc)
extern void context_switch(uintptr_t* old_sp, uintptr_t new_sp, uintptr_t new_pc);

void schedule(void) {
    pcb_t* table = get_process_table();
    pcb_t* cur = get_current_process();

    // Find next READY process (round robin)
    for (int i = 0; i < MAX_PROCESS; i++) {
        int idx = (last + i + 1) % MAX_PROCESS;

        if (table[idx].state == PROC_READY) {
            pcb_t* next = &table[idx];
            last = idx;

            // mark states
            if (cur && cur->state == PROC_RUNNING) {
                cur->state = PROC_READY;
            }
            next->state = PROC_RUNNING;

            // IMPORTANT: set current pointer before switching
            set_current_process(next);

            if (cur) {
                context_switch(&cur->context.sp, next->context.sp, next->context.pc);
            } else {
                // first time: no current process to save
                context_switch(0, next->context.sp, next->context.pc);
            }
            return;
        }
    }

    // If no READY process found, do nothing (idle)
}

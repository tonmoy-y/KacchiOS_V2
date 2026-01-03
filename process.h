#ifndef PROCESS_H
#define PROCESS_H

#include "types.h"

#define MAX_PROCESS 8

typedef enum {
    PROC_READY = 0,
    PROC_RUNNING = 1,
    PROC_TERMINATED = 2
} proc_state_t;

typedef struct {
    uintptr_t sp;
    uintptr_t pc;
} context_t;

typedef struct {
    int pid;
    proc_state_t state;
    void* stack;
    context_t context;
} pcb_t;

pcb_t* create_process(void (*entry)(void));
void terminate_process(pcb_t* p);

pcb_t* get_current_process(void);
void   set_current_process(pcb_t* p);

pcb_t* get_process_table(void);

#endif

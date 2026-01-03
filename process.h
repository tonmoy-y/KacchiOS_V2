#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include <stddef.h>

#define MAX_PROCESS 16

typedef enum {
    CURRENT,
    READY,
    TERMINATED
} process_state_t;

typedef struct context {
    uintptr_t sp;
    uintptr_t pc;
} context_t;

typedef struct pcb {
    int pid;
    process_state_t state;
    void* stack;
    context_t context;
    struct pcb* next;
} pcb_t;

void process_init();
int create_process(void (*entry)());
void terminate_process(int pid);
pcb_t* get_current_process();
void set_current_process(pcb_t* p);
pcb_t* get_process_table();
int get_max_process();

#endif

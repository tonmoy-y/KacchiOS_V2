#include "types.h"
#include "serial.h"
#include "memory.h"
#include "process.h"
#include "scheduler.h"

/* ---------- helpers ---------- */

static void puts_nl(const char* s) {
    serial_puts(s);
    serial_puts("\n");
}

static void prompt(void) {
    serial_puts("\n[kacchiOS] > ");
}

/* minimal string helpers */
static int strcmp_local(const char* a, const char* b) {
    while (*a && *b) {
        if (*a != *b) return *a - *b;
        a++; b++;
    }
    return *a - *b;
}

static int strncmp_local(const char* a, const char* b, int n) {
    while (n-- > 0) {
        char ca = *a++;
        char cb = *b++;
        if (ca != cb) return ca - cb;
        if (ca == 0) return 0;
    }
    return 0;
}

/* ---------- serial input ---------- */

static int read_line_nonblock(char* buf, int max, int* pos) {
    while (serial_available()) {
        if (*pos >= max - 1) {
            buf[*pos] = 0;
            serial_puts("\n");
            *pos = 0;
            return 1;
        }

        char c = serial_getc();

        if (c == '\r' || c == '\n') {
            buf[*pos] = 0;
            serial_puts("\n");
            *pos = 0;
            return 1;
        }

        buf[(*pos)++] = c;
        serial_putc(c);
    }
    return 0;
}

/* ---------- background process ---------- */

static volatile int demo_run = 0;

static void bg_process(void) {
    volatile int count = 0;
    while (demo_run) {
        /* Simulate work by counting */
        count++;
    }
    terminate_process(get_current_process());
    for (;;);
}

/* ---------- commands ---------- */

static void cmd_help(void) {
    puts_nl("Commands:");
    puts_nl("  help");
    puts_nl("  ps");
    puts_nl("  demo start");
    puts_nl("  demo stop");
}

static void cmd_ps(void) {
    pcb_t* t = get_process_table();

    puts_nl("PID  STATE");
    for (int i = 0; i < MAX_PROCESS; i++) {
        if (t[i].pid == 0) continue;

        serial_puts(" ");
        
        // Print PID (handle multiple digits)
        int pid = t[i].pid;
        if (pid >= 10) {
            serial_putc('0' + (pid / 10));
            serial_putc('0' + (pid % 10));
        } else {
            serial_putc('0' + pid);
        }
        
        serial_puts("   ");

        if (t[i].state == PROC_READY) serial_puts("READY");
        else if (t[i].state == PROC_RUNNING) serial_puts("RUNNING");
        else serial_puts("TERMINATED");

        serial_puts("\n");
    }
}

static void cmd_demo_start(void) {
    if (demo_run) {
        puts_nl("[demo] already running");
        return;
    }

    demo_run = 1;
    create_process(bg_process);
    create_process(bg_process);

    puts_nl("[demo] background processes started");
}

static void cmd_demo_stop(void) {
    demo_run = 0;
    
    /* Terminate all running processes */
    pcb_t* table = get_process_table();
    for (int i = 0; i < MAX_PROCESS; i++) {
        if (table[i].pid != 0 && table[i].state != PROC_TERMINATED) {
            terminate_process(&table[i]);
        }
    }
    
    puts_nl("[demo] stopping background processes");
}

/* ---------- kernel entry ---------- */

void kmain(void) {
    serial_init();

    serial_puts("\n====================================\n");
    serial_puts("         kacchiOS - booted\n");
    serial_puts("====================================\n");

    heap_init(0x00100000, 0x00010000);

    cmd_help();
    prompt();

    char line[64];
    int line_pos = 0;
    int sched_counter = 0;
    int state_flip = 0;

    for (;;) {
        /* Simulate scheduling by flipping process states */
        if (++sched_counter > 5000) {
            pcb_t* table = get_process_table();
            
            /* Toggle states between READY and RUNNING for demo (only if demo is running) */
            if (demo_run) {
                for (int i = 0; i < MAX_PROCESS; i++) {
                    if (table[i].pid != 0 && table[i].state != PROC_TERMINATED) {
                        if (state_flip % 2 == i % 2) {
                            table[i].state = PROC_RUNNING;
                        } else {
                            table[i].state = PROC_READY;
                        }
                    }
                }
                state_flip++;
            }
            sched_counter = 0;
        }

        /* Non-blocking input check */
        if (read_line_nonblock(line, sizeof(line), &line_pos)) {
            /* Command received */
            if (strcmp_local(line, "help") == 0) cmd_help();
            else if (strcmp_local(line, "ps") == 0) cmd_ps();
            else if (strcmp_local(line, "demo start") == 0) cmd_demo_start();
            else if (strcmp_local(line, "demo stop") == 0) cmd_demo_stop();
            else if (line[0] != 0) puts_nl("[error] unknown command");

            prompt();
        }
    }
}

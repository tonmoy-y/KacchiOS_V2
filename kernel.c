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

static int read_line(char* buf, int max) {
    int i = 0;
    while (i < max - 1) {
        char c = serial_getc();

        if (c == '\r' || c == '\n') {
            buf[i] = 0;
            serial_puts("\n");
            return i;
        }

        buf[i++] = c;
        serial_putc(c);
    }
    buf[i] = 0;
    serial_puts("\n");
    return i;
}

/* ---------- background process ---------- */

static volatile int demo_run = 0;

static void bg_process(void) {
    while (demo_run) {
        /* do nothing, just alive */
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
        serial_putc('0' + t[i].pid);
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
    int auto_tick = 0;

    for (;;) {
        /* soft auto scheduling */
        if (++auto_tick > 500000) {
            schedule();      // <-- auto yield
            auto_tick = 0;
        }

        read_line(line, sizeof(line));

        if (strcmp_local(line, "help") == 0) cmd_help();
        else if (strcmp_local(line, "ps") == 0) cmd_ps();
        else if (strcmp_local(line, "demo start") == 0) cmd_demo_start();
        else if (strcmp_local(line, "demo stop") == 0) cmd_demo_stop();
        else puts_nl("[error] unknown command");

        prompt();
    }
}

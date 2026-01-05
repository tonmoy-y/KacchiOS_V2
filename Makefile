# Makefile for kacchiOS
CC = gcc
LD = ld
AS = as

CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -nostdinc \
         -fno-builtin -fno-stack-protector -I.
ASFLAGS = --32
LDFLAGS = -m elf_i386

# add: memory/process/scheduler + switch (context switch)
OBJS = boot.o kernel.o serial.o string.o memory.o process.o scheduler.o switch.o

all: kernel.elf

kernel.elf: $(OBJS)
	$(LD) $(LDFLAGS) -T link.ld -o $@ $^

# C files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Assembly files (AT&T syntax .S via gcc preprocessor)
%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

# Raw assembly (.s) via as (if you ever use)
%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

run: kernel.elf
	qemu-system-i386 -kernel kernel.elf -m 64M -serial stdio -display none

run-vga: kernel.elf
	qemu-system-i386 -kernel kernel.elf -m 64M -serial mon:stdio

debug: kernel.elf
	qemu-system-i386 -kernel kernel.elf -m 64M -serial stdio -display none -s -S &
	@echo "Waiting for GDB connection on port 1234..."
	@echo "In another terminal run: gdb -ex 'target remote localhost:1234' -ex 'symbol-file kernel.elf'"

clean:
	rm -f *.o kernel.elf

.PHONY: all run run-vga debug clean

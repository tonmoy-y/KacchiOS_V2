# kacchiOS

kacchiOS is a bare-metal operating system developed as part of the  
**CSE 3202 (Operating Systems Sessional)** course at  
**Rajshahi University of Engineering & Technology (RUET)**.

The base operating system was provided with basic booting capability and a Null Process.  
This project extends the base system by implementing core operating system components.

---

## Project Objectives

The objective of this project is to understand and implement fundamental operating system concepts by extending a minimal bare-metal OS.

The following components were required:
- Memory Management
- Process Management
- CPU Scheduling

---

## Implemented Components

### Memory Manager
The memory manager handles memory allocation and deallocation for processes.

- Stack memory allocation
- Heap memory allocation
- Stack deallocation on process termination
- Heap deallocation on process termination

**Files:** `memory.c`, `memory.h`

---

### Process Manager
The process manager controls the lifecycle of processes in the system.

- Process Control Block (PCB)
- Global process table
- Process creation
- Process state transitions
- Process termination

**Process States:** `CURRENT`, `READY`, `TERMINATED`  

**Files:** `process.c`, `process.h`

---

### Scheduler
The scheduler determines which process gets CPU time.

- Simple round-robin style scheduling
- Context switching between processes
- Low-level context switching using assembly

**Files:** `scheduler.c`, `scheduler.h`, `switch.s`

---

## Boot and Kernel Execution Flow

- System bootstrapping starts from `boot.S`
- Kernel initialization is handled in `kernel.c`
- Scheduler starts process execution after kernel setup

---

## Build Instructions

### Prerequisites

```bash
# On Ubuntu/Debian
sudo apt-get install build-essential qemu-system-x86 gcc-multilib

# On Arch Linux
sudo pacman -S base-devel qemu gcc-multilib

# On macOS
brew install qemu i686-elf-gcc
```

### Build and Run

```bash
# Clone the repository
git clone https://github.com/tonmoy-y/KacchiOS_V2.git
cd kacchiOS

# Build the OS
make clean
make

# Run in QEMU
make run
```

You should see:
```
========================================
    kacchiOS - Minimal Baremetal OS
========================================
Hello from kacchiOS!
Running null process...

kacchiOS> 
```

Type something and press Enter - it will echo back!

## 📁 Project Structure

```
kacchiOS/
├── boot.S          # Bootloader entry point (Assembly)
├── kernel.c        # Main kernel (null process)
├── serial.c        # Serial port driver (COM1)
├── serial.h        # Serial driver interface
├── string.c        # String utility functions
├── string.h        # String utility interface
├── types.h         # Basic type definitions
├── io.h            # I/O port operations
├── link.ld         # Linker script
├── Makefile        # Build system
└── README.md       # This file
```

## 🛠️ Build System

### Makefile Targets

| Command | Description |
|---------|-------------|
| `make` or `make all` | Build kernel.elf |
| `make run` | Run in QEMU (serial output only) |
| `make run-vga` | Run in QEMU (with VGA window) |
| `make debug` | Run in debug mode (GDB ready) |
| `make clean` | Remove build artifacts |

## 📚 Learning Resources

### Recommended Reading

- [XINU OS](https://xinu.cs.purdue.edu/) - Educational OS similar to kacchiOS
- [OSDev Wiki](https://wiki.osdev.org/) - Comprehensive OS development guide
- [The Little OS Book](https://littleosbook.github.io/) - Practical OS development
- [Operating Systems: Three Easy Pieces](https://pages.cs.wisc.edu/~remzi/OSTEP/) - OS concepts textbook

### Related Topics

- x86 Assembly Language
- Memory Management
- Process Scheduling
- System Calls
- Interrupt Handling

## 🤝 Contributing

Contributions are welcome! Please feel free to submit issues and pull requests.

### Guidelines

1. Keep code simple and educational
2. Add comments explaining complex concepts
3. Follow existing code style
4. Test changes in QEMU before submitting

## 📄 License

This project is licensed under the MIT License.

## 👨‍🏫 About

kacchiOS was created as an educational tool for teaching operating system concepts. It provides a minimal, working foundation that students can extend to learn core OS principles through hands-on implementation.

## 🙏 Acknowledgments

- Inspired by XINU OS
- Built with guidance from OSDev community
- Thanks to all students who have contributed


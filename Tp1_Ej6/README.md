# ARM Microkernel

A small bare-metal microkernel developed from scratch for an ARM Cortex-A8 platform as part of the Digital Techniques III course at UTN.

The project focuses on low-level programming, ARM architecture, multitasking, interrupt handling, memory management and user/kernel privilege separation.

## 🚀 Features

- Multiple concurrent tasks
- Round-robin scheduler
- Timer-based context switching
- ARM exception handling
- IRQ handling
- Supervisor Calls (SVC)
- Generic Interrupt Controller (GIC)
- ARM Timer
- User and privileged execution modes
- Memory Management Unit (MMU)
- Virtual memory
- Per-task address spaces
- TTBR0 configuration
- Task-specific stacks
- Context switching
- C and ARM Assembly integration
- Custom linker scripts

## 🧠 System Architecture

The microkernel runs directly on an ARM Cortex-A8 processor without relying on an operating system.

The system is divided into several low-level components:

- Task management
- Scheduler
- Context switching
- Interrupt management
- Exception handling
- Memory management
- User/kernel mode separation

Hardware interrupts are handled through the ARM Generic Interrupt Controller (GIC), while a hardware timer provides periodic interrupts used by the scheduler.

## ⏱️ Task Scheduling

The kernel implements a round-robin scheduling algorithm to manage multiple concurrent tasks.

A hardware timer periodically generates an interrupt. The kernel saves the context of the currently running task and switches execution to another task selected by the scheduler.

This allows multiple tasks to execute concurrently on a single processor.

## ⚡ Interrupts and Exceptions

The microkernel implements ARM exception handling for different execution events, including:

- IRQ exceptions
- Supervisor Calls (SVC)

The Generic Interrupt Controller (GIC) is configured to manage hardware interrupts, while the timer provides the periodic interrupts required for task scheduling.

## 🔐 User and Kernel Modes

Tasks can execute in user mode while privileged operations are handled by the kernel.

SVC instructions provide an interface between user-level tasks and privileged kernel functionality.

This separation allows the system to distinguish between unprivileged application execution and privileged kernel operations.

## 💾 Memory Management

The ARM Memory Management Unit (MMU) is used to implement virtual memory and provide separate address spaces for the different tasks.

Each task is configured with its own translation table using the ARM Translation Table Base Register (TTBR0).

Custom linker scripts are used to control the virtual and physical memory layout of the kernel, tasks and stacks.

## 🔄 Context Switching

The kernel implements task context switching to save the processor state of the currently executing task and restore the state of the next scheduled task.

The context switching mechanism integrates C code with ARM Assembly to directly manipulate the processor state and execution context.

## 🛠️ Technologies

- C
- ARM Assembly
- ARM Cortex-A8
- ARM MMU
- ARM GIC
- ARM Timer
- Virtual Memory
- Linker Scripts
- Bare-metal programming

## 📂 Project Structure

```text
ARM-Microkernel/
├── inc/        # Header files
├── src/        # Kernel source code
├── Makefile    # Build configuration
└── memmap.ld   # Memory layout and linker configuration

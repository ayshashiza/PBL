# Multi-Process Simulator & IPC

## Overview

This project implements a multi-process simulator using three independent processes:

- **UI Process** – Handles user commands and interaction.
- **Core Process** – Handles CPU execution, memory, stack and queue operations.
- **Logger Process** – Records simulator activities and events.

The processes communicate using **POSIX Message Queues (IPC)**.

## System Architecture

```text
                 User
                  |
                  v
          +---------------+
          |   UI Process  |
          |    (ui.c)     |
          +-------+-------+
                  |
          POSIX Message Queue
                  |
                  v
          +---------------+
          | Core Process  |
          |   (core.c)    |
          +-------+-------+
                  |
          POSIX Message Queue
                  |
                  v
          +---------------+
          | Logger Process|
          |  (logger.c)  |
          +---------------+

Core handles:
- CPU execution
- Memory
- Stack
- Queue
IPC Mechanism
POSIX Message Queues
POSIX Message Queues were selected because they provide structured communication between independent processes.
Advantages:
- Easy message-based communication
- Processes remain independent
- Supports asynchronous communication
- Suitable for command-based simulator interaction
- Provides synchronization between processes
Project Structure
week2/
├── core/
│   └── core.c
├── ui/
│   └── ui.c
├── logger/
│   └── logger.c
├── ipc.h
├── .gitignore
└── README.md

Available Commands
The UI supports:
help
load
run
step
reg
mem
stack
queue
exit

Testing
The complete system was tested using three separate processes.
UI → Core Communication
Commands such as load, run, step, reg, mem, stack and queue were successfully sent from the UI process to the Core process.
Core → Logger Communication
The Logger successfully recorded events including:
Program loaded
Program executed
Registers requested
Single step executed
Memory requested
Stack requested
Queue requested
EXIT

Sample Execution
Program loaded successfully.
Program executed. ACC=100 PC=4
Step executed. ACC=100 PC=4
Memory displayed in Core.
Stack displayed in Core.
Queue displayed in Core.

Compilation
Compile the three processes using:
gcc -Wall -Wextra -I. -o core/core core/core.c -lrt
gcc -Wall -Wextra -I. -o ui/ui ui/ui.c -lrt
gcc -Wall -Wextra -I. -o logger/logger logger/logger.c -lrt

Running the Simulator
Start the Core process:
./core/core

Start the Logger process in another terminal:
./logger/logger

Start the UI process in another terminal:
./ui/ui

Then use the available commands through the UI.
Conclusion
The simulator successfully demonstrates a three-process architecture using POSIX IPC. The UI, Core and Logger processes operate independently and communicate through message queues.
The complete flow is:
UI → Core → CPU/Memory/Stack/Queue → Logger




## IPC Selection and Justification

POSIX Message Queues were selected as the IPC mechanism for this project.

Reasons:
- Provides communication between independent processes.
- Supports structured messages between UI, Core, and Logger.
- Allows asynchronous communication.
- Provides synchronization through message-based communication.
- Suitable for a multi-process simulator because processes remain independent.

Communication Flow:

UI Process -> POSIX Message Queue -> Core Process
Core Process -> POSIX Message Queue -> Logger Process

The UI sends user commands to the Core process. The Core performs the required
CPU, memory, stack, and queue operations. Important events are sent to the
Logger process for recording.

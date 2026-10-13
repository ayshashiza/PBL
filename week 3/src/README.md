# CPU Scheduling Simulator

A C command-line program that simulates CPU scheduling algorithms. It accepts process details, displays a Gantt chart, and reports scheduling metrics.

## Scheduling Algorithms

- FCFS (First-Come, First-Served)
- SJF Non-Preemptive
- SJF Preemptive
- Priority Non-Preemptive
- Priority Preemptive
- Round Robin with a configurable time quantum

## Process Information

For each process, enter its arrival time, burst time, and priority. For Round Robin, also enter the time quantum. Priority does not affect Round Robin scheduling.

## Requirements

- Ubuntu or another Linux environment
- GCC compiler

## Compile and Run

From the repository folder, compile the simulator:

```bash
gcc -Wall -Wextra \
  "week 3/src/main.c" \
  "week 3/src/fcfs.c" \
  "week 3/src/Sjf.c" \
  "week 3/src/priority.c" \
  "week 3/src/Roundrobin.c" \
  -o scheduler

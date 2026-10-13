# Week 3: Process Scheduling Simulator

## Objective

This project simulates CPU process scheduling algorithms. It accepts process details and displays scheduling results, including a Gantt chart and performance metrics.

## Scheduling Algorithms

- FCFS (First-Come, First-Served)
- SJF Non-Preemptive
- SJF Preemptive
- Priority Non-Preemptive
- Priority Preemptive
- Round Robin with a configurable time quantum

## Input Data

The following sample processes were used:

| Process | Arrival Time | Burst Time | Priority |
|---|---:|---:|---:|
| P1 | 0 | 5 | 1 |
| P2 | 1 | 3 | 1 |
| P3 | 2 | 1 | 1 |

For Round Robin, the time quantum was `2`.

> All priorities are equal in this example, so it does not demonstrate how different priorities affect scheduling.

## Test Case 1: Sample Scheduling Results

The charts and metrics below are expected results for the sample input. Verify them against the simulator’s actual output before submission.

### FCFS

**Expected Gantt chart:** `0 | P1 | 5 | P2 | 8 | P3 | 9`

| Process | Completion Time | Waiting Time | Turnaround Time | Response Time |
|---|---:|---:|---:|---:|
| P1 | 5 | 0 | 5 | 0 |
| P2 | 8 | 4 | 7 | 4 |
| P3 | 9 | 6 | 7 | 6 |

- Average Waiting Time: **3.33**
- Average Turnaround Time: **6.33**
- Average Response Time: **3.33**
- CPU Utilization: **100%**

### SJF Non-Preemptive

**Expected Gantt chart:** `0 | P1 | 5 | P3 | 6 | P2 | 9`

| Process | Completion Time | Waiting Time | Turnaround Time | Response Time |
|---|---:|---:|---:|---:|
| P1 | 5 | 0 | 5 | 0 |
| P2 | 9 | 5 | 8 | 5 |
| P3 | 6 | 3 | 4 | 3 |

- Average Waiting Time: **2.67**
- Average Turnaround Time: **5.67**
- Average Response Time: **2.67**
- CPU Utilization: **100%**

### SJF Preemptive

**Expected Gantt chart:** `0 | P1 | 1 | P2 | 2 | P3 | 3 | P2 | 5 | P1 | 9`

| Process | Completion Time | Waiting Time | Turnaround Time | Response Time |
|---|---:|---:|---:|---:|
| P1 | 9 | 4 | 9 | 0 |
| P2 | 5 | 1 | 4 | 0 |
| P3 | 3 | 0 | 1 | 0 |

- Average Waiting Time: **1.67**
- Average Turnaround Time: **4.67**
- Average Response Time: **0.00**
- CPU Utilization: **100%**

### Priority Non-Preemptive

Since every process has the same priority, a scheduler that breaks ties by arrival order should behave like FCFS for this input.

**Expected Gantt chart:** `0 | P1 | 5 | P2 | 8 | P3 | 9`

- Average Waiting Time: **3.33**
- Average Turnaround Time: **6.33**
- Average Response Time: **3.33**
- CPU Utilization: **100%**

### Priority Preemptive

Since every process has the same priority, a scheduler that breaks ties by arrival order should behave like FCFS for this input.

**Expected Gantt chart:** `0 | P1 | 5 | P2 | 8 | P3 | 9`

- Average Waiting Time: **3.33**
- Average Turnaround Time: **6.33**
- Average Response Time: **3.33**
- CPU Utilization: **100%**

### Round Robin

**Time Quantum:** `2`

**Gantt chart:** `0 | P1 | 2 | P2 | 4 | P3 | 5 | P1 | 7 | P2 | 8 | P1 | 9`

| Process | Completion Time | Waiting Time | Turnaround Time | Response Time |
|---|---:|---:|---:|---:|
| P1 | 9 | 4 | 9 | 0 |
| P2 | 8 | 4 | 7 | 1 |
| P3 | 5 | 2 | 3 | 2 |

- Average Waiting Time: **3.33**
- Average Turnaround Time: **6.33**
- Average Response Time: **1.00**
- CPU Utilization: **100%**

The Round Robin averages match the simulator output shared during testing.

## Test Cases 2 and 3

The assignment requires sample output and Gantt charts for at least three test cases. Add the actual simulator output for two more input sets here.

### Test Case 2

- Input processes, priorities, and algorithm/time quantum: _Add the values used._
- Gantt chart: _Add the chart printed by the simulator._
- Average Waiting Time: _Add the simulator result._
- Average Turnaround Time: _Add the simulator result._
- Average Response Time: _Add the simulator result, if displayed._
- CPU Utilization: _Add the simulator result, if available._

### Test Case 3

- Input processes, priorities, and algorithm/time quantum: _Add the values used._
- Gantt chart: _Add the chart printed by the simulator._
- Average Waiting Time: _Add the simulator result._
- Average Turnaround Time: _Add the simulator result._
- Average Response Time: _Add the simulator result, if displayed._
- CPU Utilization: _Add the simulator result, if available._

## Observations

For the sample input, SJF Non-Preemptive has the lowest expected average waiting time. SJF Preemptive has the lowest expected average turnaround time and response time for this workload. FCFS and both priority algorithms are expected to follow the same process order because all three priorities are equal. Round Robin gives each ready process CPU time in turns according to the time quantum.

Use different priority values in another test case to demonstrate priority scheduling.

## Requirements Checklist

- [x] Accept process ID, arrival time, burst time, and priority.
- [x] Provide FCFS, both SJF variants, both priority variants, and Round Robin.
- [x] Accept a configurable time quantum for Round Robin.
- [x] Display a Gantt chart and per-process WT, TAT, and RT.
- [ ] Include actual output and Gantt charts for at least three test cases.
- [ ] Confirm CPU utilization is displayed or reported.
- [ ] Verify the expected results above against the simulator output for algorithms 1–5.

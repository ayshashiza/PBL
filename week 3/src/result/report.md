# Week 3 Project Report: CPU Scheduling Simulator

## 1. Objective

The objective of this project is to create a CPU scheduling simulator in C. The simulator accepts process details and runs different scheduling algorithms. It displays a Gantt chart and calculates scheduling metrics so their results can be compared.

## 2. Scheduling Algorithms

The simulator provides:

- First-Come, First-Served (FCFS)
- Shortest Job First (SJF), non-preemptive
- Shortest Job First (SJF), preemptive
- Priority Scheduling, non-preemptive
- Priority Scheduling, preemptive
- Round Robin (RR), with a configurable time quantum

## 3. Input and Metrics

For each process, the user enters its process ID, arrival time, burst time, and priority. For Round Robin, the user also enters a time quantum.

The simulator reports a Gantt chart and process scheduling metrics, including waiting time (WT), turnaround time (TAT), and response time (RT).

## 4. Test Case 1

### Input

| Process | Arrival Time | Burst Time | Priority |
|---|---:|---:|---:|
| P1 | 0 | 5 | 1 |
| P2 | 1 | 3 | 1 |
| P3 | 2 | 1 | 1 |

All priorities are equal in this test. Therefore, this test does not show how different priority values affect process ordering.

### FCFS

**Expected Gantt chart:** `0 | P1 | 5 | P2 | 8 | P3 | 9`

| Process | Completion Time | WT | TAT | RT |
|---|---:|---:|---:|---:|
| P1 | 5 | 0 | 5 | 0 |
| P2 | 8 | 4 | 7 | 4 |
| P3 | 9 | 6 | 7 | 6 |

**Expected averages:** WT = 3.33, TAT = 6.33, RT = 3.33

### SJF Non-Preemptive

**Expected Gantt chart:** `0 | P1 | 5 | P3 | 6 | P2 | 9`

| Process | Completion Time | WT | TAT | RT |
|---|---:|---:|---:|---:|
| P1 | 5 | 0 | 5 | 0 |
| P2 | 9 | 5 | 8 | 5 |
| P3 | 6 | 3 | 4 | 3 |

**Expected averages:** WT = 2.67, TAT = 5.67, RT = 2.67

### SJF Preemptive

**Expected Gantt chart:** `0 | P1 | 1 | P2 | 2 | P3 | 3 | P2 | 5 | P1 | 9`

| Process | Completion Time | WT | TAT | RT |
|---|---:|---:|---:|---:|
| P1 | 9 | 4 | 9 | 0 |
| P2 | 5 | 1 | 4 | 0 |
| P3 | 3 | 0 | 1 | 0 |

**Expected averages:** WT = 1.67, TAT = 4.67, RT = 0.00

### Priority Non-Preemptive

All processes have equal priority. A scheduler that breaks ties by arrival order is expected to behave like FCFS for this input.

**Expected Gantt chart:** `0 | P1 | 5 | P2 | 8 | P3 | 9`

**Expected averages:** WT = 3.33, TAT = 6.33, RT = 3.33

### Priority Preemptive

All processes have equal priority. A scheduler that breaks ties by arrival order is expected to behave like FCFS for this input.

**Expected Gantt chart:** `0 | P1 | 5 | P2 | 8 | P3 | 9`

**Expected averages:** WT = 3.33, TAT = 6.33, RT = 3.33

### Round Robin

**Time quantum:** 2

**Gantt chart:** `0 | P1 | 2 | P2 | 4 | P3 | 5 | P1 | 7 | P2 | 8 | P1 | 9`

| Process | Completion Time | WT | TAT | RT |
|---|---:|---:|---:|---:|
| P1 | 9 | 4 | 9 | 0 |
| P2 | 8 | 4 | 7 | 1 |
| P3 | 5 | 2 | 3 | 2 |

**Simulator output:** Average WT = 3.33; Average TAT = 6.33

These Round Robin averages match the output observed during testing.

## 5. Test Case 2

**Algorithm tested:** [Enter algorithm name]

| Process | Arrival Time | Burst Time | Priority |
|---|---:|---:|---:|
| P1 | [value] | [value] | [value] |
| P2 | [value] | [value] | [value] |
| P3 | [value] | [value] | [value] |

**Time quantum, if using Round Robin:** [value or N/A]

**Gantt chart:** [Copy the chart printed by the simulator]

**Average WT:** [Copy simulator result]  
**Average TAT:** [Copy simulator result]  
**Average RT:** [Copy simulator result, if displayed]

## 6. Test Case 3

**Algorithm tested:** [Enter algorithm name]

| Process | Arrival Time | Burst Time | Priority |
|---|---:|---:|---:|
| P1 | [value] | [value] | [value] |
| P2 | [value] | [value] | [value] |
| P3 | [value] | [value] | [value] |

**Time quantum, if using Round Robin:** [value or N/A]

**Gantt chart:** [Copy the chart printed by the simulator]

**Average WT:** [Copy simulator result]  
**Average TAT:** [Copy simulator result]  
**Average RT:** [Copy simulator result, if displayed]

## 7. Observations

For Test Case 1, the expected SJF non-preemptive result has a lower average waiting time than the expected FCFS result. The expected SJF preemptive result has lower average waiting, turnaround, and response times for this set of processes. Round Robin gives each ready process a turn according to the selected time quantum.

The priority algorithms cannot be meaningfully compared using Test Case 1 because every process has the same priority. A test with different priority values should be included to demonstrate priority scheduling.

## 8. Conclusion

The simulator demonstrates how CPU scheduling algorithms choose processes and how those choices affect waiting time, turnaround time, and response time. The Gantt chart helps show the execution order and duration of each process.

## 9. Items to Verify Before Submission

- Replace the Test Case 2 and Test Case 3 placeholders with actual simulator runs.
- Compare the expected FCFS, SJF, and Priority results above with the program’s printed output.
- Use different priority values in at least one test.
- Confirm whether the project reports CPU utilization. If it does, include the actual result for each test case.
- Include screenshots showing the actual Gantt charts and output.

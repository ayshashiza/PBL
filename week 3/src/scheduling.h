#ifndef SCHEDULING_H
#define SCHEDULING_H

#define MAX_PROCESSES 100

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int priority;

    int completion_time;
    int waiting_time;
    int turnaround_time;
    int response_time;
    int remaining_time;
} Process;

/* Scheduling algorithm functions */

void fcfs(Process processes[], int n);

void sjf_nonpreemptive(Process processes[], int n);

void sjf_preemptive(Process processes[], int n);

void priority_nonpreemptive(Process processes[], int n);

void priority_preemptive(Process processes[], int n);

void round_robin(Process processes[], int n, int quantum);

#endif

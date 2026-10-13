#include <stdio.h>
#include "scheduling.h"

/* SJF Non-Preemptive */
void sjf_nonpreemptive(Process processes[], int n)
{
    int done[MAX_PROCESSES] = {0};
    int time = 0;
    int completed = 0;
    double total_wt = 0;
    double total_tat = 0;

    printf("\n--- SJF Non-Preemptive ---\n");
    printf("Gantt Chart:\n");

    while (completed < n) {
        int selected = -1;

        for (int i = 0; i < n; i++) {
            if (!done[i] &&
                processes[i].arrival_time <= time) {
                if (selected == -1 ||
                    processes[i].burst_time <
                    processes[selected].burst_time) {
                    selected = i;
                }
            }
        }

        if (selected == -1) {
            int next = -1;

            for (int i = 0; i < n; i++) {
                if (!done[i] &&
                    (next == -1 ||
                     processes[i].arrival_time <
                     processes[next].arrival_time)) {
                    next = i;
                }
            }

            printf("| IDLE (%d-%d) ", time,
                   processes[next].arrival_time);
            time = processes[next].arrival_time;
            continue;
        }

        Process *p = &processes[selected];
        int start = time;

        p->response_time = start - p->arrival_time;
        time += p->burst_time;

        p->completion_time = time;
        p->turnaround_time = time - p->arrival_time;
        p->waiting_time = p->turnaround_time - p->burst_time;
        p->remaining_time = 0;

        printf("| P%d (%d-%d) ", p->pid, start, time);

        total_wt += p->waiting_time;
        total_tat += p->turnaround_time;

        done[selected] = 1;
        completed++;
    }

    printf("|\n");
    printf("\nPID\tAT\tBT\tPriority\tCT\tWT\tTAT\tRT\n");

    for (int i = 0; i < n; i++) {
        Process *p = &processes[i];

        printf("P%d\t%d\t%d\t%d\t\t%d\t%d\t%d\t%d\n",
               p->pid, p->arrival_time, p->burst_time,
               p->priority, p->completion_time,
               p->waiting_time, p->turnaround_time,
               p->response_time);
    }

    printf("\nAverage Waiting Time: %.2f\n", total_wt / n);
    printf("Average Turnaround Time: %.2f\n", total_tat / n);
}

/* SJF Preemptive: Shortest Remaining Time First */
void sjf_preemptive(Process processes[], int n)
{
    int time = 0;
    int completed = 0;
    double total_wt = 0;
    double total_tat = 0;
    int last_pid = -1;
    int segment_start = 0;

    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].response_time = -1;
    }

    printf("\n--- SJF Preemptive (SRTF) ---\n");
    printf("Gantt Chart:\n");

    while (completed < n) {
        int selected = -1;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= time &&
                processes[i].remaining_time > 0) {
                if (selected == -1 ||
                    processes[i].remaining_time <
                    processes[selected].remaining_time) {
                    selected = i;
                }
            }
        }

        int current_pid = (selected == -1)
                          ? -1 : processes[selected].pid;

        if (current_pid != last_pid) {
            if (last_pid != -1)
                printf("P%d (%d-%d) | ",
                       last_pid, segment_start, time);
            else if (last_pid == -1 && time > segment_start)
                printf("IDLE (%d-%d) | ",
                       segment_start, time);

            segment_start = time;
            last_pid = current_pid;
        }

        if (selected == -1) {
            time++;
            continue;
        }

        Process *p = &processes[selected];

        if (p->response_time == -1)
            p->response_time = time - p->arrival_time;

        p->remaining_time--;
        time++;

        if (p->remaining_time == 0) {
            p->completion_time = time;
            p->turnaround_time = time - p->arrival_time;
            p->waiting_time = p->turnaround_time - p->burst_time;
            completed++;
        }
    }

    if (last_pid != -1)
        printf("P%d (%d-%d)", last_pid, segment_start, time);

    printf(" |\n");
    printf("\nPID\tAT\tBT\tPriority\tCT\tWT\tTAT\tRT\n");

    for (int i = 0; i < n; i++) {
        Process *p = &processes[i];

        total_wt += p->waiting_time;
        total_tat += p->turnaround_time;

        printf("P%d\t%d\t%d\t%d\t\t%d\t%d\t%d\t%d\n",
               p->pid, p->arrival_time, p->burst_time,
               p->priority, p->completion_time,
               p->waiting_time, p->turnaround_time,
               p->response_time);
    }

    printf("\nAverage Waiting Time: %.2f\n", total_wt / n);
    printf("Average Turnaround Time: %.2f\n", total_tat / n);
}

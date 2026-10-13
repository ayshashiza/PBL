#include <stdio.h>
#include "scheduling.h"

void fcfs(Process processes[], int n)
{
    int order[MAX_PROCESSES];
    int time = 0;
    double total_wt = 0;
    double total_tat = 0;

    /* Arrange processes by arrival time */
    for (int i = 0; i < n; i++) {
        order[i] = i;
    }

    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (processes[order[j]].arrival_time <
                processes[order[i]].arrival_time) {
                int temp = order[i];
                order[i] = order[j];
                order[j] = temp;
            }
        }
    }

    printf("\n--- FCFS Scheduling ---\n");
    printf("Gantt Chart:\n");

    for (int i = 0; i < n; i++) {
        Process *p = &processes[order[i]];

        if (time < p->arrival_time) {
            printf("| IDLE (%d-%d) ", time, p->arrival_time);
            time = p->arrival_time;
        }

        int start = time;
        int response = start - p->arrival_time;

        time += p->burst_time;
        p->completion_time = time;
        p->turnaround_time = time - p->arrival_time;
        p->waiting_time = p->turnaround_time - p->burst_time;
        p->response_time = response;

        printf("| P%d (%d-%d) ", p->pid, start, time);

        total_wt += p->waiting_time;
        total_tat += p->turnaround_time;
    }

    printf("|\n\n");
    printf("PID\tAT\tBT\tCT\tWT\tTAT\tRT\n");

    for (int i = 0; i < n; i++) {
        Process *p = &processes[i];

        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p->pid, p->arrival_time, p->burst_time,
               p->completion_time, p->waiting_time,
               p->turnaround_time, p->response_time);
    }

    printf("\nAverage Waiting Time: %.2f\n", total_wt / n);
    printf("Average Turnaround Time: %.2f\n", total_tat / n);
}

#include <stdio.h>
#include "scheduling.h"

void round_robin(Process processes[], int n, int quantum)
{
    int queue[MAX_PROCESSES * MAX_PROCESSES];
    int front = 0, rear = 0;
    int time = 0;
    int completed = 0;
    int in_queue[MAX_PROCESSES] = {0};
    double total_wt = 0;
    double total_tat = 0;

    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].response_time = -1;
    }

    printf("\n--- Round Robin Scheduling ---\n");
    printf("Gantt Chart:\n");

    while (completed < n) {
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= time &&
                processes[i].remaining_time > 0 &&
                !in_queue[i]) {
                queue[rear++] = i;
                in_queue[i] = 1;
            }
        }

        if (front == rear) {
            int next = -1;

            for (int i = 0; i < n; i++) {
                if (processes[i].remaining_time > 0 &&
                    (next == -1 ||
                     processes[i].arrival_time <
                     processes[next].arrival_time)) {
                    next = i;
                }
            }

            if (next == -1)
                break;

            if (time < processes[next].arrival_time) {
                printf("| IDLE (%d-%d) ",
                       time, processes[next].arrival_time);
                time = processes[next].arrival_time;
            }

            for (int i = 0; i < n; i++) {
                if (processes[i].arrival_time <= time &&
                    processes[i].remaining_time > 0 &&
                    !in_queue[i]) {
                    queue[rear++] = i;
                    in_queue[i] = 1;
                }
            }
        }

        if (front == rear)
            continue;

        int index = queue[front++];
        in_queue[index] = 0;

        Process *p = &processes[index];

        if (p->response_time == -1)
            p->response_time = time - p->arrival_time;

        int start = time;
        int run_time = p->remaining_time < quantum
                       ? p->remaining_time : quantum;

        time += run_time;
        p->remaining_time -= run_time;

        printf("| P%d (%d-%d) ", p->pid, start, time);

        /* Add newly arrived processes to the queue. */
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= time &&
                processes[i].remaining_time > 0 &&
                !in_queue[i] && i != index) {
                queue[rear++] = i;
                in_queue[i] = 1;
            }
        }

        if (p->remaining_time > 0) {
            queue[rear++] = index;
            in_queue[index] = 1;
        } else {
            p->completion_time = time;
            p->turnaround_time = time - p->arrival_time;
            p->waiting_time = p->turnaround_time - p->burst_time;
            completed++;
        }
    }

    printf("|\n");
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

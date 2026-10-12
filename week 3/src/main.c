#include <stdio.h>
#include <stdlib.h>
#include "scheduling.h"

int main(void)
{
    int choice;
    int n;
    int quantum;
    Process processes[MAX_PROCESSES];

    do {
        printf("\n====================================\n");
        printf("     CPU SCHEDULING SIMULATOR\n");
        printf("====================================\n");
        printf("1. FCFS Scheduling\n");
        printf("2. SJF Non-Preemptive\n");
        printf("3. SJF Preemptive\n");
        printf("4. Priority Non-Preemptive\n");
        printf("5. Priority Preemptive\n");
        printf("6. Round Robin\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            return 1;
        }

        if (choice == 7) {
            printf("Exiting simulator. Goodbye!\n");
            break;
        }

        if (choice < 1 || choice > 6) {
            printf("Invalid choice. Try again.\n");
            continue;
        }

        printf("Enter number of processes (1-%d): ",
               MAX_PROCESSES);

        if (scanf("%d", &n) != 1 ||
            n < 1 || n > MAX_PROCESSES) {
            printf("Invalid number of processes.\n");
            return 1;
        }

        for (int i = 0; i < n; i++) {
            printf("\nProcess P%d\n", i + 1);
            processes[i].pid = i + 1;

            printf("Arrival time: ");
            if (scanf("%d", &processes[i].arrival_time) != 1 ||
                processes[i].arrival_time < 0) {
                printf("Invalid arrival time.\n");
                return 1;
            }

            printf("Burst time: ");
            if (scanf("%d", &processes[i].burst_time) != 1 ||
                processes[i].burst_time <= 0) {
                printf("Burst time must be positive.\n");
                return 1;
            }

            printf("Priority (smaller number = higher priority): ");
            if (scanf("%d", &processes[i].priority) != 1) {
                printf("Invalid priority.\n");
                return 1;
            }
        }

        if (choice == 6) {
            printf("Enter time quantum: ");
            if (scanf("%d", &quantum) != 1 || quantum <= 0) {
                printf("Time quantum must be positive.\n");
                return 1;
            }
        }

        switch (choice) {
            case 1:
                fcfs(processes, n);
                break;
            case 2:
                sjf_nonpreemptive(processes, n);
                break;
            case 3:
                sjf_preemptive(processes, n);
                break;
            case 4:
                priority_nonpreemptive(processes, n);
                break;
            case 5:
                priority_preemptive(processes, n);
                break;
            case 6:
                round_robin(processes, n, quantum);
                break;
        }

    } while (1);

    return 0;
}

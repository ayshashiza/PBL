
#include <stdio.h>
#include <stdlib.h>
#include "page_replacement.h"

int main(void)
{
    int pages[MAX_PAGES];
    int n, frames, choice;

    printf("====================================\n");
    printf(" PAGE REPLACEMENT SIMULATOR\n");
    printf("====================================\n");

    printf("Enter number of page references (1-%d): ", MAX_PAGES);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_PAGES) {
        printf("Invalid number of page references.\n");
        return 1;
    }

    printf("Enter the page reference string (space-separated):\n");
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &pages[i]) != 1 || pages[i] < 0) {
            printf("Invalid page number.\n");
            return 1;
        }
    }

    printf("Enter number of frames (1-%d): ", MAX_FRAMES);
    if (scanf("%d", &frames) != 1 ||
        frames < 1 || frames > MAX_FRAMES) {
        printf("Invalid number of frames.\n");
        return 1;
    }

    printf("\nChoose algorithm:\n");
    printf("1. FIFO\n");
    printf("2. LRU\n");
    printf("3. Optimal\n");
    printf("4. Compare all\n");
    printf("Enter choice: ");

    if (scanf("%d", &choice) != 1 || choice < 1 || choice > 4) {
        printf("Invalid choice.\n");
        return 1;
    }

    Result results[3];

    if (choice == 1 || choice == 4)
        fifo(pages, n, frames, &results[0]);

    if (choice == 2 || choice == 4)
        lru(pages, n, frames, &results[1]);

    if (choice == 3 || choice == 4)
        optimal(pages, n, frames, &results[2]);

    if (choice == 4) {
        printf("\n========== COMPARISON ==========\n");
        printf("%-12s %-8s %-8s %-12s %-12s\n",
               "Algorithm", "Hits", "Faults", "Hit Ratio", "Fault Ratio");

        const char *names[] = {"FIFO", "LRU", "Optimal"};

        for (int i = 0; i < 3; i++) {
            double hit_ratio =
                (double)results[i].hits / n * 100.0;
            double fault_ratio =
                (double)results[i].faults / n * 100.0;

            printf("%-12s %-8d %-8d %-11.2f%% %-11.2f%%\n",
                   names[i], results[i].hits, results[i].faults,
                   hit_ratio, fault_ratio);
        }
    }

    return 0;
}

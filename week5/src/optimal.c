
#include <stdio.h>
#include "page_replacement.h"

void optimal(int pages[], int n, int frames, Result *result)
{
    int memory[MAX_FRAMES];

    result->total_pages = n;
    result->frame_count = frames;
    result->hits = 0;
    result->faults = 0;

    for (int i = 0; i < frames; i++)
        memory[i] = -1;

    printf("\nOptimal Page Replacement\n");

    for (int i = 0; i < n; i++) {
        int found = 0;

        for (int j = 0; j < frames; j++) {
            if (memory[j] == pages[i]) {
                found = 1;
                break;
            }
        }

        if (found) {
            result->hits++;
            printf("Page %d: HIT   | Frames: ", pages[i]);
        } else {
            result->faults++;

            int replace = -1;
            int farthest = -1;

            for (int j = 0; j < frames; j++) {
                if (memory[j] == -1) {
                    replace = j;
                    break;
                }

                int k;
                for (k = i + 1; k < n; k++) {
                    if (pages[k] == memory[j])
                        break;
                }

                if (k == n) {
                    replace = j;
                    break;
                }

                if (k > farthest) {
                    farthest = k;
                    replace = j;
                }
            }

            memory[replace] = pages[i];

            printf("Page %d: FAULT | Frames: ", pages[i]);
        }

        for (int j = 0; j < frames; j++) {
            if (memory[j] == -1)
                printf("- ");
            else
                printf("%d ", memory[j]);
        }

        printf("\n");
    }

    printf("Total hits: %d\n", result->hits);
    printf("Total faults: %d\n", result->faults);
}


#include <stdio.h>
#include "page_replacement.h"

void lru(int pages[], int n, int frames, Result *result)
{
    int memory[MAX_FRAMES];
    int last_used[MAX_FRAMES];

    result->total_pages = n;
    result->frame_count = frames;
    result->hits = 0;
    result->faults = 0;

    for (int i = 0; i < frames; i++) {
        memory[i] = -1;
        last_used[i] = -1;
    }

    printf("\nLRU Page Replacement\n");

    for (int i = 0; i < n; i++) {
        int found = -1;

        for (int j = 0; j < frames; j++) {
            if (memory[j] == pages[i]) {
                found = j;
                break;
            }
        }

        if (found != -1) {
            result->hits++;
            last_used[found] = i;
            printf("Page %d: HIT   | Frames: ", pages[i]);
        } else {
            result->faults++;

            int replace = -1;

            for (int j = 0; j < frames; j++) {
                if (memory[j] == -1) {
                    replace = j;
                    break;
                }
            }

            if (replace == -1) {
                replace = 0;

                for (int j = 1; j < frames; j++) {
                    if (last_used[j] < last_used[replace])
                        replace = j;
                }
            }

            memory[replace] = pages[i];
            last_used[replace] = i;

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

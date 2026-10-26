
#include <stdio.h>
#include "page_replacement.h"

void fifo(int pages[], int n, int frames, Result *result)
{
    int memory[MAX_FRAMES];
    int next = 0;

    result->total_pages = n;
    result->frame_count = frames;
    result->hits = 0;
    result->faults = 0;

    for (int i = 0; i < frames; i++)
        memory[i] = -1;

    printf("\nFIFO Page Replacement\n");

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
            memory[next] = pages[i];
            next = (next + 1) % frames;
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


#ifndef PAGE_REPLACEMENT_H
#define PAGE_REPLACEMENT_H

#define MAX_PAGES 100
#define MAX_FRAMES 20

typedef struct {
    int total_pages;
    int frame_count;
    int hits;
    int faults;
} Result;

void fifo(int pages[], int n, int frames, Result *result);
void lru(int pages[], int n, int frames, Result *result);
void optimal(int pages[], int n, int frames, Result *result);

#endif

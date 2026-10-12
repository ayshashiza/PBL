
#include <stdio.h>
#include "scheduling.h"

int main(void)
{
    Process p[3] = {
        {.pid = 1, .arrival_time = 0, .burst_time = 5, .priority = 2},
        {.pid = 2, .arrival_time = 1, .burst_time = 3, .priority = 1},
        {.pid = 3, .arrival_time = 2, .burst_time = 2, .priority = 3}
    };

    fcfs(p, 3);
    return 0;
}

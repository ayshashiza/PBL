#ifndef IPC_H
#define IPC_H

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>

#define FIFO_UI_TO_CORE "/tmp/week2_ui_to_core"
#define FIFO_CORE_TO_UI "/tmp/week2_core_to_ui"

#define SHM_NAME "/week2_shared_memory"
#define MQ_NAME "/week2_log_queue"

#define MAX_ITEMS 100
#define MAX_MESSAGE 256
#define MAX_RESPONSE 2048

typedef struct {
    int accumulator;
    int program_counter;
    int memory[MAX_ITEMS];
    int program_size;
    int stack[MAX_ITEMS];
    int stack_top;
    int queue[MAX_ITEMS];
    int queue_front;
    int queue_rear;
    int queue_count;
} SharedData;

#endif

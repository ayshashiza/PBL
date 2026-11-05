#define _POSIX_C_SOURCE 200809L

#include "../ipc.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <mqueue.h>
#include <errno.h>

static void log_message(mqd_t mq, const char *message) {
    if (mq != (mqd_t)-1) {
        if (mq_send(mq, message, strlen(message) + 1, 0) == -1)
            perror("Core: mq_send");
    }
}

static void respond(int fd, const char *message) {
    if (write(fd, message, strlen(message) + 1) == -1)
        perror("Core: response write");
}

static void execute_step(SharedData *data) {
    if (data->program_counter >= data->program_size)
        return;

    data->accumulator += data->memory[data->program_counter];
    data->program_counter++;
}

int main(void) {

    mkfifo(FIFO_UI_TO_CORE, 0666);
    mkfifo(FIFO_CORE_TO_UI, 0666);

    int shm_fd = shm_open(
        SHM_NAME,
        O_CREAT | O_RDWR,
        0666
    );

    if (shm_fd == -1) {
        perror("Core: shm_open");
        return 1;
    }

    if (ftruncate(shm_fd, sizeof(SharedData)) == -1) {
        perror("Core: ftruncate");
        return 1;
    }

    SharedData *data = mmap(
        NULL,
        sizeof(SharedData),
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        shm_fd,
        0
    );

    if (data == MAP_FAILED) {
        perror("Core: mmap");
        return 1;
    }

    memset(data, 0, sizeof(SharedData));

    data->stack_top = -1;

    mqd_t mq = mq_open(MQ_NAME, O_WRONLY);

    if (mq == (mqd_t)-1) {
        perror("Core: Start Logger first");
        return 1;
    }

    printf("Core started. Waiting for UI commands...\n");

    log_message(mq, "Core process started");

    int input = open(FIFO_UI_TO_CORE, O_RDONLY);

    if (input == -1) {
        perror("Core: input FIFO");
        return 1;
    }

    int output = open(FIFO_CORE_TO_UI, O_WRONLY);

    if (output == -1) {
        perror("Core: output FIFO");
        return 1;
    }

    char command[MAX_MESSAGE];
    char response[MAX_RESPONSE];

    while (1) {

        ssize_t n = read(input, command, sizeof(command) - 1);

        if (n <= 0)
            break;

        command[n] = '\0';

        command[strcspn(command, "\n")] = '\0';

        response[0] = '\0';

        if (strcmp(command, "load") == 0 ||
            strncmp(command, "load ", 5) == 0) {

            char *token = strtok(command + 4, " ");
            int count = 0;

            data->program_counter = 0;
            data->accumulator = 0;

            while (token && count < MAX_ITEMS) {
                char *end;
                long value = strtol(token, &end, 10);

                if (*end != '\0')
                    break;

                data->memory[count++] = (int)value;
                token = strtok(NULL, " ");
            }

            data->program_size = count;

            snprintf(
                response,
                sizeof(response),
                "Program loaded successfully. Instructions: %d",
                count
            );

            log_message(mq, "Program loaded");

        }

        else if (strcmp(command, "run") == 0) {

            while (data->program_counter < data->program_size)
                execute_step(data);

            snprintf(
                response,
                sizeof(response),
                "Program executed.\nACC = %d\nPC = %d",
                data->accumulator,
                data->program_counter
            );

            log_message(mq, "Program executed");

        }

        else if (strcmp(command, "step") == 0) {

            if (data->program_counter < data->program_size) {
                execute_step(data);

                snprintf(
                    response,
                    sizeof(response),
                    "One instruction executed.\nACC = %d\nPC = %d",
                    data->accumulator,
                    data->program_counter
                );
            } else {
                snprintf(response, sizeof(response),
                         "No instructions remaining.");
            }

            log_message(mq, "Single step executed");

        }

        else if (strcmp(command, "reg") == 0) {

            snprintf(
                response,
                sizeof(response),
                "CPU Registers:\nACC = %d\nPC = %d",
                data->accumulator,
                data->program_counter
            );

        }

        else if (strcmp(command, "mem") == 0) {

            int pos = 0;

            pos += snprintf(
                response + pos,
                sizeof(response) - pos,
                "Memory Contents:\n"
            );

            for (int i = 0; i < data->program_size; i++) {
                pos += snprintf(
                    response + pos,
                    sizeof(response) - pos,
                    "Memory[%d] = %d\n",
                    i,
                    data->memory[i]
                );
            }

        }

        else if (strcmp(command, "stack") == 0) {

            char action[20];
            int value;

            if (sscanf(command, "stack %19s %d", action, &value) == 2 &&
                strcmp(action, "push") == 0) {

                if (data->stack_top < MAX_ITEMS - 1) {
                    data->stack[++data->stack_top] = value;
                    snprintf(response, sizeof(response),
                             "Pushed %d onto stack.", value);
                } else {
                    snprintf(response, sizeof(response),
                             "Stack overflow.");
                }

            } else if (strcmp(command, "stack %19s", action) == 1 &&
                       strcmp(action, "pop") == 0) {

                if (data->stack_top >= 0) {
                    value = data->stack[data->stack_top--];
                    snprintf(response, sizeof(response),
                             "Popped value: %d", value);
                } else {
                    snprintf(response, sizeof(response),
                             "Stack underflow.");
                }

            } else {
                snprintf(response, sizeof(response),
                         "Use: stack push <number> OR stack pop");
            }

        }

        else if (strcmp(command, "queue") == 0) {

            char action[20];
            int value;

            if (sscanf(command, "queue %19s %d", action, &value) == 2 &&
                strcmp(action, "enqueue") == 0) {

                if (data->queue_count < MAX_ITEMS) {
                    data->queue[data->queue_rear] = value;

                    data->queue_rear =
                        (data->queue_rear + 1) % MAX_ITEMS;

                    data->queue_count++;

                    snprintf(response, sizeof(response),
                             "Enqueued %d.", value);
                } else {
                    snprintf(response, sizeof(response),
                             "Queue is full.");
                }

            } else if (strcmp(command, "queue dequeue") == 0) {

                if (data->queue_count > 0) {
                    value = data->queue[data->queue_front];

                    data->queue_front =
                        (data->queue_front + 1) % MAX_ITEMS;

                    data->queue_count--;

                    snprintf(response, sizeof(response),
                             "Dequeued value: %d", value);
                } else {
                    snprintf(response, sizeof(response),
                             "Queue is empty.");
                }

            } else {
                snprintf(response, sizeof(response),
                         "Use: queue enqueue <number> OR queue dequeue");
            }

        }

        else if (strcmp(command, "exit") == 0) {

            snprintf(response, sizeof(response),
                     "Core process shutting down.");

            respond(output, response);

            log_message(mq, "Core process stopped");
            log_message(mq, "SHUTDOWN");

            break;

        }

        else {
            snprintf(response, sizeof(response),
                     "Unknown command. Type help.");
        }

        respond(output, response);
    }

    close(input);
    close(output);

    mq_close(mq);

    munmap(data, sizeof(SharedData));
    close(shm_fd);
    shm_unlink(SHM_NAME);

    unlink(FIFO_UI_TO_CORE);
    unlink(FIFO_CORE_TO_UI);

    printf("Core stopped.\n");

    return 0;
}

#define _POSIX_C_SOURCE 200809L

#include "../ipc.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

int main(void) {

    printf("\n====================================\n");
    printf("     MULTI-PROCESS SIMULATOR\n");
    printf("====================================\n");

    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);

    if (shm_fd == -1) {
        perror("UI: Start Core first");
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
        perror("UI: mmap");
        return 1;
    }

    int output = open(FIFO_UI_TO_CORE, O_WRONLY);

    if (output == -1) {
        perror("UI: output FIFO");
        return 1;
    }

    int input = open(FIFO_CORE_TO_UI, O_RDONLY);

    if (input == -1) {
        perror("UI: input FIFO");
        return 1;
    }

    char command[MAX_MESSAGE];
    char response[MAX_RESPONSE];

    printf("\nType 'help' to see available commands.\n");

    while (1) {

        printf("\nsim> ");
        fflush(stdout);

        if (!fgets(command, sizeof(command), stdin))
            break;

        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "help") == 0) {

            printf("\nAvailable Commands:\n");
            printf("load 10 20 30 40  - Load program\n");
            printf("run               - Execute program\n");
            printf("step              - Execute one instruction\n");
            printf("reg               - Display registers\n");
            printf("mem               - Display memory\n");
            printf("stack push 10     - Push onto stack\n");
            printf("stack pop         - Pop from stack\n");
            printf("queue enqueue 10  - Add to queue\n");
            printf("queue dequeue     - Remove from queue\n");
            printf("exit              - Exit simulator\n");

            continue;
        }

        if (write(output, command, strlen(command) + 1) == -1) {
            perror("UI: write");
            break;
        }

        ssize_t n = read(input, response, sizeof(response) - 1);

        if (n <= 0) {
            printf("Core process disconnected.\n");
            break;
        }

        response[n] = '\0';

        printf("\n%s\n", response);

        if (strcmp(command, "exit") == 0)
            break;

        if (strcmp(command, "reg") == 0) {
            printf("Shared Memory: ACC=%d, PC=%d\n",
                   data->accumulator,
                   data->program_counter);
        }
    }

    close(output);
    close(input);

    munmap(data, sizeof(SharedData));
    close(shm_fd);

    printf("\nUI stopped.\n");

    return 0;
}

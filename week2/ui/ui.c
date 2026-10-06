#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <mqueue.h>
#include <sys/stat.h>
#include "ipc.h"

int main(void)
{
    mqd_t q_ui_core;
    mqd_t q_core_ui;

    char command[256];

    printf("========================================\n");
    printf("     MULTI-PROCESS SIMULATOR - UI\n");
    printf("========================================\n");

    /*
     * Open UI -> Core queue
     */
    q_ui_core = mq_open(Q_UI_TO_CORE, O_WRONLY);

    if (q_ui_core == (mqd_t)-1)
    {
        perror("UI: Unable to open UI -> Core queue");
        printf("Make sure the Core process is running first.\n");
        return 1;
    }

    /*
     * Open Core -> UI queue
     */
    q_core_ui = mq_open(Q_CORE_TO_UI, O_RDONLY);

    if (q_core_ui == (mqd_t)-1)
    {
        perror("UI: Unable to open Core -> UI queue");
        mq_close(q_ui_core);
        return 1;
    }

    printf("\nSimulator UI started.\n");
    printf("Type 'help' for commands.\n\n");

    while (1)
    {
        printf("sim> ");
        fflush(stdout);

        if (fgets(command, sizeof(command), stdin) == NULL)
        {
            break;
        }

        command[strcspn(command, "\n")] = '\0';

        if (strlen(command) == 0)
        {
            continue;
        }

        /*
         * Exit command
         */
        if (strcmp(command, "exit") == 0 ||
            strcmp(command, "quit") == 0)
        {
            mq_send(q_ui_core,
                    command,
                    strlen(command) + 1,
                    0);

            printf("UI: Exiting simulator.\n");
            break;
        }

        /*
         * Help command
         */
        if (strcmp(command, "help") == 0)
        {
            printf("\nAvailable commands:\n");
            printf("  help       - Show available commands\n");
            printf("  load       - Load a program\n");
            printf("  run        - Execute the program\n");
            printf("  step       - Execute one instruction\n");
            printf("  reg        - Display registers\n");
            printf("  mem        - Display memory\n");
            printf("  stack      - Display stack\n");
            printf("  queue      - Display queue\n");
            printf("  exit       - Exit simulator\n\n");

            continue;
        }

        /*
         * Send command from UI to Core
         */
        if (mq_send(q_ui_core,
                    command,
                    strlen(command) + 1,
                    0) == -1)
        {
            perror("UI: mq_send failed");
            continue;
        }

        /*
         * Wait for response from Core
         */
        char response[1024];

        ssize_t bytes_read = mq_receive(
            q_core_ui,
            response,
            sizeof(response),
            NULL
        );

        if (bytes_read == -1)
        {
            perror("UI: mq_receive failed");
            continue;
        }

        response[bytes_read] = '\0';

        printf("%s\n", response);
    }

    mq_close(q_ui_core);
    mq_close(q_core_ui);

    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <mqueue.h>
#include <sys/stat.h>
#include "ipc.h"

/* Simulator state */
int ACC = 0;
int PC = 0;

int memory[100];
int stack[100];
int stack_top = -1;

int queue[100];
int queue_front = 0;
int queue_rear = -1;

/* Send message to Logger */
void send_log(mqd_t logger_queue, const char *message)
{
    LogMessage log;

    memset(&log, 0, sizeof(log));
    strncpy(log.message, message, sizeof(log.message) - 1);

    if (mq_send(logger_queue,
                (char *)&log,
                sizeof(log),
                0) == -1)
    {
        perror("Core: Unable to send log");
    }
}

/* Load default program */
void load_program(void)
{
    int program[] = {10, 20, 30, 40};
    int size = sizeof(program) / sizeof(program[0]);

    for (int i = 0; i < size; i++)
    {
        memory[i] = program[i];
    }

    PC = 0;
    ACC = 0;

    printf("Program loaded: %d instructions\n", size);
}

/* Execute one instruction */
void step_program(void)
{
    if (PC >= 4)
    {
        printf("Program completed.\n");
        return;
    }

    ACC += memory[PC];
    PC++;

    printf("Step executed. ACC=%d PC=%d\n", ACC, PC);
}

/* Execute complete program */
void run_program(void)
{
    while (PC < 4)
    {
        ACC += memory[PC];
        PC++;
    }

    printf("Program executed. ACC=%d PC=%d\n", ACC, PC);
}

/* Display registers */
void show_registers(void)
{
    printf("Registers: ACC=%d PC=%d\n", ACC, PC);
}

/* Display memory */
void show_memory(void)
{
    printf("Memory:\n");

    for (int i = 0; i < 10; i++)
    {
        printf("Memory[%d] = %d\n", i, memory[i]);
    }
}

/* Display stack */
void show_stack(void)
{
    if (stack_top == -1)
    {
        printf("Stack: EMPTY\n");
        return;
    }

    printf("Stack:\n");

    for (int i = stack_top; i >= 0; i--)
    {
        printf("%d\n", stack[i]);
    }
}

/* Display queue */
void show_queue(void)
{
    if (queue_rear < queue_front)
    {
        printf("Queue: EMPTY\n");
        return;
    }

    printf("Queue:\n");

    for (int i = queue_front; i <= queue_rear; i++)
    {
        printf("%d\n", queue[i]);
    }
}

int main(void)
{
    mqd_t q_ui_core;
    mqd_t q_core_ui;
    mqd_t q_core_log;

    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 256;
    attr.mq_curmsgs = 0;

    /*
     * Create UI -> Core queue
     */
    q_ui_core = mq_open(
        Q_UI_TO_CORE,
        O_CREAT | O_RDONLY,
        0666,
        &attr
    );

    if (q_ui_core == (mqd_t)-1)
    {
        perror("Core: Unable to open UI -> Core queue");
        return 1;
    }

    /*
     * Create Core -> UI queue
     */
    q_core_ui = mq_open(
        Q_CORE_TO_UI,
        O_CREAT | O_WRONLY,
        0666,
        &attr
    );

    if (q_core_ui == (mqd_t)-1)
    {
        perror("Core: Unable to open Core -> UI queue");
        mq_close(q_ui_core);
        return 1;
    }

    /*
     * Open Core -> Logger queue.
     *
     * Logger should be started before Core.
     */
    q_core_log = mq_open(
        Q_CORE_TO_LOG,
        O_WRONLY
    );

    if (q_core_log == (mqd_t)-1)
    {
        perror("Core: Unable to open Core -> Logger queue");
        printf("Start Logger first.\n");

        mq_close(q_ui_core);
        mq_close(q_core_ui);

        return 1;
    }

    printf("Core process started. Waiting for commands...\n");

    while (1)
    {
        char command[256];
        char response[256];

        memset(command, 0, sizeof(command));
        memset(response, 0, sizeof(response));

        /*
         * Receive command from UI
         */
        ssize_t received = mq_receive(
            q_ui_core,
            command,
            sizeof(command),
            NULL
        );

        if (received == -1)
        {
            perror("Core: mq_receive failed");
            continue;
        }

        command[received] = '\0';

        printf("Core received: %s\n", command);

        /*
         * HELP
         */
        if (strcmp(command, "help") == 0)
        {
            strcpy(
                response,
                "Commands: help, load, run, step, reg, mem, stack, queue, exit"
            );

            send_log(q_core_log, "HELP command received");
        }

        /*
         * LOAD
         */
        else if (strcmp(command, "load") == 0)
        {
            load_program();

            strcpy(response, "Program loaded successfully.");

            send_log(q_core_log, "Program loaded");
        }

        /*
         * RUN
         */
        else if (strcmp(command, "run") == 0)
        {
            run_program();

            snprintf(
                response,
                sizeof(response),
                "Program executed. ACC=%d PC=%d",
                ACC,
                PC
            );

            send_log(q_core_log, "Program executed");
        }

        /*
         * STEP
         */
        else if (strcmp(command, "step") == 0)
        {
            step_program();

            snprintf(
                response,
                sizeof(response),
                "Step executed. ACC=%d PC=%d",
                ACC,
                PC
            );

            send_log(q_core_log, "Single step executed");
        }

        /*
         * REGISTERS
         */
        else if (strcmp(command, "reg") == 0)
        {
            snprintf(
                response,
                sizeof(response),
                "Registers: ACC=%d PC=%d",
                ACC,
                PC
            );

            send_log(q_core_log, "Registers requested");
        }

        /*
         * MEMORY
         */
        else if (strcmp(command, "mem") == 0)
        {
            printf("Memory:\n");

            for (int i = 0; i < 10; i++)
            {
                printf("Memory[%d] = %d\n", i, memory[i]);
            }

            strcpy(response, "Memory displayed in Core.");

            send_log(q_core_log, "Memory requested");
        }

        /*
         * STACK
         */
        else if (strcmp(command, "stack") == 0)
        {
            show_stack();

            strcpy(response, "Stack displayed in Core.");

            send_log(q_core_log, "Stack requested");
        }

        /*
         * QUEUE
         */
        else if (strcmp(command, "queue") == 0)
        {
            show_queue();

            strcpy(response, "Queue displayed in Core.");

            send_log(q_core_log, "Queue requested");
        }

        /*
         * EXIT
         */
        else if (strcmp(command, "exit") == 0 ||
                 strcmp(command, "quit") == 0)
        {
            strcpy(response, "Simulator shutting down.");

            send_log(q_core_log, "EXIT");

            mq_send(
                q_core_ui,
                response,
                strlen(response) + 1,
                0
            );

            break;
        }

        /*
         * UNKNOWN COMMAND
         */
        else
        {
            strcpy(response, "Unknown command. Type 'help'.");

            send_log(q_core_log, "Unknown command received");
        }

        /*
         * Send response back to UI
         */
        if (mq_send(
                q_core_ui,
                response,
                strlen(response) + 1,
                0) == -1)
        {
            perror("Core: Unable to send response to UI");
        }
    }

    /*
     * Cleanup
     */
    mq_close(q_ui_core);
    mq_close(q_core_ui);
    mq_close(q_core_log);

    mq_unlink(Q_UI_TO_CORE);
    mq_unlink(Q_CORE_TO_UI);

    printf("Core process stopped.\n");

    return 0;
}
  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <time.h>
#include "ipc.h"

int main(void)
{
    mqd_t q_core_log;
    struct mq_attr attr;

    /*
     * Message queue configuration
     */
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(LogMessage);
    attr.mq_curmsgs = 0;

    /*
     * Create Core -> Logger queue
     */
    q_core_log = mq_open(
        Q_CORE_TO_LOG,
        O_CREAT | O_RDONLY,
        0666,
        &attr
    );

    if (q_core_log == (mqd_t)-1)
    {
        perror("Logger: mq_open failed");
        return 1;
    }

    printf("Logger process started.\n");
    printf("Waiting for log messages...\n");

    /*
     * Open log file
     */
    FILE *log_file = fopen("simulator.log", "a");

    if (log_file == NULL)
    {
        perror("Logger: Unable to open log file");
        mq_close(q_core_log);
        return 1;
    }

    while (1)
    {
        LogMessage message;

        /*
         * Receive message from Core
         */
        ssize_t received = mq_receive(
            q_core_log,
            (char *)&message,
            sizeof(message),
            NULL
        );

        if (received == -1)
        {
            perror("Logger: mq_receive failed");
            continue;
        }

        /*
         * Display log message
         */
        printf("[LOGGER] %s\n", message.message);

        /*
         * Add timestamp to log file
         */
        time_t now = time(NULL);
        struct tm *time_info = localtime(&now);

        char timestamp[64];

        strftime(
            timestamp,
            sizeof(timestamp),
            "%Y-%m-%d %H:%M:%S",
            time_info
        );

        fprintf(
            log_file,
            "[%s] %s\n",
            timestamp,
            message.message
        );

        fflush(log_file);

        /*
         * Stop logger when Core sends EXIT
         */
        if (strcmp(message.message, "EXIT") == 0)
        {
            break;
        }
    }

    fclose(log_file);

    mq_close(q_core_log);
    mq_unlink(Q_CORE_TO_LOG);

    printf("Logger process stopped.\n");

    return 0;
}

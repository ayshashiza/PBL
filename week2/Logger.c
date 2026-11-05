#define _POSIX_C_SOURCE 200809L

#include "../ipc.h"

#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(void) {
    struct mq_attr attr = {0};

    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_MESSAGE;

    mq_unlink(MQ_NAME);

    mqd_t mq = mq_open(
        MQ_NAME,
        O_CREAT | O_RDONLY,
        0666,
        &attr
    );

    if (mq == (mqd_t)-1) {
        perror("Logger: mq_open");
        return 1;
    }

    FILE *file = fopen("simulator.log", "a");

    if (!file) {
        perror("Logger: fopen");
        mq_close(mq);
        mq_unlink(MQ_NAME);
        return 1;
    }

    printf("Logger started. Waiting for messages...\n");

    char message[MAX_MESSAGE];

    while (1) {
        ssize_t bytes = mq_receive(
            mq,
            message,
            sizeof(message),
            NULL
        );

        if (bytes < 0) {
            perror("Logger: mq_receive");
            break;
        }

        message[bytes] = '\0';

        if (strcmp(message, "SHUTDOWN") == 0)
            break;

        fprintf(file, "%s\n", message);
        fflush(file);

        printf("LOG: %s\n", message);
    }

    fclose(file);
    mq_close(mq);
    mq_unlink(MQ_NAME);

    printf("Logger stopped.\n");

    return 0;
}

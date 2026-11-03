#define _POSIX_C_SOURCE 200809L

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static volatile sig_atomic_t signal_received = 0;

static void handle_signal(int signal_number)
{
    (void)signal_number;
    signal_received = 1;
}

int main(void)
{
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = handle_signal;
    sigemptyset(&action.sa_mask);

    if (sigaction(SIGUSR1, &action, NULL) == -1) {
        perror("sigaction");
        return EXIT_FAILURE;
    }

    pid_t child = fork();
    if (child == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (child == 0) {
        printf("Child process (PID %ld): waiting for SIGUSR1...\n",
               (long)getpid());
        fflush(stdout);

        while (!signal_received) {
            pause();
        }

        printf("Child process: received SIGUSR1.\n");
        _exit(EXIT_SUCCESS);
    }

    printf("Parent process (PID %ld): sending SIGUSR1 to child (PID %ld).\n",
           (long)getpid(), (long)child);

    /* Give the child time to enter its wait loop. */
    sleep(1);

    if (kill(child, SIGUSR1) == -1) {
        perror("kill");
        waitpid(child, NULL, 0);
        return EXIT_FAILURE;
    }

    int status;
    if (waitpid(child, &status, 0) == -1) {
        perror("waitpid");
        return EXIT_FAILURE;
    }

    if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS) {
        printf("Parent process: child finished successfully.\n");
        return EXIT_SUCCESS;
    }

    fprintf(stderr, "Parent process: child did not finish successfully.\n");
    return EXIT_FAILURE;
}

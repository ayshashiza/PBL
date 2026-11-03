#define _POSIX_C_SOURCE 200809L

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/ipc_unix_socket_demo.sock"
#define BUFFER_SIZE 256

int main(void)
{
    int server_fd = -1;
    int client_fd = -1;
    int result = EXIT_FAILURE;
    pid_t child = -1;
    struct sockaddr_un address;

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        goto cleanup;
    }

    memset(&address, 0, sizeof(address));
    address.sun_family = AF_UNIX;
    snprintf(address.sun_path, sizeof(address.sun_path), "%s", SOCKET_PATH);

    /* Remove a stale socket file from an earlier run. */
    unlink(SOCKET_PATH);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
        perror("bind");
        goto cleanup;
    }

    if (listen(server_fd, 1) == -1) {
        perror("listen");
        goto cleanup;
    }

    child = fork();
    if (child == -1) {
        perror("fork");
        goto cleanup;
    }

    if (child == 0) {
        close(server_fd);

        int socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (socket_fd == -1) {
            perror("client socket");
            _exit(EXIT_FAILURE);
        }

        if (connect(socket_fd, (struct sockaddr *)&address,
                    sizeof(address)) == -1) {
            perror("connect");
            close(socket_fd);
            _exit(EXIT_FAILURE);
        }

        const char message[] = "Hello from the client process!";
        if (send(socket_fd, message, sizeof(message), 0) == -1) {
            perror("send");
            close(socket_fd);
            _exit(EXIT_FAILURE);
        }

        printf("Client: sent a message to the server.\n");
        close(socket_fd);
        _exit(EXIT_SUCCESS);
    }

    printf("Server: waiting for a client connection...\n");

    client_fd = accept(server_fd, NULL, NULL);
    if (client_fd == -1) {
        perror("accept");
        goto cleanup;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received == -1) {
        perror("recv");
        goto cleanup;
    }

    buffer[bytes_received] = '\0';
    printf("Server: received: %s\n", buffer);

    int status;
    if (waitpid(child, &status, 0) == -1) {
        perror("waitpid");
        child = -1;
        goto cleanup;
    }
    child = -1;

    if (!WIFEXITED(status) || WEXITSTATUS(status) != EXIT_SUCCESS) {
        fprintf(stderr, "Client process did not finish successfully.\n");
        goto cleanup;
    }

    result = EXIT_SUCCESS;

cleanup:
    if (client_fd != -1) {
        close(client_fd);
    }
    if (server_fd != -1) {
        close(server_fd);
    }
    if (child > 0) {
        waitpid(child, NULL, 0);
    }
    unlink(SOCKET_PATH);
    return result;
}

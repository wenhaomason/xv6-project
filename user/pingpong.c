#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

int main() {
    int ping[2];
    int pong[2];
    pipe(ping);
    pipe(pong);
    int pid = fork();
    if (pid < 0) {
        fprintf(2, "Fork failed!\n");
        exit(1);
    }

    if (pid == 0) {
        close(ping[1]);
        close(pong[0]);
        char ping_buffer[5] = {0};
        char pong_buffer[5] = "pong";
        read(ping[0], ping_buffer, 5);
        printf("%d: received %s\n", getpid(), ping_buffer);
        write(pong[1], pong_buffer, 5);
        close(ping[0]);
        close(pong[1]);
        exit(0);
    } else {
        close(ping[0]);
        close(pong[1]);
        char ping_buffer[5] = "ping";
        char pong_buffer[5] = {0};
        write(ping[1], ping_buffer, 5);
        read(pong[0], pong_buffer, 5);
        printf("%d: received %s\n", getpid(), pong_buffer);
        close(ping[1]);
        close(pong[0]);
    }

    exit(0);
}
#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int fd_ping[2];
    pipe(fd_ping);
    int fd_pong[2];
    pipe(fd_pong);
    int pid = fork();
    if (pid == -1)
    {
        fprintf(2, "Error during fork()\n");
        exit(1);
    }
    else if (pid == 0) // pong
    {
        close(fd_pong[0]);
        close(fd_ping[1]);
        char c = 0;
        read(fd_ping[0], &c, 1);
        printf("%d: received ping\n", getpid());
        write(fd_pong[1], &c, 1);
    }
    else // ping
    {
        close(fd_pong[1]);
        close(fd_ping[0]);
        char c = 0;
        write(fd_ping[1], &c, 1);
        read(fd_pong[0], &c, 1);
        printf("%d: received pong\n", getpid());
    }

    exit(0);
}

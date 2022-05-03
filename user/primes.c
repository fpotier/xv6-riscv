#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void prime_pipe(int* fd);

int
main(int argc, char *argv[])
{
    int first_pipe[2];
    pipe(first_pipe);
    for (int i = 2; i < 36; i++)
        write(first_pipe[1], &i, sizeof(i));
    close(first_pipe[1]);
    prime_pipe(first_pipe);

    exit(0);
}

void prime_pipe(int* fd)
{
    close(fd[1]);
    int first_num, num, bytes_read;
    bytes_read = read(fd[0], &first_num, sizeof(first_num));
    if (bytes_read != sizeof(first_num))
            return;
    printf("prime %d\n", first_num);
    int tmp_pipe[2];
    pipe(tmp_pipe);
    while (read(fd[0], &num, sizeof(num)) > 0)
        if (num % first_num)
            write(tmp_pipe[1], &num, sizeof(num));

    int pid = fork();
    if (pid == 0)
    {
        close(fd[0]);
        close(tmp_pipe[1]);
        prime_pipe(tmp_pipe);
        close(tmp_pipe[0]);
    }
    else if (pid > 0)
    {
        close(fd[0]);
        close(tmp_pipe[0]);
        close(tmp_pipe[1]);
        wait(0);
    }
}

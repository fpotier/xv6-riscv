#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    char buffer[512];
    int pos = 0;
    while (read(0, buffer + pos, 1) && pos < 512)
    {
        if (buffer[pos] != '\n')
        {
            pos++;
            continue;
        }
        buffer[pos] = '\0';
        char* exec_arg[MAXARG];
        for (int i = 1; i < argc; i++)
            exec_arg[i - 1] = argv[i];
        exec_arg[argc - 1] = buffer;
        int pid = fork();
        if (pid == -1)
        {
            fprintf(2, "Error during fork()\n");
            exit(1);
        }
        else if (pid == 0)
            exec(exec_arg[0], exec_arg);
        else
            wait(0);

        pos = 0;
    }

    exit(0);
}

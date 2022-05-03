#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

void find(char* path, char* pattern);

int
main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(2, "Usage: find dir pattern\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}

void
find(char* path, char* pattern)
{
    int fd;
    struct stat st;
    if((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    if(fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    struct dirent de;
    char buffer[512];
    if (st.type == T_DIR)
    {
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                continue;
            if (strcmp(de.name, pattern) == 0)
                printf("%s/%s\n", path, de.name);
            int len_path = strlen(path);
            memcpy(buffer, path, len_path);
            buffer[len_path] = '/';
            int len_ent = strlen(de.name);
            memcpy(buffer + len_path + 1, de.name, len_ent);
            buffer[len_path + len_ent + 1] = '\0';

            find(buffer, pattern);
        }
    }
    close(fd);
}

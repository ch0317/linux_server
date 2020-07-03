#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main()
{
    int outfd = dup(1);
    int fd = open("world", O_WRONLY | O_CREAT, 0666);
    dup2(fd, 1);
    printf("hello world\n");
    fflush(stdout);
    dup2(outfd, 1);

    printf("hello world\n");
    close(fd);
}

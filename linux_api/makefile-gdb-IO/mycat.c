#include<stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
int main(int argc, char *argv[])
{
    if(argc < 2){
        printf("arg error");
    }
    int fd = open(argv[1], O_RDWR | O_CREAT, 0666);
    
    write(fd,"helloworld",10);
    lseek(fd, 0,SEEK_SET);

    char buff[100];
    read(fd,buff,10);
    write(STDOUT_FILENO, buff, 10);
    close(fd);
    return 0;
}

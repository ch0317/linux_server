#include <unistd.h>
#include <stdio.h>

int main(){

    int fd[2];
    pipe(fd);
    pid_t pid = fork();
    if(pid == 0){
        sleep(3);
        write(fd[1], "hello", 5);
    }
    if(pid > 0){
        char buf[256] = {0};
        read(fd[0], buf, 5);
        printf("%s\n", buf);
    }
    return 0;
}

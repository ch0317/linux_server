#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
    printf("Begin\n");
    pid_t pid = fork();
    if(pid < 0){
        perror("error");
        exit(1);
    }
    if(pid > 0){
       printf("parent %d, %d\n", getpid(),getppid()); 
       sleep(1);
    }
    if(pid == 0){
        printf("child %d, %d\n", getpid(), getppid());
    }
    printf("END\n");
    return 0;
}

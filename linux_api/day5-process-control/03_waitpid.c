#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(){
    int i = 0;
    for(i = 0; i < 5; i++){
        pid_t pid = fork();
        if(pid == 0)
            break;
    }

    if(i == 5){
        printf("parent process");
        while(1){
            int status;
            pid_t wpid = waitpid(-1, &status, WNOHANG);
            if(wpid > 0){
                printf("wait wpid=%d\n",wpid);
            }
        }
    }

    if(i < 5)
    {
        printf("child process, i = %d, pid = %d\n",i, getpid());
        sleep(1);
    }
    
    return 0;
}

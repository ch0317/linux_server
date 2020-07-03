#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>

void catch_sig(int num)
{
    printf("catch %d sig\n", num);
}

int main(){
    signal(SIGALRM, catch_sig);
    struct itimerval myit = {{3,0},{5,0}};
    setitimer(ITIMER_REAL, &myit, NULL);

    while(1){
        printf("who can kill me!\n");
        sleep(1);
    }

    return 0;
}

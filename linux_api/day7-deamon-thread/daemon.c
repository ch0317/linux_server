#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

void my_handler()
{
    char *home = getenv("HOME");
    char strFilename[256] = {0};
    sprintf(strFilename,"%s/test/%ld",time(NULL));
   int fd = open(strFilename, O_RDWR|O_CREAT);
   if(fd < 0)
   {

   }
   close(fd);
}

int main()
{
    pid_t pid = fork();
    if(pid < 0)
    {
        printf("fork error");
    }
    if(pid == 0){
        setsid();
        chdir(getenv("HOME"));
        umask(0);

        struct itimerval t = {{0, 0},{5, 0}};
        setitimer(ITIMER_REAL, &t, NULL);
        struct sigaction act;
        act.sa_handler = my_handler;
        act.sa_flags = 0;
        sigemptyset(&act.sa_mask);
        sigaction(SIGALRM, &act, NULL);

        while(1){
            sleep(1);
        }


    }
    return 0;
}

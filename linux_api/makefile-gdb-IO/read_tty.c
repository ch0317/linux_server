#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    int ret = 0;
    char buff[256] = {0};
    int fd = open("/dev/tty", O_RDWR | O_NONBLOCK);
    while(1){
        ret = read(fd,buff,sizeof(buff));
        if(ret < 0){
            perror("read error");
        }
        if(ret){
            printf("buff%s",buff);
        }
        printf("hi\n");
        sleep(1);
    }
    fclose(fd);
    return 0;
}

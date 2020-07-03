#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>

int main(int argc, char * argv[]){
    if(argc != 2){
        printf("./a.out fifoname\n");
        return -1;
    }

    printf("begin open read...\n");
    int fd = open(argv[1], O_RDONLY);
    printf("end open read...\n");

    char buf[256];
    int ret;
    while(1){
        memset(buf, 0, sizeof(buf));
        ret = read(fd, buf, sizeof(buf));
        if(ret > 0)
            printf("read:%s\n",buf);

    }

    close(fd);
    return 0;
}

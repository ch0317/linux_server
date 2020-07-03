#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    if(argc != 2){
        printf("./a.out fifoname\n");
        return -1;
    }
    
    int fd = open(argv[1], O_WRONLY);
    char buf[256];
    int num = 1;
    while(1){
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "hi%04d", num++);
        write(fd, buf, sizeof(buf));
        sleep(1);
    }
    close(fd);

    return 0;
}

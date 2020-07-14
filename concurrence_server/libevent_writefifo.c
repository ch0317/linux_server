#include <stdio.h>
#include <event2/event.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

void write_cb(evutil_socket_t fd, short what, void * arg)
{
    char buf[1024] = {0};
    static int num = 0;
    sprintf(buf, "hello, world == %d\n", num++);
    write(fd, buf, strlen(buf) + 1);
}

int main()
{
    int fd = open("myfifo", O_WRONLY | O_NONBLOCK);
    
    struct event_base* base = NULL;
    base = event_base_new();

    struct event* ev = NULL;
    ev = event_new(base, fd, EV_WRITE | EV_PERSIST, write_cb, NULL);
    
    event_add(ev, NULL);

    event_base_dispatch(base);

    event_free(ev);
    event_base_free(base);

    return 0;
    return 0;
}


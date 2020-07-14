#include <stdio.h>
#include <event2/event.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

void read_cb(evutil_socket_t fd, short what, void * arg)
{
    char buf[1024] = {0};
    int len = read(fd, buf, sizeof(buf));
    printf("data len = %d, buf = %s\n", len, buf);
    printf("read event: %s", what & EV_READ ? "yes": "NO");
}

int main()
{
    unlink("myfifo");
    mkfifo("myfifo", 0664);

    int fd = open("myfifo", O_RDONLY | O_NONBLOCK);
    
    struct event_base* base = NULL;
    base = event_base_new();

    struct event* ev = NULL;
    ev = event_new(base, fd, EV_READ | EV_PERSIST, read_cb, NULL);
    
    event_add(ev, NULL);

    event_base_dispatch(base);

    event_free(ev);
    event_base_free(base);

    return 0;
    return 0;
}


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>

void read_cb(struct bufferevent *bev, void *arg)
{
    char buf[1024] = {0};
    bufferevent_read(bev, buf, sizeof(buf));
    char* p = "Message have been recieved.";
    printf(" %s\n",p);

    bufferevent_write(bev, p, strlen(p) + 1);
    printf("send buf: %s\n",buf); 
}

void write_cb(struct bufferevent *bev, void *arg)
{
    printf("write buffer callback\n");
}

void event_cb(struct bufferevnet *bev, short events, void *arg)
{
    if(events & BEV_EVENT_EOF)
    {
        printf("connecton closed\n");
    }
    else if(events & BEV_EVENT_ERROR)
    {
        printf("other errors.\n");
    }

    bufferevent_free(bev);
}

void cb_listener(
                 struct evconnlistener *listener,
                 evutil_socket_t fd,
                 struct sockaddr *addr,
                 int len, void *ptr
                )
{
    printf("connect new client\n");

    struct event_base* base = (struct event_base*)ptr;
    //add new event
    struct bufferevent *bev;
    bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

bufferevent_setcb(bev, read_cb, write_cb, event_cb, NULL);
    bufferevent_enable(bev, EV_READ);
}

int main()
{
    struct event_base *base = event_base_new();
    struct sockaddr_in serv;
    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(8765);
    serv.sin_addr.s_addr = htonl(INADDR_ANY);


    struct evconnlistener* listen = NULL;
    listen = evconnlistener_new_bind(base, cb_listener, base, LEV_OPT_CLOSE_ON_FREE
                                     | LEV_OPT_REUSEABLE,
                                     -1, (struct sockaddr*)&serv, sizeof(serv));


    event_base_dispatch(base);
    evconnlistener_free(listen);
    event_base_free(base);
    return 0;
}


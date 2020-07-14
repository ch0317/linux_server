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
    bufferevent_read()
}

void event_cb(struct bufferevnet *bev, short events, void *arg)
{
    if(events & BEV_EVENT_EOF)
    {
        printf("connecton closed\n");
    }
    else if(events & BEV_EVENT_ERROR)
    {

    }

    bufferevent_free(bev);
}

void listen_cb()
{
    
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
    listen = evconnlistener_new_bind(base, listen_cb,base, LEV_OPT_CLOSE_ON_FREE
                                     | LEV_OPT_REUSEABLE,
                                     -1, (struct sockaddr*)&serv, sizeof(serv));


    event_base_dispatch(base);
    evconnlistener_free(listen);
    event_base_free(base);
    return 0;
}


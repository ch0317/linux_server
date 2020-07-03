#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


void *thr(void *arg){
    while(1){
        printf("tid %lu\n",pthread_self());
        sleep(1);
    }
    return NULL;
}

int main(){
    pthread_t tid;
    pthread_create(&tid, NULL, thr, NULL);
    sleep(5);
    pthread_cancel(tid);

    void *ret;
    pthread_join(tid,&ret);
}

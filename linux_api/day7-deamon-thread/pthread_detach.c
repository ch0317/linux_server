#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void *thr(void *arg)
{
    printf("%ln\n", pthread_self());
    sleep(4);
    printf("%ln\n", pthread_self());
    return NULL;
}

int main(){
    pthread_t tid;
    pthread_create(&tid, NULL, thr, NULL);
    pthread_detach(tid);

    int ret = 0;
    if((ret=pthread_join(tid,NULL)) >0){
        printf ("err:%d\n", ret);
    }

    return 0;
}

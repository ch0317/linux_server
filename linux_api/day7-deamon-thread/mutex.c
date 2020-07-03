#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void *thr1(void *arg){
    while(1){
        pthread_mutex_lock(&mutex);
        printf("hello world");
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return NULL;
}

void *thr2(void *arg){
    while(1){
        pthread_mutex_lock(&mutex);
        printf("HELLO WORLD");
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return NULL;
}

int main(){
    pthread_t tid1;
    pthread_t tid2;

    pthread_create(&tid1,NULL, thr1, NULL);
    pthread_create(&tid2, NULL, thr2, NULL);
    while(1){
        sleep(1); 
    }

}

//
// Created by PakShing Kan on 11/20/20.
//

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

static volatile int counter =0;

pthread_mutex_t lock;

void *mythread(void *arg){
    printf("%s: Begin\n",(char*)arg);
    for(int i=0; i < 1e7; i++){
        pthread_mutex_lock(&lock);
        counter +=1;
        pthread_mutex_unlock(&lock);
    }
    printf("%s: done\n", (char *)arg);
    return NULL;
}

int main(){
    pthread_t p1, p2;
    pthread_mutex_init(&lock,NULL);
    printf("main: begin counter =%d\n", counter);

    pthread_create(&p1,NULL, mythread,"A");
    pthread_create(&p2,NULL, mythread,"B");

    pthread_join(p1,NULL);
    pthread_join(p2,NULL);
    printf("main: done with both counter = %d\n", counter);

    char str = 'a';
    int key =0;
    key += str;
    printf("%d",key);
    return 0;
}

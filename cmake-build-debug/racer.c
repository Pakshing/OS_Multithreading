//
// Created by PakShing Kan on 11/19/20.
//

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

struct racer_s{
    int id;
};

void *run_race(void *v){
    struct racer_s *racer = v;
    int i;
    printf("racer %d started\n", racer->id);
    for(i = 0; i < 3; i++){
        sleep(3);
        printf("racer %d passed checkpoint %d\n", racer->id,i);
    }
    sleep(3);
    printf("racer %d finished\n", racer->id);
    return NULL;
}

int main(){
    struct racer_s racer1 = {.id=1};
    struct racer_s racer2 = {.id=2};
    struct racer_s racer3 = {.id=3};
    pthread_t p1,p2,p3;
    pthread_create(&p1, NULL, run_race, &racer1);
    pthread_create(&p2, NULL, run_race, &racer2);
    pthread_create(&p3, NULL, run_race, &racer3);

    pthread_join(p1,NULL);
    pthread_join(p2,NULL);
    pthread_join(p3,NULL);
    return 0;
}

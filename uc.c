#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define bucket_size 256

pthread_mutex_t count_lock;
pthread_mutex_t buckets_lock;
struct Bucket *buckets[bucket_size];
unsigned int count=0;

struct Node{
    struct Node* next;
    char* word;
};

struct Bucket{
    struct Node *head;
    pthread_mutex_t head_lock;
};


unsigned int hashing(char *word){
    unsigned int key = 0;
    for(int i=0; i<strlen(word);i++){
        key+= word[i];
    }
    key = key % bucket_size;
    return key;
}


bool checkWord(char* word){
    unsigned int key = hashing(word);

    pthread_mutex_lock(&buckets_lock);
    if(buckets[key] == NULL){
        pthread_mutex_unlock(&buckets_lock);
        return false;
    }
    pthread_mutex_unlock(&buckets_lock);


    pthread_mutex_lock(&buckets[key]->head_lock);
    struct Node *cur = buckets[key]->head;
    while(cur != NULL){
        if(strcmp(cur->word,word) == 0){
            pthread_mutex_unlock(&buckets[key]->head_lock);
            return true;
        }
        cur = cur->next;
    }
    pthread_mutex_unlock(&buckets[key]->head_lock);
    return false;
}

void insert_new_node(char* word){
    struct Node* node = malloc(sizeof(struct Node));
    node->word = malloc(sizeof(strlen(word))+1);
    strcpy(node->word,word);
    node->next = NULL;
    //make new node

    int key = hashing(word);
    pthread_mutex_lock(&buckets_lock);
    if(buckets[key] == NULL){
        struct Bucket *bucket = malloc(sizeof (struct Bucket));
        struct Node *dumpy = malloc(sizeof(struct Node));
        dumpy->word = "-1";
        dumpy->next = NULL;
        bucket->head = dumpy;
        pthread_mutex_init(&bucket->head_lock,NULL);
        buckets[key] = bucket;
        //make new bucket
    }
    pthread_mutex_unlock(&buckets_lock);

    pthread_mutex_lock(&(buckets[key]->head_lock));
    struct Node *cur = buckets[key]->head;
    while(cur->next != NULL) cur = cur->next;
    cur->next = node;
    pthread_mutex_unlock(&(buckets[key]->head_lock));

}

void *mythread(void *arg){
    FILE *fh = fopen((char*)arg,"r");
    if(fh == NULL){
        printf("%s: No such file or directory\n",(char*)arg);
        return NULL;
    }
    char singleWord[50];
    char* ptr;
    fscanf(fh,"%ms",&ptr);
    while( ptr != NULL) {
        strcpy(singleWord,ptr);
        if(!checkWord(singleWord)){
            pthread_mutex_lock(&count_lock);
            count++;
            pthread_mutex_unlock(&count_lock);
            insert_new_node(singleWord);
        }
        fscanf(fh,"%ms",&ptr);
    }
    free(ptr);
    fclose(fh);
    return NULL;
}




int main(int argc, char *argv[]) {
    pthread_mutex_init(&buckets_lock,NULL);
    pthread_mutex_init(&count_lock,NULL);
    int threadsNum = argc-1;
    pthread_t threads[threadsNum];
    for(int i=0; i < threadsNum;i++){
        pthread_create(&threads[i],NULL,mythread,argv[i+1]);
        pthread_join(threads[i],NULL);
    }
    printf("%d\n", count);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define item int
#define n 5

int in = 0, out = 0;
item buffer[n];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;

void *producer () {
    int nextp = 0;
    while (1) {
        nextp = rand() % 1000 + 1;  
        printf("producer an item nextp: %d\n", nextp);
        pthread_mutex_lock(&mutex);
        buffer[in] = nextp;
        in = (in + 1) % n;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&full);
        sleep(rand() % 3);
    }
}

void *consumer () {
    int is_full = 0;
    int i = 0;
    int nextc = 0;
    while (1) {
        pthread_mutex_lock(&mutex);
        for (i = 0; i < n; i++) {
            is_full = is_full + buffer[i];
        }      
        if (is_full == 0) {
            pthread_cond_wait(&full, &mutex);
        }
        nextc = buffer[out];
        out = (out + 1) % n;
        pthread_mutex_unlock(&mutex);
        printf("consume the item in nextc: %d\n", nextc);
        sleep(rand() % 3);
    }
}

int main () {
    pthread_t pid, cid;
    srand(time(NULL));
    pthread_create(&pid, NULL, producer, NULL);
    pthread_create(&cid, NULL, consumer, NULL);
    pthread_join(pid, NULL);
    pthread_join(cid, NULL);
    return 0;
}

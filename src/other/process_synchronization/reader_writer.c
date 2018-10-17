#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t rmutex;
pthread_mutex_t wmutex;
int readcount = 0;

void *reader (void *arg) {
    int reader_id = *(int*)arg;
    while (1) {
        if (0 == readcount) {
            pthread_mutex_lock(&wmutex);
        }
        pthread_mutex_lock(&rmutex);
        ++readcount;
        pthread_mutex_unlock(&rmutex);
        printf("reader %d is reading.\n", reader_id);
        sleep(rand() % 3);
        pthread_mutex_lock(&rmutex);
        --readcount;
        pthread_mutex_unlock(&rmutex);
        if (0 == readcount) {
            pthread_mutex_unlock(&wmutex);
        }
    }
}

void *writer (void *arg) {
    int writer_id = *(int*)arg;
    while (1) {
        if (0 == readcount) {
            pthread_mutex_lock(&wmutex);
            printf("writer %d is writing.\n", writer_id);
            sleep(rand() % 3);
            pthread_mutex_unlock(&wmutex);
        } else {
            printf("writer %d failed to write.\n", writer_id);
        }
        sleep(rand() % 3);
    }
}

int main () {
    int i = 0, reader_id[5], writer_id[2];
    pthread_t rid[5], wid[2];
    srand(time(NULL));
    pthread_mutex_init(&rmutex, NULL);
    pthread_mutex_init(&wmutex, NULL);
    for (i = 0; i < 5; i++) {
        reader_id[i] = i;
        pthread_create(&rid[i], NULL, reader, (void*)&(reader_id[i]));
    }
    for (i = 0; i < 2; i++) {
        writer_id[i] = i;
        pthread_create(&wid[i], NULL, writer, (void*)&(writer_id[i]));
    }
    for (i = 0; i < 5; i++) {
        pthread_join(rid[i], NULL);
    }
    for (i = 0; i < 2; i++) {
        pthread_join(wid[i], NULL);
    }
    pthread_mutex_destroy(&rmutex);
    pthread_mutex_destroy(&wmutex);
    return 0;
}

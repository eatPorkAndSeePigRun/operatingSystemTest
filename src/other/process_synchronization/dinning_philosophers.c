#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t chopstick[5];

void *philospher (void *arg) {
    int id = *((int*)arg);
    while (1) {
        if ((id % 2) == 1) {  //奇数号哲学家
            sem_wait(&chopstick[(id + 1) % 5]);
            sem_wait(&chopstick[id]);
            printf("philospher %d is eating.\n", id);
            sleep(rand() % 3);
            sem_post(&chopstick[(id + 1) % 5]);
            sem_post(&chopstick[id]);
        } else {            //偶数号哲学家
            sem_wait(&chopstick[id]);
            sem_wait(&chopstick[(id + 1) % 5]);
            printf("philospher %d is eating.\n", id);
            sleep(rand() % 3);
            sem_post(&chopstick[id]);
            sem_post(&chopstick[(id + 1) % 5]);
        }
        printf("philospher %d is thinking.\n", id);
        sleep(rand() % 3);
    }
}

int main () {
    int i = 0, id[5];
    pthread_t pid[5];
    srand(time(NULL));
    for (i = 0; i < 5; i++) {
        sem_init(&chopstick[i], 0, 1);
        id[i] = i;
    }
    for (i = 0; i < 5; i++) {
        pthread_create(&pid[i], NULL, philospher, (void*)&(id[i]));
    }
    for (i = 0; i < 5; i++) {
        pthread_join(pid[i], NULL);
    }
    for (i = 0; i < 5; i++) {
        sem_destroy(&chopstick[i]);
    }
    return 0;
}

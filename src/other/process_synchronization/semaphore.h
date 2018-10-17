#include <pthread.h>

struct sem_t {
    int num;
    pthread_mutex_t lock;
    pthread_cond_t cond;
}

void sem_init(sem_t *sem, int num) {
    sem->num = num;
    pthread_mutex_init(&sem->lock, NULL);
    pthread_cond_init(&sem->cond, NULL);
}

void sem_wait(sem_t *sem) {
    pthread_mutex_lock(&sem->lock);
    while (0 == sem->num) {
        pthread_cond_wait(&sem->cond, &sem->lock);
    }
    sem->num--;
    pthread_mutex_unlock(&sem->lock);
}

void sem_post(sem_t *sem) {
    pthread_mutex_lock(&sem->lock);
    sem->num++;
    pthread_mutex_unlock(&sem->lock);
    pthread_cond_signal(&sem->cond);
}

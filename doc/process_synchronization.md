# 线程

### 1.线程的调用
```
pthread_create(thread, attr, func, arg) // 创建线程

从线程函数return         // 终止线程的三种方法
pthread_exit(value_ptr) // 终止自身
pthread_cancel()        // 终止同进程的另一个线程

pthread_join(thread, value_ptr)         // 挂起线程

pthread_detach(tid)     // 设置线程为detach状态

getpid()                // 获得当前进程id
pthread_self()          // 得到当前线程id
```
*注：编译时使用参数*`-lpthread`，例如：`gcc a.c -lpthread`

-----

### 2.线程间同步

- (1) mutex
```
    pthread_mutex_init(mutex, attr) 或 pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER
    pthread_mutex_destroy(mutex)

    pthread_mutex_lock(mutex) 或 pthread_mutex_trylock(mutex)
    pthread_mutex_unlock(mutex)
```

- (2) Condition Variable
```
    pthread_cond_init(cond, attr) 或 pthread_cond_t cond = PTHREAD_COND_INITIALIZER
    pthread_cond_destroy(cond)
    
    pthread_cond_wait(cond, mutex) 或 pthread_cond_timewait(cond, mutex, abstime)
    pthread_cond_signal(cond) 或 pthread_cond_broabcast(cond)
```

- (3) Semaphore
```
    sem_init(sem, pshared, value)
    sem_destroy(sem)

    sem_wait(sem) 或 sem_trywait(sem)
    sem_post(sem)
```

-----

### 3.经典进程同步问题

- (1) 生产者-消费者问题
```c
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
```

- (2) 哲学家进餐问题
避免死锁的三种方法：
    - 至多只允许有四位哲学家同时去拿左边的筷子，最终能保证至少有一位哲学家能够进餐，并在用毕时能释放出他用过的两只筷子，从而使更多的哲学家能够进餐。
    - 仅当哲学家的左、右两只筷子均可用时，才允许他拿起筷子进餐。
    - 规定奇数号哲学家先拿他左边的筷子，然后再去拿右边的筷子：而偶数号哲学家则相反。按此规定，将是1、2号哲学家竟争1号筷子；3、4号哲学家竞争3号筷子。即五位哲学家都先竟争奇数号筷子，获得后，再去竞争偶数号筷子，最后总会有一位哲学家能获得两只筷子而进餐。

```c
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
```

- (3) 读者-写者问题
为实现reader与writer间互斥设置wmutex；表示正在读的线程数readcount是临界资源，为它设置互斥量rmutex。

```c
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
```
*注：*`pthread_create()`等系统调用未做`errno`处理

-----

### 4.用Condition Variable实现Semaphore

```c
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
```

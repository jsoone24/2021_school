#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

volatile int counter = 0;
int loops;

void *worker(void *arg)
{
    int i;
    pthread_mutex_lock((pthread_mutex_t *)arg);
    for (i = 0; i < loops; i++)
    {
        counter++;
    }
    pthread_mutex_unlock((pthread_mutex_t *)arg);

    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "usage : threads <value>\n");
        exit(1);
    }

    loops = atoi(argv[1]);
    pthread_t p1, p2;
    printf("Initial value : %d\n", counter);

    pthread_mutex_t lock;
    int rc = pthread_mutex_init(&lock, NULL);
    assert(rc == 0);

    pthread_create(&p1, NULL, worker, &lock);
    pthread_create(&p2, NULL, worker, &lock);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    printf("Final value : %d\n", counter);

    return 0;
}

// lock function
// int pthread_mutex_lock(pthread_mutex_t *mutex);
// int pthread_mutex_unlock(pthread_mutex_t *mutex);

// Usage
// pthread_mutex_t lock;
// 
// int rc = pthread_mutex_init(&lock, NULL);
// assert(rc == 0); // always check success;
// 
// pthread_mutex_lock(&lock);
// x = x + 1; // or whatever your critical section is
// pthread_mutex_unlock(&lock);

// critical section : counter
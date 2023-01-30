#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

#define MAX 5

int buffer[MAX];
int fill = 0;
int use = 0;
int count = 0;
int psum = 0;
int csum = 0;

void put(int value)
{
    buffer[fill] = value;
    fill = (fill + 1) % MAX;
    count++;
    psum += (value - fill);
}

int get()
{
    int tmp = buffer[use];
    use = (use + 1) % MAX;
    count--;
    csum += (tmp - use);

    return tmp;
}

int loops = 0;
pthread_mutex_t _empty = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t _fill = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t _mutex = PTHREAD_COND_INITIALIZER;

void *producer(void *arg)
{
    int i;
    for (i = 0; i < loops; i++)
    {
        pthread_mutex_lock(&_mutex);
        while (count == MAX)
        {
            pthread_cond_wait(&_empty, &_mutex);
        }
        put(i);
        pthread_cond_signal(&_fill);
        pthread_mutex_unlock(&_mutex);
    }
}

void *consumer(void *arg)
{
    int i;
    for (i = 0; i < loops; i++)
    {
        pthread_mutex_lock(&_mutex);
        while (count == 0)
        {
            pthread_cond_wait(&_fill, &_mutex);
        }
        int tmp = get();
        pthread_cond_signal(&_empty);
        pthread_mutex_unlock(&_mutex);
        printf("%d\n", tmp);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "usage : threads <value>\n");
        exit(1);
    }
    loops = atoi(argv[1]);
    pthread_t p1, p2, p3, p4;

    pthread_create(&p1, NULL, producer, NULL);
    pthread_create(&p2, NULL, producer, NULL);
    pthread_create(&p3, NULL, consumer, NULL);
    pthread_create(&p4, NULL, consumer, NULL);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    pthread_join(p3, NULL);
    pthread_join(p4, NULL);

    if (psum == csum)
    {
        printf("Test Ok\n");
    }
    else
    {
        printf("Wrong\n");
    }

    return 0;
}

// Condition Variable function
// int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
// int pthread_cond_signal(pthread_cond_t *cond);

// Usage
// pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
// pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
//
// pthread_mutex_lock(&lock);
// while (ready == 0)
//     pthread_cond_wait(&cond, &lock);
// pthread_mutex_unlock(&lock);
//
// pthread_mutex_lock(&lock);
// ready = 1;
// pthread_cond_signal(&cond);
// pthread_mutex_unlock(&lock);

// critical section : counter
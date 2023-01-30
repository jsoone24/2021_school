#include <pthread.h>
#include <assert.h>
#include <sys/time.h>

void get_time(struct timeval* tv);
double time_diff_us(struct timeval ts, struct timeval te); 
double time_diff_ms(struct timeval ts, struct timeval te); 



// barrier
typedef struct barrier {
    pthread_cond_t complete;
    pthread_mutex_t mutex;
    int count;
    int crossing;
} barrier_t;

void barrier_init(barrier_t *b, int n);
void barrier_cross(barrier_t *b);



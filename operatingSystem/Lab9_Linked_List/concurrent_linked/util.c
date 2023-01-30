#include "util.h"


// time functions
void get_time(struct timeval* tv)  {
	assert(gettimeofday(tv, NULL) != -1);
}

double time_diff_us(struct timeval ts, struct timeval te) {
	//return te.tv_sec - ts.tv_sec + (te.tv_usec - ts.tv_usec) / 1000000.0;
	return (te.tv_sec - (double)ts.tv_sec) * 1000000.0 + te.tv_usec - ts.tv_usec;
}

double time_diff_ms(struct timeval ts, struct timeval te) {
	//return te.tv_sec - ts.tv_sec + (te.tv_usec - ts.tv_usec) / 1000000.0;
	return (te.tv_sec * 1000 + te.tv_usec / 1000)  - (ts.tv_sec * 1000 + ts.tv_usec/1000);
	//double us = (te.tv_sec - ts.tv_sec) * 1000000 + te.tv_usec - ts.tv_usec;
//	return us / 3000;
	//return te.tv_sec - ts.tv_sec + (te.tv_usec - ts.tv_usec) / 1000000.0;
}



void barrier_init(barrier_t *b, int n)
{
    pthread_cond_init(&b->complete, NULL);
    pthread_mutex_init(&b->mutex, NULL);
    b->count = n;
    b->crossing = 0;
}

void barrier_cross(barrier_t *b)
{
    pthread_mutex_lock(&b->mutex);
    /* One more thread through */
    b->crossing++;
    /* If not all here, wait */
    if (b->crossing < b->count) {
        pthread_cond_wait(&b->complete, &b->mutex);
    } else {
        pthread_cond_broadcast(&b->complete);
        /* Reset for next time */
        b->crossing = 0;
    }
    pthread_mutex_unlock(&b->mutex);
}

#if 0
static int floor_log_2(unsigned int n) {                                              
  int pos = 0;
  if (n >= 1<<16) { n >>= 16; pos += 16; }
  if (n >= 1<< 8) { n >>=  8; pos +=  8; }
  if (n >= 1<< 4) { n >>=  4; pos +=  4; }                                     
  if (n >= 1<< 2) { n >>=  2; pos +=  2; }                                     
  if (n >= 1<< 1) {           pos +=  1; }                                     
  return ((n == 0) ? (-1) : pos);                                              
}      
#endif

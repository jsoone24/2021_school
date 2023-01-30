#ifndef __MYTHREADS_h__
#define __MYTHREADS_h__

#include <pthread.h>
#include <assert.h>
#include <sched.h>

void Pthread_cond_wait(pthread_cond_t *cv,pthread_mutex_t *mutex)
{
	int rc = pthread_cond_wait(cv,mutex);
	assert(rc == 0);
}
void Pthread_cond_broadcast(pthread_cond_t *cv)
{
	int rc = pthread_cond_broadcast(cv);
	assert(rc == 0);
}

void Pthread_mutex_lock(pthread_mutex_t *mutex)
{
   int rc = pthread_mutex_lock(mutex);
   assert(rc == 0);
}

void Pthread_mutex_unlock(pthread_mutex_t *mutex)
{
    int rc = pthread_mutex_unlock(mutex);
    assert(rc == 0);
}

void Pthread_mutex_trylock(pthread_mutex_t *mutex)
{
	int rc = pthread_mutex_trylock(mutex);
	assert(rc == 0);
}                                                                                
void Pthread_create(pthread_t *thread, const pthread_attr_t *attr, 	
	       void *(*start_routine)(void*), void *arg)
{
    int rc = pthread_create(thread, attr, start_routine, arg);
    assert(rc == 0);
}

void Pthread_join(pthread_t thread, void **value_ptr)
{
    int rc = pthread_join(thread, value_ptr);
    assert(rc == 0);
}
#endif // __MYTHREADS_h__

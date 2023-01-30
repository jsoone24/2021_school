#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include "linkedlist.h"
#include "thread.h"
#include "util.h"
//#include "barrier.h"

#define _GNU_SOURCE
#define MAX_THREAD_NUM 128

#define TRC_NUM 10000

#define MAX_KEY 65536 // 2^15
#define MAX_DATA 65536 // 2^15
#define MAX_OP_NUM 10000
#define SAMPLE_NUM 1000 // per thread

enum op_type { PUT, GET };

pthread_t threads[MAX_THREAD_NUM];

struct linked_list_t* ll;


// thread args. 
typedef struct op {
	int type;
	long key;
} op_t;

typedef struct targ{ 
	int tid; 	
	linked_list_t* ll;
	barrier_t* barrier;

	int init_nops;
	op_t* init_ops;
	int nops;
	op_t* ops;

	// latency 
	double* lat;	

} targ_t;


void get_time(struct timeval* tv);
double time_diff_us(struct timeval ts, struct timeval te); 
double time_diff_ms(struct timeval ts, struct timeval te); 



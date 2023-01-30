//#define STAT_LATENCY
//#define STAT_COUNT

#define PUT_SEQ 0
#define PUT_RAND 1
#define GET_SEQ 2
#define GET_RAND 3
#define TRC_BENCH 4

#define LINE_SIZE 100
//#define MACRO_BENCH_TRC 2

#ifdef DEBUG
#define DEBUG_PRINT(x) printf x
#else
#define DEBUG_PRINT(x) do {} while (0)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include <stdatomic.h> 
#include <string.h> // memset

#include <unistd.h>
#include <sys/types.h>
#include "common.h" // CAS 

typedef struct node {
	long key;	
	long value;
	long level;
	struct node** next; 
} node_t;

typedef struct linked_list {
	node_t* list_head;
#ifdef BLINKED_LIST
	pthread_mutex_t list_lock;
#endif
} linked_list_t;


int linked_list_init();
void linked_list_destroy();
long linked_list_put (long key, long data, linked_list_t* ll);
long linked_list_get (long key, linked_list_t* ll);


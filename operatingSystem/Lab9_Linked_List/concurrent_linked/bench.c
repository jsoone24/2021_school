#include "bench.h"


void *do_operation(void* arg)
{
	targ_t* targ = (targ_t*) arg;
	linked_list_t* ll = targ->ll;	

	long rv;
	int nops = targ->nops;	

	// initial operations  
	for(int n = 0; n < targ->init_nops; n++) {
		int type = targ->init_ops[n].type;
		long key = targ->init_ops[n].key;

		if (type == PUT) {
			rv = linked_list_put(key, key, ll);
			assert(!rv);
		} else if (type == GET) {
			rv = linked_list_get(key, ll);
		}
	}


	// perform operations 
	barrier_cross(targ->barrier);

	DEBUG_PRINT(("nops = %d\n", nops));

	for(int n = 0; n < nops; n++) {
		int type = targ->ops[n].type;
		long key = targ->ops[n].key;

		if (type == PUT) {
			rv = linked_list_put(key, key, ll);
			assert(!rv);
		} else if (type == GET) {
			rv = linked_list_get(key, ll);
		}
	}
	return (void**) stat;

}

//------------- main -------------- //
int main(int argc, char* argv[])
{

	if(argc < 4) {
		printf("Usage: %s threads bench_type nops \n", argv[0]);
		printf("bench_type: PUT_SEQ = 0, PUT_RAND = 1, GET_SEQ = 2, GET_RAND = 3\n");
		return -1;
	}
	
	linked_list_t* ll;

	int nthreads = atoi(argv[1]);
	int bench = atoi(argv[2]);
	int init_nops = 0; 	// number of operations for initialization 
	int nops = 0; 		// number of operations for test  
	int sidx;			// key suffix for sequential put 
	barrier_t barrier;


	// initialize linked list  
	linked_list_init(&ll);
	barrier_init(&barrier, nthreads + 1);

	// initialize keys  
	targ_t* targ = (targ_t*) malloc (sizeof(targ_t) * nthreads);

	for (int t = 0; t < nthreads; t++) {
		targ[t].tid = t;
		targ[t].ll = ll;
		targ[t].barrier = &barrier;

		// set dataset 
		switch(bench) {
			case PUT_SEQ:
				DEBUG_PRINT(("PUT_SEQ\n"));
				nops = atoi(argv[3]) / nthreads;
				targ[t].init_nops = 0;	
				targ[t].nops = nops;
				targ[t].ops = (op_t*) malloc (sizeof(op_t) * nops);
		
				sidx = nops * t;

				for (int o = 0; o < nops; o++) {
					targ[t].ops[o].type = PUT;
					targ[t].ops[o].key =  (long) (o + sidx);
				}
				break;	
			case PUT_RAND:
				DEBUG_PRINT(("PUT_RAND\n"));
				nops = atoi(argv[3]) / nthreads;
				targ[t].init_nops = 0;
				targ[t].nops = nops;
				targ[t].ops = (op_t*) malloc (sizeof(op_t) * nops);
				for (int o = 0; o < nops; o++) {
					targ[t].ops[o].type = PUT;
					targ[t].ops[o].key = (long) rand() % MAX_KEY;
					//printf("key = %ld\n", targ[t].ops[o].key);
				}
				break;	

			case GET_SEQ:
				DEBUG_PRINT(("GET_SEQ\n"));
				nops = atoi(argv[3]) / nthreads;
				init_nops = nops;

				// initial put 
				targ[t].init_nops = init_nops;
				targ[t].init_ops = (op_t*) malloc (sizeof(op_t) * init_nops);

				for (int o = 0; o < init_nops; o++) {
					targ[t].init_ops[o].type = PUT;
					targ[t].init_ops[o].key =  (long) o;
				}

				// get
				targ[t].nops = nops;
				targ[t].ops = (op_t*) malloc (sizeof(op_t) * nops);
				for (int o = 0;o < nops; o++) {
					targ[t].ops[o].type = GET;
					targ[t].ops[o].key =  (long) o;
				}
				break;	

			case GET_RAND:
				DEBUG_PRINT(("GET_RAND\n"));
				nops = atoi(argv[3]) / nthreads;
				init_nops = nops;

				// initial put 
				targ[t].init_nops = init_nops;
				targ[t].init_ops = (op_t*) malloc (sizeof(op_t) * init_nops);

				for (int o = 0; o < init_nops; o++) {
					targ[t].init_ops[o].type = PUT;
					targ[t].init_ops[o].key =  (long) o;
				}

				// get
				targ[t].nops = nops;
				targ[t].ops = (op_t*) malloc (sizeof(op_t) * nops);
				for (int o = 0;o < nops; o++) {
					targ[t].ops[o].type = GET;
					targ[t].ops[o].key = (long) rand() % nops;
				}
				break;	

			default:
				printf("Unknown option\n");
				break;
		}
	}

	//---------------- Run Threads ---------------- // 
	struct timeval ts;
	struct timeval te;

	// create threads 
	for (int i=0; i < nthreads; i++){
		Pthread_create(&threads[i], NULL, do_operation, (void*) &targ[i]);
	}

	double** rv = (double**) malloc (sizeof(double*) * nthreads);

	// barrier 
	barrier_cross(&barrier);

	// do benchmark 
	get_time(&ts);

	for (int i=0; i < nthreads; i++){
			Pthread_join(threads[i], (void **) &rv[i]);
	}	

	get_time(&te);


	for(int i = 0; i < nthreads; i++) {
		free(targ[i].ops);
	}
	free(targ);


	//---------------- Print Results---------------- // 

	double exec_time = time_diff_ms(ts, te);
	nops = nops * nthreads; 

	switch(bench) {
		case PUT_SEQ:
			fprintf(stdout, "PUT_SEQ");
			break;
		case PUT_RAND:
			fprintf(stdout, "PUT_RAND");
			break;
		case GET_SEQ:
			fprintf(stdout, "GET_SEQ");
			break;
		case GET_RAND:
			fprintf(stdout, "GET_RAND");
			break;
	}		
	fprintf(stdout, " ops: %d time(ms): %10.2f iops: %10.2f", 
			nops, exec_time, (nops / exec_time) * 1000);

	fprintf(stdout, "\n");


	linked_list_destroy(ll);

	return 0;
}



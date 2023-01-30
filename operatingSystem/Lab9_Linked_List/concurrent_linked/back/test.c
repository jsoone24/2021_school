#include "test.h"


//-------- do_operation -----------// 
void *do_operation(void* arg)
{
	targ_t* targ = (targ_t*) arg;
	linked_list_t* ll = targ->ll;	

	DEBUG_PRINT(("linked_list = %p\n", (void*)ll));
	DEBUG_PRINT(("linked_list head next = %p\n", (void*)ll->list_head->next));

	long val; 
	int nops = targ->nops;	

#ifdef STAT_LATENCY
	struct timeval ts;
	struct timeval te;
	targ->lat = (double*) malloc (sizeof(double) * nops);
#endif

	DEBUG_PRINT(("init_nops = %d\n", targ->init_nops));
	for(int n = 0; n < targ->init_nops; n++) {
		int type = targ->init_ops[n].type;
		long key = targ->init_ops[n].key;

		if (type == PUT) {
			linked_list_put(key, key, ll);
		} else if (type == GET) {
			val = linked_list_get(key, ll);
		}
	}

	// ----------------------// 
	barrier_cross(targ->barrier);

#ifdef STAT_COUNT
	linked_list_reset_stat();
	barrier_cross(targ->barrier);
#endif

	DEBUG_PRINT(("nops = %d\n", nops));

	for(int n = 0; n < nops; n++) {
		int type = targ->ops[n].type;
		long key = targ->ops[n].key;

#ifdef STAT_LATENCY
		get_time(&ts);
#endif

		if (type == PUT) {
#ifdef BLINKED_LIST
			pthread_mutex_lock(&ll->list_lock);
#endif
			linked_list_put(key, key, ll);

#ifdef BLINKED_LIST
			pthread_mutex_unlock(&ll->list_lock);
#endif
		} else if (type == GET) {
			val = linked_list_get(key, ll);
			if(val == key) 
				STAT_FAI(&ll->found_cnt);
		}
#ifdef STAT_LATENCY
		get_time(&te);
		targ->lat[n] = time_diff_us(ts, te);
#endif

	}
	return (void**) stat;

}



//------------- main -------------- //
int main(int argc, char* argv[])
{

	if(argc < 4) {
		printf("Usage: %s threads bench_type nops load_trc run_trc \n", argv[0]);
		printf("bench_type: PUT_SEQ = 0, PUT_RAND = 1, GET_SEQ = 2, GET_RAND = 3, TRC = 4\n");
		return -1;
	}
	
	linked_list_t* ll;

	int nthreads = atoi(argv[1]);
	int bench = atoi(argv[2]);
	int nops = 0, init_nops = 0;
	char run_trc_fname[100];
	char load_trc_fname[100];
	int lines = 0;
	int sidx;
	barrier_t barrier;


	// init skip list  
	linked_list_init(&ll);
	barrier_init(&barrier, nthreads + 1);

	DEBUG_PRINT(("[main] linked_list = %p\n", (void*) ll));
	DEBUG_PRINT(("[main] linked_list head next = %p\n", (void*)(ll->list_head->next)));

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
					//printf("key = %ld\n", targ[t].ops[o].key);
				}

				// get
				targ[t].nops = nops;
				targ[t].ops = (op_t*) malloc (sizeof(op_t) * nops);
				for (int o = 0;o < nops; o++) {
					targ[t].ops[o].type = GET;
					targ[t].ops[o].key =  (long) o;
					//printf("key = %ld\n", targ[t].ops[o].key);
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
					//printf("key = %ld\n", targ[t].ops[o].key);
				}

				// get
				targ[t].nops = nops;
				targ[t].ops = (op_t*) malloc (sizeof(op_t) * nops);
				for (int o = 0;o < nops; o++) {
					targ[t].ops[o].type = GET;
					targ[t].ops[o].key = (long) rand() % nops;
					//printf("key = %ld\n", targ[t].ops[o].key);
				}
				break;	

			case TRC_BENCH:
				DEBUG_PRINT(("TRC_BENCH\n"));

				// initial trc 
				strcpy(load_trc_fname, argv[3]);

				FILE* fp = fopen(load_trc_fname, "r");
				if(fp == NULL){
					printf("Failed to open file %s\n", load_trc_fname);
					return -1;
				}

				char buff[LINE_SIZE];

				// get lines 
				lines = 0;
				while(fgets(buff, LINE_SIZE-1, fp) != NULL) 
					lines++;
				init_nops = lines / nthreads; 
				targ[t].init_nops = init_nops;

				// read trc 
				fseek(fp, 0, SEEK_SET);
				targ[t].init_ops = (op_t*) malloc (sizeof(op_t) * init_nops);	

				int o = 0;
				for (int ln = 0; ln < lines; ln++) {
					int type;
					long key;
					
					int rv = fscanf(fp, "%d %ld\n", &type, &key);
					if(rv == EOF) break;

					if (ln % nthreads == t) {
						targ[t].init_ops[o].type = type;
						targ[t].init_ops[o].key = key;
						o++;
					}
				}
				//assert(o == init_nops);
				fclose(fp);
	
				// run 
				strcpy(run_trc_fname, argv[4]);

				fp = fopen(run_trc_fname, "r");
				if(fp == NULL){
					printf("Failed to open file%s\n", run_trc_fname);
					return -1;
				}

		//		char buff[LINE_SIZE];

				// get lines 
				lines = 0;
				while(fgets(buff, LINE_SIZE-1, fp) != NULL) 
					lines++;

				nops = lines / nthreads; 
				targ[t].nops = nops;

				// read trc 
				fseek(fp, 0, SEEK_SET);
				targ[t].ops = (op_t*) malloc (sizeof(op_t) * nops);	

				o = 0;
				for (int ln = 0; ln < lines; ln++) {
					int type;
					long key;
					
					int rv = fscanf(fp, "%d %ld\n", &type, &key);
					if(rv == EOF) break;

					if (ln % nthreads == t) {
						targ[t].ops[o].type = type;
						targ[t].ops[o].key = key;
						o++;
					}
				}
				//assert(o == nops);
				fclose(fp);

				break;

			default:
				printf("Unknown option\n");
				break;
		}
	}


#ifdef BLINKED_LIST
	pthread_mutex_init(&(ll->list_lock), NULL);
#endif


	//---------------- Run Threads ---------------- // 
	struct timeval ts;
	struct timeval te;

	// create threads 
	for (int i=0; i < nthreads; i++){
			//Pthread_create(&threads[i], NULL, do_write_with_trc, (void*) &ops);
			Pthread_create(&threads[i], NULL, do_operation, (void*) &targ[i]);
	}

	double** rv = (double**) malloc (sizeof(double*) * nthreads);

	// barrier 
	barrier_cross(&barrier);
#ifdef STAT_COUNT
	linked_list_reset_stat();
	barrier_cross(&barrier);
#endif

	get_time(&ts);

	// wait threads 
	for (int i=0; i < nthreads; i++){
			Pthread_join(threads[i], (void **) &rv[i]);
	}	

	// end time 
	get_time(&te);

#ifdef STAT_LATENCY

	char lfname[100]; 
	strcpy(lfname, argv[5]);
//	strcpy(lfname, run_trc_fname);
//	strcat(lfname, argv[0]);
//	strcat(lfname, "_");
//	strcat(lfname, argv[1]); // nthreads 
//	strcat(lfname, ".lat");
//
	FILE* ofp = fopen(lfname, "w");
	assert(ofp != NULL);

	for(int i = 0; i < nthreads; i++) {
		for(int j = 0; j < nops; j++) { 
			fprintf(ofp, "%.2lf\n", targ[i].lat[j]);
			//fprintf(ofp, "%d %.2lf\n", bench, targ[i].lat[j]);
		}
	}
#endif

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
		case TRC_BENCH:
			fprintf(stdout, "TRC_BENCH");
			break;
	}		
	fprintf(stdout, " ops: %d time(ms): %10.2f iops: %10.2f", 
			nops, exec_time, (nops / exec_time) * 1000);

	if(bench == TRC_BENCH)
		fprintf(stdout, " trc: %s\n", run_trc_fname);
	else
		fprintf(stdout, "\n");


#ifdef STAT_COUNT
	linked_list_stat(ll);

#endif
	// destroy linked_list
	linked_list_destroy(ll);

	return 0;
}




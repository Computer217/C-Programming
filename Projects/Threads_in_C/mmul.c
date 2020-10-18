#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "matrix.h"

// Search TODO to find the locations where code needs to be completed

#define     NUM_THREADS     2

typedef struct {
    unsigned int id;
    TMatrix *m, *n, *t;
} thread_arg_t;

static void * thread_main(void * p_arg)
{
    // TODO
    thread_arg_t *arg = p_arg;
    for (unsigned int i = arg->id; i < arg->m->nrows; i+=2){
    	for (unsigned int j = 0; j < arg->n->ncols; j++){
    		TElement s = (TElement)0;
    		for (unsigned int x = 0; x < arg->m->ncols; x++){
    			s += arg->m->data[i][x] * arg->n->data[x][j];
    		}
    		arg->t->data[i][j] = s;
    	}
    }
    return NULL;
}

/* Return the sum of two matrices.
 *
 * If any pthread function fails, report error and exit. 
 * Return NULL if anything else is wrong.
 *
 * Similar to mulMatrix, but with multi-threading.
 */
TMatrix * mulMatrix_thread(TMatrix *m, TMatrix *n)
{
    if (    m == NULL || n == NULL
         || m->ncols != n->nrows )
        return NULL;

    TMatrix * t = newMatrix(m->nrows, n->ncols);
    if (t == NULL)
        return t;

    // TODO
    //array of threads 
    pthread_t thread_ids[NUM_THREADS];
    //array of structures 
	thread_arg_t thread_args[NUM_THREADS];
    
    //create threads and fill in the structure
    
    for (int i = 0; i < NUM_THREADS; i++) {
    	//fill in structure for corresponding thread id
		thread_args[i].m = m;
		thread_args[i].t = t;
		thread_args[i].n = n;
		thread_args[i].id = i;
		//create the thread each thread is assigned a structure 
		int rv = pthread_create(&thread_ids[i], NULL, thread_main, &thread_args[i]);
		//check for error
		if (rv) {
			fprintf(stderr, "pthread_create() returned %d\n", rv);
			exit(1);
		}
	}
	
	//wait for threads
	for (int i = 0; i < NUM_THREADS; i++) {
		int rv = pthread_join(thread_ids[i], NULL);
		if (rv) {
			fprintf(stderr, "pthread_join() returned %d\n", rv);
			exit(1);
		}
	}
    
    return t;
}

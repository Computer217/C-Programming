#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "matrix.h"

#define     NUM_THREADS     2

typedef struct {
    unsigned int id;
    TMatrix *m, *n, *t;
} thread_arg_t;

/* the main function of threads */
static void * thread_main(void * p_arg)
{
	thread_arg_t * arg = (thread_arg_t *) p_arg;
	int start;
	//do the even
	if (arg->id == 0){
		start = arg->id;
	}
	else{
		start = arg->id;
	}
	
	for (int i = start; i < arg->n->nrows; i+=2){
			for (int j = 0; j < arg->n->ncols; j+=1){
				arg->t->data[i][j]= arg->m->data[i][j] + arg->n->data[i][j];
			}
	}
	
    return NULL;
}

/* Return the sum of two matrices. The result is in a newly creaed matrix. 
 *
 * If a pthread function fails, report error and exit. 
 * Return NULL if something else is wrong.
 *
 * Similar to addMatrix, but this function uses 2 threads.
 */
TMatrix * addMatrix_thread(TMatrix *m, TMatrix *n)
{
    if (    m == NULL || n == NULL
         || m->nrows != n->nrows || m->ncols != n->ncols )
        return NULL;

    TMatrix * t = newMatrix(m->nrows, m->ncols);
    if (t == NULL)
        return t;
    
    pthread_t threads[NUM_THREADS];
    thread_arg_t args[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i ++) {
        args[i].id = i;
        args[i].m = m;
        args[i].n = n;
    	args[i].t = t;
       
        int rv = pthread_create(&threads[i], NULL, thread_main, &args[i]);
        if (rv != 0) {
            perror("threat not created");
            exit(1);
        }
    }
    
    for ( int i = 0; i < NUM_THREADS; i++ ){
    	pthread_join(threads[i], NULL);
    }

    // TODO
    return t;
}

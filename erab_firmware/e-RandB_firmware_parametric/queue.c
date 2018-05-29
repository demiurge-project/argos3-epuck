
/*	queue.c

	Implementation of a FIFO queue abstract data type.

	by: Steven Skiena
	begun: March 27, 2002
*/


/*
Copyright 2003 by Steven S. Skiena; all rights reserved. 

Permission is granted for use in non-commerical applications
provided this copyright notice remains intact and unchanged.

This program appears in my book:

"Programming Challenges: The Programming Contest Training Manual"
by Steven Skiena and Miguel Revilla, Springer-Verlag, New York 2003.

See our website www.programming-challenges.com for additional information.

This book can be ordered from Amazon.com at

http://www.amazon.com/exec/obidos/ASIN/0387001638/thealgorithmrepo/

*/

#include "queue.h"

int getQueueIndexArray(int frameDataLength){
	return getFrameBits(frameDataLength) + (2 * EOT_LENGTH);
}

void init_queue(queue *q)
{
        q->first = 0;
        q->last = QUEUESIZE-1;
        q->count = 0;
}

int QueueIn(queue *q, unsigned int *x)
{        
	if (q->count >= QUEUESIZE){
		return FALSE;
	}
  	else {
		int i;
		int queueIndexArray = getQueueIndexArray(get_data_length());
		int aux[queueIndexArray];
        q->last = (q->last+1) % QUEUESIZE;
		for ( i = 0 ; i < queueIndexArray ; i ++){
        	q->q[ q->last ][i] = x[i];    	
			aux[i] = x[i];
		}
		

        q->count = q->count + 1;
		return TRUE;
    }
	return TRUE;
}

int QueueOut(queue *q, unsigned int* x)
{
	int i;
    if (q->count <= 0) {
		return FALSE;
	}
    else {
		int queueIndexArray = getQueueIndexArray(get_data_length());
		for ( i = 0; i< queueIndexArray; i++){
			x[i]=q->q[ q->first ][i];
		}
        q->first = (q->first+1) % QUEUESIZE;
		
        q->count = q->count - 1;
		return TRUE;
   	}
}

int QueueEmpty(queue *q)
{
        if (q->count <= 0) return (TRUE);
        else return (FALSE);
}

/*** SINGLE QUEUE */

void init_queue_single(queueSingle *q)
{
        q->first = 0;
        q->last = QUEUESIZESINGLE-1;
        q->count = 0;
}

int QueueInSingle(queueSingle *q, unsigned int x)
{        
	if (q->count >= QUEUESIZESINGLE){
		return FALSE;
	}
  	else {
        q->last = (q->last+1) % QUEUESIZESINGLE;
        q->q[ q->last ] = x;    	
	
        q->count = q->count + 1;
		return TRUE;
    }
	return TRUE;
}

int QueueOutSingle(queueSingle *q, unsigned int* x)
{
    if (q->count <= 0) {
		return FALSE;
	}
    else {
    	*x = q->q[ q->first ];
        q->first = (q->first+1) % QUEUESIZESINGLE;
		
        q->count = q->count - 1;
		return TRUE;
   	}
}

int QueueEmptySingle(queueSingle *q)
{
        if (q->count <= 0) return (TRUE);
        else return (FALSE);
}

int QueueStatusSingle(queueSingle *q){
	return q->count;
}

void QueueClearSingle(queueSingle *q)
{
	q->first = 0;
    q->last = QUEUESIZESINGLE-1;
    q->count = 0;
}

/*FINAL QUEUE*/

void init_queue_final(queueFinal *q)
{
        q->first = 0;
        q->last = QUEUESIZEFINAL-1;
        q->count = 0;
}

int QueueInFinal(queueFinal *q, finalDataRegister x)
{        
	if (q->count >= QUEUESIZEFINAL){
		return FALSE;
	}
  	else {
        q->last = (q->last+1) % QUEUESIZEFINAL;

		q->q[ q->last ] = x;

        q->count = q->count + 1;
		return TRUE;
    }
	return TRUE;
}

int QueueOutFinal(queueFinal *q, finalDataRegister *x)
{
    if (q->count <= 0) {
		return FALSE;
	}
    else {
		*x=q->q[ q->first ];

        q->first = (q->first+1) % QUEUESIZEFINAL;
		
        q->count = q->count - 1;
		return TRUE;
   	}
}

int QueueEmptyFinal(queueFinal *q)
{
        if (q->count <= 0) return (TRUE);
        else return (FALSE);
}



/* QUEUE MULTIPLE */

void init_queueMultiple(queueMultiple *q)
{
        q->first = 0;
        q->last = QUEUESIZEMULTIPLE-1;
        q->count = 0;
}

int QueueInMultiple(queueMultiple *q, unsigned char *x)
{        
	if (q->count >= QUEUESIZEMULTIPLE){
		return FALSE;
	}
  	else {
		int i;
//		int aux[NUM_RECEPTOR_SENSORS];
        q->last = (q->last+1) % QUEUESIZEMULTIPLE;
		for ( i = 0 ; i < NUM_RECEPTOR_SENSORS ; i ++){
        	q->q[ q->last ][i] = x[i];    	
//			aux[i] = x[i];
		}

        q->count = q->count + 1;
		return TRUE;
    }
	return TRUE;
}

int QueueOutMultiple(queueMultiple *q, unsigned char* x)
{
	int i;
    if (q->count <= 0) {
		return FALSE;
	}
    else {
		for ( i = 0; i< NUM_RECEPTOR_SENSORS; i++){
			x[i]=q->q[ q->first ][i];
		}
        q->first = (q->first+1) % QUEUESIZEMULTIPLE;
		
        q->count = q->count - 1;
		return TRUE;
   	}
}

int QueueEmptyMultiple(queueMultiple *q)
{
        if (q->count <= 0) return (TRUE);
        else return (FALSE);
}

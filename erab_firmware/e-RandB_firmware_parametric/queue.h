/*	queue.h

	Header file for queue implementation

	by: Steven Skiena
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

#include "PWMEmission.h"
#include "registers.h"

#include "ports.h"
#include "DataLength.h"


#define QUEUESIZE       	10
#define QUEUEINDEXARRAY_32		FRAME_BITS_32 + (2 * EOT_LENGTH)
#define QUEUESIZESINGLE     110
#define QUEUESIZEFINAL     	10
#define QUEUESIZEMULTIPLE   1


int getQueueIndexArray(int frameDataLength);


typedef struct {
		int q[QUEUESIZE+1][QUEUEINDEXARRAY_32];	/* body of queue */
        int first;                      		/* position of first element */
        int last;                       		/* position of last element */
        int count;                      		/* number of queue elements */
} queue;


void init_queue(queue *q);
int QueueIn(queue *q, unsigned int *x);
int QueueOut(queue *q, unsigned int *value);
int QueueEmpty(queue *q);


typedef struct {
        unsigned int q[QUEUESIZESINGLE+1];		/* body of queue */
        int first;                      /* position of first element */
        int last;                       /* position of last element */
        int count;                      /* number of queue elements */
} queueSingle;


void init_queue_single(queueSingle *q);
int QueueInSingle(queueSingle *q, unsigned int x);
int QueueOutSingle(queueSingle *q, unsigned int *x);
int QueueEmptySingle(queueSingle *q);
int QueueStatusSingle(queueSingle *q);
void QueueClearSingle(queueSingle *q);

typedef struct {
        finalDataRegister q[QUEUESIZEFINAL+1];	/* body of queue */
        int first;                      			/* position of first element */
        int last;                       			/* position of last element */
        int count;                      			/* number of queue elements */
} queueFinal;


void init_queue_final(queueFinal *q);
int QueueInFinal(queueFinal *q, finalDataRegister x);
int QueueOutFinal(queueFinal *q, finalDataRegister *x);
int QueueEmptyFinal(queueFinal *q);
//int QueueStatusFinal(queueFinal *q);


typedef struct {
        int q[QUEUESIZEMULTIPLE+1][NUM_RECEPTOR_SENSORS];	/* body of queue */
        int first;                      			/* position of first element */
        int last;                       			/* position of last element */
        int count;                      			/* number of queue elements */
} queueMultiple;


void init_queueMultiple(queueMultiple *q);
int QueueInMultiple(queueMultiple *q, unsigned char *x);
int QueueOutMultiple(queueMultiple *q, unsigned char *value);
int QueueEmptyMultiple(queueMultiple *q);

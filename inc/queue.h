#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "AVLtree.h"
#include "searchtree.h"

//typedef AVLtree *elementType;



typedef searchTree *elementType;

#define MinQueueSize 	5

//typedef int elementType;

typedef struct queueRecord
{
	int capacity;
	int front;
	int rear;
	int size;
	elementType *array;
}QUEUE;

int queue_test_01(void);

int isEmpty(QUEUE *q);
int isFull(QUEUE *q);
QUEUE *createQueue(int maxElements);
void disposeQueue(QUEUE *q);
void makeEmpty(QUEUE *q);
void enQueue(elementType element, QUEUE *q);
elementType front(QUEUE *q);
void deQueue(QUEUE *q);
elementType frontAndDequeue(QUEUE *q);

#endif

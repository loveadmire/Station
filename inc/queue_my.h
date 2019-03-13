#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>


typedef struct node
{
	char *ip;
	long int timestamp;
	struct node* next;
}Node;


typedef struct queue
{
	Node* head;
	Node* tail;
}Queue;

void InitQueue(Queue*);
int Search_ip(Queue*, char *);

int LengQueue(Queue*);
void EnQueue(Queue*, char*, long int);
int DeQueue(Queue* queue);
void PrintQueue(Queue* queue);
int IsNull(Queue* queue);
void DelQueue(Queue* queue);

#if 0
int test_queue(void)
{
	Queue queue;
	InitQueue(&queue);
	//printf("IsNull = %d\n", IsNull(&queue));
	printf("enque 4 times and the elems: 1, 2, 3, 4\n");
	EnQueue(&queue, "aaaa",32);

	PrintQueue(&queue);
	printf("deque 1 times.\n");
	DeQueue(&queue);
	PrintQueue(&queue);

	printf("IsNull = %d\n", IsNull(&queue));
	DelQueue(&queue);
	printf("IsNull = %d\n", IsNull(&queue));

	return 0;
}
#endif

#endif //QUEUE_H

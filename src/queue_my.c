#include "queue_my.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void DelQueue(Queue* queue)
{
	while (queue->head != queue->tail)
	{
		DeQueue(queue);
	}

}
//����ip�Ƿ��Ѿ��Ƿ����ʣ��ǣ�����0���µ�ip����-1��
int Search_ip(Queue* queue, char *ip) {
	if (IsNull(queue)) {
		return 0;
	}
	if (ip == NULL) {
		return 0;
	}
	int statuse = 1;
	Node* curNode = queue->head->next;
	while (curNode) {
		if (curNode->next != NULL)
		{
			if (strcmp(ip, curNode->ip) == 0) {
				statuse = 0;
			}
		}
		else
		{
			if (strcmp(ip, curNode->ip) == 0) {
				statuse = 0;
			}
		}
		curNode = curNode->next;
	}
	return statuse;
}

int LengQueue(Queue* queue) {
	int size = 0;
	if (IsNull(queue))
	{
		return size;
	}
	Node* curNode = queue->head->next;
	while (curNode)
	{
		if (curNode->next != NULL)
		{
			size += 1;
		}
		else
		{
			return size + 1;
		}
		curNode = curNode->next;
	}
	return size;
}

void PrintQueue(Queue* queue)
{
	if (IsNull(queue))
	{
		printf("empty queue.\n");
		return;
	}
	Node* curNode = queue->head->next;
	while (curNode)
	{
		if (curNode->next != NULL)
		{
			printf("ip is :%s ", curNode->ip);
			printf("%ld ", curNode->timestamp);
		}
		else
		{
			//ֻ��һ����ʱ��
			printf(" it is %s:", curNode->ip);
			printf("it is %ld", curNode->timestamp);

		}
		curNode = curNode->next;
	}
	printf("\n");

}


void InitQueue(Queue* queue)
{
	queue->tail = (Node*)malloc(sizeof(Node));//warning
	queue->tail->ip = NULL;
	queue->tail->timestamp = 0;
	queue->head = queue->tail;
	queue->tail->next = NULL;
}
//�����
void EnQueue(Queue* queue, char *ip, long int timestamp)
{
	Node * newNode = (Node*)malloc(sizeof(Node));//warning
	newNode->ip = ip;
	newNode->timestamp = timestamp;
	newNode->next = NULL;
	queue->tail->next = newNode;//2b ����©�ˡ�������������������ȥ
	queue->tail = newNode;
}

int DeQueue(Queue* queue)
{
	long int  popValue = queue->head->timestamp;
	char *ip = queue->head->ip;
	Node *popNode = queue->head;
	queue->head = queue->head->next;
	free(queue->head->ip);
	free(popNode);//warning
	return 0;
}
//1 means Null
int IsNull(Queue* queue)
{
	return (queue->head == queue->tail);
}
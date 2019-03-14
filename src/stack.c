#include <stdlib.h>
#include <stdio.h>
#include "../inc/stack.h"

int Stack_isEmpty(STACK *s)
{
	return (s->stackTop == EmptyTOS);
}

int Stack_isFull(STACK *s)
{
	return (s->stackTop == s->capacity -1);
}

STACK *createStack(int maxElements)
{
	STACK *s;

	if(maxElements < MinStackSize) {
		printf("Stack size si too small\n");	
		return NULL;
	}
	s = (STACK *)malloc(sizeof( STACK));
	s->array = (elementType *)malloc(sizeof(elementType) * maxElements);
	s->capacity = maxElements;
	Stack_makeEmpty(s);

	return s;

}
void disposeStack(STACK *s)
{
	if(s != NULL) {
		free(s->array);	
		free(s);
	}
}

void Stack_makeEmpty(STACK *s)
{
	if(s != NULL) {
		s->stackTop = EmptyTOS;	
	}
}

void push(elementType element, STACK *s)
{
	if(!Stack_isFull(s)) {
		s->stackTop++;
		s->array[s->stackTop] = element;
	} else {
		printf("full stack\n");
	}
}

elementType top(STACK *s)
{
	if(!Stack_isEmpty(s)) {
		return s->array[s->stackTop];	
	} else {
		printf("empty stack\n");
		return 0;
	}
}

void pop(STACK *s)
{
	if(!Stack_isEmpty(s))
		s->stackTop--;	
	else
		printf("empty stack\n");
}

elementType topAndTop(STACK *s)
{
	if(!Stack_isEmpty(s)) {
		return s->array[s->stackTop--];
	} else {
		printf("empty stack\n");	
		return 0;
	}
}




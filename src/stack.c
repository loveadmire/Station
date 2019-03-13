#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

int isEmpty_stack(STACK *s)
{
	return (s->stackTop == EmptyTOS);
}

int isFull_stack(STACK *s)
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
	s = (STACK *)malloc(sizeof(STACK));
	s->array = (elementType *)malloc(sizeof(elementType) * maxElements);
	s->capacity = maxElements;
	makeEmpty_stack(s);

	return s;

}
void disposeStack(STACK *s)
{
	if(s != NULL) {
		free(s->array);	
		free(s);
	}
}

void makeEmpty_stack(STACK *s)
{
	if(s != NULL) {
		s->stackTop = EmptyTOS;	
	}
}

void push(elementType element, STACK *s)
{
	if(!isFull_stack(s)) {
		s->stackTop++;
		s->array[s->stackTop] = element;
	} else {
		printf("full stack\n");
	}
}

elementType top(STACK *s)
{
	if(!isEmpty_stack(s)) {
		return s->array[s->stackTop];	
	} else {
		printf("empty stack\n");
		return 0;
	}
}

void pop(STACK *s)
{
	if(!isEmpty_stack(s))
		s->stackTop--;	
	else
		printf("empty stack\n");
}

elementType topAndTop(STACK *s)
{
	if(!isEmpty_stack(s)) {
		return s->array[s->stackTop--];
	} else {
		printf("empty stack\n");	
		return 0;
	}
}




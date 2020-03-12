#ifndef LIB_H
#define LIB_H

#include <malloc.h>
#include <stddef.h>

typedef struct stack
{
	int *data;
	size_t length;
	size_t capacity;
}
Stack;

Stack *new_stack(void);
void stack_push(Stack *v, int elem);
void stack_pop(Stack *v);
int stack_top(Stack *v);

#endif
#ifndef STACK_H
#define STACK_H

#include <malloc.h>
#include <stddef.h>

#include "lexer.h"

typedef struct stack
{
	void **data;
	size_t length;
	size_t capacity;
}
Stack;

Stack *new_stack  ();
void   stack_push (Stack *v, void *elem);
void  *stack_pop  (Stack *v);
void  *stack_top  (Stack *v);

#endif
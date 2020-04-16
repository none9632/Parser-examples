#include "../include/stack.h"

Stack *new_stack()
{
	Stack *v = malloc(sizeof(Stack));
	v->capacity = 16;
	v->length = 0;
	v->data = malloc(sizeof(int) * v->capacity);
	return v;
}

void stack_push(Stack *v, int elem)
{
	if (v->length + 1 >= v->capacity)
	{
		v->capacity *= 2;
		v->data = realloc(v->data, sizeof(int) * v->capacity);
	}
	v->data[v->length++] = elem;
}

// removes and returns a top value
int stack_pop(Stack *v)
{
	v->length--;
	return v->data[v->length];
}

// returns a top value
int stack_top(Stack *v)
{
	return v->data[v->length - 1];
}

#include "../include/stack.h"

Stack *new_stack()
{
	Stack *s = malloc(sizeof(Stack));

	if (s == NULL)
		error("memory allocation error in new_stack()");

	s->capacity = 16;
	s->length   = 0;
	s->data     = malloc(sizeof(int) * s->capacity);

	if (s->data == NULL)
		error("memory allocation error in new_stack()");

	return s;
}

void stack_push(Stack *s, void *elem)
{
	if (s->length + 1 >= s->capacity)
	{
		s->capacity *= 2;
		s->data      = realloc(s->data, sizeof(int) * s->capacity);

		if (s->data == NULL)
			error("memory allocation error in stack_push()");
	}
	s->data[s->length++] = elem;
}

void *stack_pop(Stack *s)
{
	return s->data[--s->length];
}

void *stack_top(Stack *s)
{
	return s->data[s->length - 1];
}

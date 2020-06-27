#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "error.h"

typedef struct node
{
	int kind;
	int value;
	struct node *n[3];
}
		Node;

Node *new_node         (int kind);
void  start_print_node (Node *n);

#endif
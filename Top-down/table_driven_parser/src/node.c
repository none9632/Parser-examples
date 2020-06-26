#include "../include/node.h"

char *prefix = NULL;

Node* new_node(int kind)
{
	Node *n = malloc(sizeof(Node));

	if (n == NULL)
		error("memory allocation error in new_node()");

	n->kind  = kind;
	n->value = 0;

	return n;
}

static void print_kind(int kind, int value)
{
	switch (kind)
	{
		case NUM:        printf("%i\n", value); break;
		case PLUS:       printf("+\n");         break;
		case ASTERISK:   printf("*\n");         break;
		case LP:         printf("(\n");         break;
		case RP:         printf(")\n");         break;
		case EXPR:       printf("E\n");         break;
		case EXPR_PRIME: printf("E'\n");        break;
		case TERM:       printf("T\n");         break;
		case TERM_PRIME: printf("T'\n");        break;
		case FACT:       printf("F\n");         break;
		case EPSILON:    printf("epsilon\n");   break;
	}
}

/*
 * The '│' symbol cannot be saved to the pointer so I save '|' and output '│'.
 */
static void print_prefix(int prefix_len)
{
	for (int i = 0; i < prefix_len; i++)
	{
		if (prefix[i] == '|')
			printf("│");
		else
			printf("%c", prefix[i]);
	}
}

/*
 * Adds '|   ' or '    ' to the prefix.
 */
static void new_prefix(int prefix_len, int is_left)
{
	prefix = realloc(prefix, prefix_len + 4);

	if (prefix == NULL)
		error("memory allocation error in new_prefix()");

	for (int i = prefix_len; i < prefix_len + 4; ++i)
		prefix[i] = ' ';

	if (is_left)
		prefix[prefix_len] = '|';
}

void print_node(Node *n, int prefix_len, int is_left)
{
	if (n != NULL)
	{
		print_prefix(prefix_len);
		printf("%s", (is_left ? "├── " : "└── "));
		print_kind(n->kind, n->value);

		new_prefix(prefix_len, is_left);
		prefix_len += 4;

		print_node(n->n[2], prefix_len, 1);
		print_node(n->n[1], prefix_len, 1);
		print_node(n->n[0], prefix_len, 0);
	}
}

void start_print_node(Node *n)
{
	prefix = NULL;

	print_kind(n->kind, n->value);

	print_node(n->n[2], 0, 1);
	print_node(n->n[1], 0, 1);
	print_node(n->n[0], 0, 0);
}

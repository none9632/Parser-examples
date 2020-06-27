#include "../include/node.h"

char *prefix;

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
		case NUM:      printf("%i\n", value); break;
		case PLUS:     printf("+\n");         break;
		case MINUS:    printf("-\n");         break;
		case ASTERISK: printf("*\n");         break;
		case SLASH:    printf("/\n");         break;
		case LP:       printf("(\n");         break;
		case RP:       printf(")\n");         break;
		case EXPR:     printf("E\n");         break;
		case TERM:     printf("T\n");         break;
		case FACT:     printf("F\n");         break;
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

void print_node(Node *node, int prefix_len, int is_left)
{
	if (node != NULL)
	{
		print_prefix(prefix_len);
		printf("%s", (is_left ? "├── " : "└── "));
		print_kind(node->kind, node->value);

		new_prefix(prefix_len, is_left);
		prefix_len += 4;

		print_node(node->n[2], prefix_len, 1);
		print_node(node->n[1], prefix_len, 1);
		print_node(node->n[0], prefix_len, 0);
	}
}

void start_print_node(Node *tree)
{
	prefix = NULL;

	print_kind(tree->kind, tree->value);

	print_node(tree->n[2], 0, 1);
	print_node(tree->n[1], 0, 1);
	print_node(tree->n[0], 0, 0);
}

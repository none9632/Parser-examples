#include "../include/parser.h"

#define EMPTY -1

#define COLUMNS_PARSE_TABLE 8
#define LINE_PARSE_TABLE 5
#define COLUMNS_PRODUCTION_TABLE 3
#define LINE_PRODUCTION_TABLE 9

typedef struct element_parse_stack
{
	int state;
	Node *node;
}
Elem_PS;

static Token  token;
static Stack *parse_stack;

/*
 * Parse table
 * |-------------------------------------------------------------------------------------|
 * |     |   NUM   |    +    |    -    |    *    |    /    |    (    |    )    |    $    |
 * |-------------------------------------------------------------------------------------|
 * |  E  | E->TE'  |         |         |         |         | E->TE'  |         |         |
 * |-------------------------------------------------------------------------------------|
 * |  E' |         | E'->+TE'| E'->-TE'|         |         |         | E'->e   | E'->e   |
 * |-------------------------------------------------------------------------------------|
 * |  T  | T->FT'  |         |         |         |         | T->FT'  |         |         |
 * |-------------------------------------------------------------------------------------|
 * |  T' |         | T'->e   | T'->e   | T'->*FT'| T'->/FT'|         | T'->e   | T'->e   |
 * |-------------------------------------------------------------------------------------|
 * |  F  | F->NUM  |         |         |         |         | F->(E)  |         |         |
 * |-------------------------------------------------------------------------------------|
 */
static int parse_table[LINE_PARSE_TABLE][COLUMNS_PARSE_TABLE] =
{
		{ 0,     EMPTY, EMPTY, EMPTY, EMPTY, 0,     EMPTY, EMPTY },
		{ EMPTY, 1,     2,     EMPTY, EMPTY, EMPTY, 8,     8     },
		{ 3,     EMPTY, EMPTY, EMPTY, EMPTY, 3,     EMPTY, EMPTY },
		{ EMPTY, 8,     8,     4,     5,     EMPTY, 8,     8     },
		{ 6,     EMPTY, EMPTY, EMPTY, EMPTY, 7,     EMPTY, EMPTY }
};

/*
 * Production table
 * |---------------------------------------|
 * |   | Element 1 | Element 2 | Element 3 |
 * |---------------------------------------|
 * | 0 |     E'    |     T     |           |
 * |---------------------------------------|
 * | 1 |     E'    |     T     |     +     |
 * |---------------------------------------|
 * | 2 |     E'    |     T     |     -     |
 * |---------------------------------------|
 * | 3 |     T'    |     F     |           |
 * |---------------------------------------|
 * | 4 |     T'    |     F     |     *     |
 * |---------------------------------------|
 * | 5 |     T'    |     F     |     /     |
 * |---------------------------------------|
 * | 6 |    NUM    |           |           |
 * |---------------------------------------|
 * | 7 |     )     |     E     |     (     |
 * |---------------------------------------|
 * | 8 |     e     |           |           |
 * |---------------------------------------|
 */
static int production_table[LINE_PRODUCTION_TABLE][COLUMNS_PRODUCTION_TABLE] =
{
		{ EXPR_PRIME, TERM,  EMPTY    },
		{ EXPR_PRIME, TERM,  PLUS     },
		{ EXPR_PRIME, TERM,  MINUS    },
		{ TERM_PRIME, FACT,  EMPTY    },
		{ TERM_PRIME, FACT,  ASTERISK },
		{ TERM_PRIME, FACT,  SLASH    },
		{ NUM,        EMPTY, EMPTY    },
		{ RP,         EXPR,  LP,      },
		{ EPSILON,    EMPTY, EMPTY    }
};

static Elem_PS *new_elem_PS(int state, Node *node)
{
	Elem_PS *elem_PS = malloc(sizeof(Elem_PS));

	if (elem_PS == NULL)
		error("memory allocation error in new_elem_PS()");

	elem_PS->state = state;
	elem_PS->node  = node;

	return elem_PS;
}

// Getting index from non terminal
static int nonterminal_to_index(int nonterminal)
{
	return nonterminal - EXPR;
}

static void predict(Elem_PS *top_elem)
{
	// getting index from parser_table
	int index = parse_table[nonterminal_to_index(top_elem->state)][token.type];

	if (index == EMPTY)
		error("syntax error");

	stack_pop(parse_stack);

	for (int i = 0; i < COLUMNS_PRODUCTION_TABLE; ++i)
	{
		int elem = production_table[index][i];

		if (elem != EMPTY)
		{
			top_elem->node->n[i] = new_node(elem);
			if (elem != EPSILON)
				stack_push(parse_stack, new_elem_PS(elem, top_elem->node->n[i]));
		}
	}
}

static void match(Elem_PS *top_elem)
{
	if (top_elem->state == NUM)
		top_elem->node->value = token.value;

	stack_pop(parse_stack);
	token = get_next_token();
}

Node *LL_parser()
{
	Node *tree;

	tree        = new_node(EXPR);
	token       = get_next_token();
	parse_stack = new_stack();

	stack_push(parse_stack, new_elem_PS(EXPR, tree));

	while (parse_stack->length > 0)
	{
		Elem_PS *top_elem = stack_top(parse_stack);

		if (top_elem->state >= EXPR)
			predict(top_elem);
		else if (top_elem->state == token.type)
			match(top_elem);
		else
			error("syntax error");
	}

	return tree;
}

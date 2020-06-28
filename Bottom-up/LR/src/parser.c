#include "../include/parser.h"

#define EMPTY -1
#define EMPTY_ELEM EMPTY, EMPTY

#define COLUMNS_PARSE_TABLE 11
#define LINE_PARSE_TABLE 30
#define COLUMNS_PRODUCTION_TABLE 4
#define LINE_PRODUCTION_TABLE 8

// Actions
enum
{
	SHIFT,
	REDUCE,
	GOTO,
	ACCEPT
};

typedef struct elem_parse_table
{
	int action;
	int value;
}
Elem_PT;

typedef struct elem_parse_stack
{
	int state;
	Node *node;
}
Elem_PS;

static Token  token;
static Stack *parse_stack;

/*
 * Parse table
 * |---------------------------------------------------------------------------------------|
 * |-------|                             Action                             |     Goto     |
 * |---------------------------------------------------------------------------------------|
 * | State |  NUM  |   +   |   -   |   *   |   /   |   (   |   )   |   $    | E  | T  | F  |
 * |---------------------------------------------------------------------------------------|
 * |   0   | S(4)  |       |       |       |       | S(5)  |       |        | 1  | 2  | 3  |
 * |---------------------------------------------------------------------------------------|
 * |   1   |       | S(6)  | S(7)  |       |       |       |       | accept |    |    |    |
 * |---------------------------------------------------------------------------------------|
 * |   2   |       | R(2)  | R(2)  | S(8)  | S(9)  |       |       | R(2)   |    |    |    |
 * |---------------------------------------------------------------------------------------|
 * |   3   |       | R(5)  | R(5)  | R(5)  | R(5)  |       |       | R(5)   |    |    |    |
 * |---------------------------------------------------------------------------------------|
 * |   4   |       | R(6)  | R(6)  | R(6)  | R(6)  |       |       | R(6)   |    |    |    |
 * |---------------------------------------------------------------------------------------|
 * |   5   | S(18) |       |       |       |       | S(19) |       |        | 10 | 21 | 20 |
 * |---------------------------------------------------------------------------------------|
 * |   6   | S(4)  |       |       |       |       | S(5)  |       |        |    | 11 | 3  |
 * |---------------------------------------------------------------------------------------|
 * |   7   | S(4)  |       |       |       |       | S(5)  |       |        |    | 12 | 3  |
 * |---------------------------------------------------------------------------------------|
 * |   8   | S(4)  |       |       |       |       | S(5)  |       |        |    |    | 13 |
 * |---------------------------------------------------------------------------------------|
 * |   9   | S(4)  |       |       |       |       | S(5)  |       |        |    |    | 14 |
 * |---------------------------------------------------------------------------------------|
 * |  10   |       | S(16) | S(17) |       |       |       | S(15) |        |    |    |    |
 * |---------------------------------------------------------------------------------------|
 * |  11   |       | R(0)  | R(0)  | S(8)  | S(9)  |       |       | R(0)   |    |    |    |
 * |---------------------------------------------------------------------------------------|
 * |  12   |       | R(1)  | R(1)  | S(8)  | S(9)  |       |       | R(1)   |    |    |    |
 * |---------------------------------------------------------------------------------------|
 * |  13   |       | R(3)  | R(3)  | R(3)  | R(3)  |       |       | R(3)   |    |    |    |
 * |---------------------------------------------------------------------------------------|
 * |  14   |       | R(4)  | R(4)  | R(4)  | R(4)  |       |       | R(4)   |    |    |    |
 * |---------------------------------------------------------------------------------------|
 * |  15   |       | R(7)  | R(7)  | R(7)  | R(7)  |       |       | R(7)   |    |    |    |
 * |---------------------------------------------------------------------------------------|
 * |  16   | S(18) |       |       |       |       | S(19) |       |        |    | 23 | 20 |
 * |---------------------------------------------------------------------------------------|
 * |  17   | S(18) |       |       |       |       | S(19) |       |        |    | 24 | 20 |
 * |---------------------------------------------------------------------------------------|
 * |  18   |       | R(6)  | R(6)  | R(6)  | R(6)  |       |       | R(6)   |    |    |    |
 * |---------------------------------------------------------------------------------------|
 * |  19   | S(18) |       |       |       |       | S(19) |       |        | 22 | 21 | 20 |
 * |---------------------------------------------------------------------------------------|
 * |  20   |       | R(5)  | R(5)  | R(5)  | R(5)  |       |       | R(5)   |    |    |    |
 * |---------------------------------------------------------------------------------------|
 * |  21   |       | R(2)  | R(2)  | S(25) | S(26) |       |       | R(2)   |    |    |    |
 * |---------------------------------------------------------------------------------------|
 * |  22   |       | S(16) | S(17) |       |       |       |       | S(29)  |    |    |    |
 * |---------------------------------------------------------------------------------------|
 * |  23   |       | R(0)  | R(0)  | S(25) | S(26) |       |       | R(0)   |    |    |    |
 * |---------------------------------------------------------------------------------------|
 * |  24   |       | R(1)  | R(1)  | S(25) | S(26) |       |       | R(1)   |    |    |    |
 * |---------------------------------------------------------------------------------------|
 * |  25   | S(18) |       |       |       |       | S(19) |       |        |    |    | 27 |
 * |---------------------------------------------------------------------------------------|
 * |  26   | S(18) |       |       |       |       | S(19) |       |        |    |    | 28 |
 * |---------------------------------------------------------------------------------------|
 * |  27   |       | R(3)  | R(3)  | R(3)  | R(3)  |       |       | R(3)   |    |    |    |
 * |---------------------------------------------------------------------------------------|
 * |  28   |       | R(4)  | R(4)  | R(4)  | R(4)  |       |       | R(4)   |    |    |    |
 * |---------------------------------------------------------------------------------------|
 * |  29   |       | R(7)  | R(7)  | R(7)  | R(7)  |       |       | R(7)   |    |    |    |
 * |---------------------------------------------------------------------------------------|
 */
static Elem_PT parse_table[LINE_PARSE_TABLE][COLUMNS_PARSE_TABLE] =
{
		{ {SHIFT, 4},   {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {SHIFT, 5},   {EMPTY_ELEM}, {EMPTY_ELEM}, {GOTO, 1},    {GOTO, 2},    {GOTO, 3}    },
		{ {EMPTY_ELEM}, {SHIFT, 6},   {SHIFT, 7},   {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {ACCEPT, 0},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM} },
		{ {EMPTY_ELEM}, {REDUCE, 2},  {REDUCE, 2},  {SHIFT, 8},   {SHIFT, 9},   {EMPTY_ELEM}, {EMPTY_ELEM}, {REDUCE, 2},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM} },
		{ {EMPTY_ELEM}, {REDUCE, 5},  {REDUCE, 5},  {REDUCE, 5},  {REDUCE, 5},  {EMPTY_ELEM}, {EMPTY_ELEM}, {REDUCE, 5},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM} },
		{ {EMPTY_ELEM}, {REDUCE, 6},  {REDUCE, 6},  {REDUCE, 6},  {REDUCE, 6},  {EMPTY_ELEM}, {EMPTY_ELEM}, {REDUCE, 6},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM} },
		{ {SHIFT, 18},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {SHIFT, 19},  {EMPTY_ELEM}, {EMPTY_ELEM}, {GOTO, 10},   {GOTO, 21},   {GOTO, 20}   },
		{ {SHIFT, 4},   {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {SHIFT, 5},   {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {GOTO, 11},   {GOTO, 3}    },
		{ {SHIFT, 4},   {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {SHIFT, 5},   {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {GOTO, 12},   {GOTO, 3}    },
		{ {SHIFT, 4},   {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {SHIFT, 5},   {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {GOTO, 13}   },
		{ {SHIFT, 4},   {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {SHIFT, 5},   {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {GOTO, 14}   },
		{ {EMPTY_ELEM}, {SHIFT, 16},  {SHIFT, 17},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {SHIFT, 15},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM} },
		{ {EMPTY_ELEM}, {REDUCE, 0},  {REDUCE, 0},  {SHIFT, 8},   {SHIFT, 9},   {EMPTY_ELEM}, {EMPTY_ELEM}, {REDUCE, 0},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM} },
		{ {EMPTY_ELEM}, {REDUCE, 1},  {REDUCE, 1},  {SHIFT, 8},   {SHIFT, 9},   {EMPTY_ELEM}, {EMPTY_ELEM}, {REDUCE, 1},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM} },
		{ {EMPTY_ELEM}, {REDUCE, 3},  {REDUCE, 3},  {REDUCE, 3},  {REDUCE, 3},  {EMPTY_ELEM}, {EMPTY_ELEM}, {REDUCE, 3},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM} },
		{ {EMPTY_ELEM}, {REDUCE, 4},  {REDUCE, 4},  {REDUCE, 4},  {REDUCE, 4},  {EMPTY_ELEM}, {EMPTY_ELEM}, {REDUCE, 4},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM} },
		{ {EMPTY_ELEM}, {REDUCE, 7},  {REDUCE, 7},  {REDUCE, 7},  {REDUCE, 7},  {EMPTY_ELEM}, {EMPTY_ELEM}, {REDUCE, 7},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM} },
		{ {SHIFT, 18},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {SHIFT, 19},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {GOTO, 23},   {GOTO, 20}   },
		{ {SHIFT, 18},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {SHIFT, 19},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {GOTO, 24},   {GOTO, 20}   },
		{ {EMPTY_ELEM}, {REDUCE, 6},  {REDUCE, 6},  {REDUCE, 6},  {REDUCE, 6},  {EMPTY_ELEM}, {REDUCE, 6},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM} },
		{ {SHIFT, 18},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {SHIFT, 19},  {EMPTY_ELEM}, {EMPTY_ELEM}, {GOTO, 22},   {GOTO, 21},   {GOTO, 20}   },
		{ {EMPTY_ELEM}, {REDUCE, 5},  {REDUCE, 5},  {REDUCE, 5},  {REDUCE, 5},  {EMPTY_ELEM}, {REDUCE, 5},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM} },
		{ {EMPTY_ELEM}, {REDUCE, 2},  {REDUCE, 2},  {SHIFT, 25},  {SHIFT, 26},  {EMPTY_ELEM}, {REDUCE, 2},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM} },
		{ {EMPTY_ELEM}, {SHIFT, 16},  {SHIFT, 17},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {SHIFT, 29},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM} },
		{ {EMPTY_ELEM}, {REDUCE, 0},  {REDUCE, 0},  {SHIFT, 25},  {SHIFT, 26},  {EMPTY_ELEM}, {REDUCE, 0},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM} },
		{ {EMPTY_ELEM}, {REDUCE, 1},  {REDUCE, 1},  {SHIFT, 25},  {SHIFT, 26},  {EMPTY_ELEM}, {REDUCE, 1},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM} },
		{ {SHIFT, 18},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {SHIFT, 19},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {GOTO, 27}   },
		{ {SHIFT, 18},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {SHIFT, 19},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {GOTO, 28}   },
		{ {EMPTY_ELEM}, {REDUCE, 3},  {REDUCE, 3},  {REDUCE, 3},  {REDUCE, 3},  {EMPTY_ELEM}, {REDUCE, 3},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM} },
		{ {EMPTY_ELEM}, {REDUCE, 4},  {REDUCE, 4},  {REDUCE, 4},  {REDUCE, 4},  {EMPTY_ELEM}, {REDUCE, 4},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM} },
		{ {EMPTY_ELEM}, {REDUCE, 7},  {REDUCE, 7},  {REDUCE, 7},  {REDUCE, 7},  {EMPTY_ELEM}, {REDUCE, 7},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM} }
};

/*
 * Production table
 * |---------------------------------------------------|
 * |   |  Heading  | Element 1 | Element 2 | Element 3 |
 * |---------------------------------------------------|
 * | 0 |     E     |     E     |     +     |     T     |       E -> E + T
 * |---------------------------------------------------|
 * | 1 |     E     |     E     |     -     |     T     |       E -> E - T
 * |---------------------------------------------------|
 * | 2 |     E     |     T     |           |           |       E -> T
 * |---------------------------------------------------|
 * | 3 |     T     |     T     |     *     |     F     |       T -> T * F
 * |---------------------------------------------------|
 * | 4 |     T     |     T     |     /     |     F     |       T -> T / F
 * |---------------------------------------------------|
 * | 5 |     T     |     F     |           |           |       T -> F
 * |---------------------------------------------------|
 * | 6 |     F     |    NUM    |           |           |       F -> NUM
 * |---------------------------------------------------|
 * | 7 |     F     |     (     |     E     |     )     |       F -> (E)
 * |---------------------------------------------------|
 */
static int production_table[LINE_PRODUCTION_TABLE][COLUMNS_PRODUCTION_TABLE] =
{
		{ EXPR, EXPR, PLUS,     TERM  },
		{ EXPR, EXPR, MINUS,    TERM  },
		{ EXPR, TERM, EMPTY,    EMPTY },
		{ TERM, TERM, ASTERISK, FACT  },
		{ TERM, TERM, SLASH,    FACT  },
		{ TERM, FACT, EMPTY,    EMPTY },
		{ FACT, NUM,  EMPTY,    EMPTY },
		{ FACT, LP,   EXPR,     RP    }
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

static void shift(int state)
{
	Elem_PS *elem_PS = new_elem_PS(state, new_node(token.type));

	if (token.type == NUM)
		elem_PS->node->value = token.value;

	stack_push(parse_stack, elem_PS);

	token = get_next_token();
}

static void reduce(int value)
{
	Node *node = new_node(production_table[value][0]);

	for (int i = 1; i < COLUMNS_PRODUCTION_TABLE; ++i)
	{
		if (production_table[value][i] != EMPTY)
		{
			Elem_PS *elem_PS = stack_pop(parse_stack);
			node->n[i - 1] = elem_PS->node;
		}
	}

	Elem_PS *elem_PS = stack_top(parse_stack);
	int      heading = production_table[value][0];
	int      state   = parse_table[elem_PS->state][heading].value;

	stack_push(parse_stack, new_elem_PS(state, node));
}

Node *LR_parser()
{
	token       = get_next_token();
	parse_stack = new_stack();

	stack_push(parse_stack, new_elem_PS(0, NULL));

	while (1)
	{
		Elem_PS *elem_PS = stack_top(parse_stack);
		Elem_PT  elem_PT = parse_table[elem_PS->state][token.type];

		if (elem_PT.action == SHIFT)
			shift(elem_PT.value);
		else if (elem_PT.action == REDUCE)
			reduce(elem_PT.value);
		else if (elem_PT.action == ACCEPT)
			break;
		else
			error("syntax error");
	}

	Elem_PS *elem_PS = stack_top(parse_stack);

	return elem_PS->node;
}

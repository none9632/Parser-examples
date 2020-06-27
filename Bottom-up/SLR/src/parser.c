#include "../include/parser.h"

#define EMPTY -1
#define EMPTY_ELEM EMPTY, EMPTY

#define COLUMNS_PARSE_TABLE 11
#define LINE_PARSE_TABLE 16
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
	int number;
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
 * |---------------------------------------------------------------------------------|
 * |-------|                          Action                          |     Goto     |
 * |---------------------------------------------------------------------------------|
 * | State | NUM  |  +   |  -   |  *   |  /   |  (   |   )   |   $    | E  | T  | F  |
 * |---------------------------------------------------------------------------------|
 * |   0   | S(4) |      |      |      |      | S(3) |       |        | 1  | 2  | 5  |
 * |---------------------------------------------------------------------------------|
 * |   1   |      | S(6) | S(7) |      |      |      |       | accept |    |    |    |
 * |---------------------------------------------------------------------------------|
 * |   2   |      | R(2) | R(2) | S(8) | S(9) |      | R(2)  |  R(2)  |    |    |    |
 * |---------------------------------------------------------------------------------|
 * |   3   | S(4) |      |      |      |      | S(3) |       |        | 10 | 2  | 5  |
 * |---------------------------------------------------------------------------------|
 * |   4   |      | R(6) | R(6) | R(6) | R(6) |      | R(6)  |  R(6)  |    |    |    |
 * |---------------------------------------------------------------------------------|
 * |   5   |      | R(5) | R(5) | R(5) | R(5) |      | R(5)  |  R(5)  |    |    |    |
 * |---------------------------------------------------------------------------------|
 * |   6   | S(4) |      |      |      |      | S(3) |       |        |    | 11 | 5  |
 * |---------------------------------------------------------------------------------|
 * |   7   | S(4) |      |      |      |      | S(3) |       |        |    | 12 | 5  |
 * |---------------------------------------------------------------------------------|
 * |   8   | S(4) |      |      |      |      | S(3) |       |        |    |    | 13 |
 * |---------------------------------------------------------------------------------|
 * |   9   | S(4) |      |      |      |      | S(3) |       |        |    |    | 14 |
 * |---------------------------------------------------------------------------------|
 * |  10   |      | S(6) | S(7) |      |      |      | S(15) |        |    |    |    |
 * |---------------------------------------------------------------------------------|
 * |  11   |      | R(0) | R(0) | S(8) | S(9) |      | R(0)  |  R(0)  |    |    |    |
 * |---------------------------------------------------------------------------------|
 * |  12   |      | R(1) | R(1) | S(8) | S(9) |      | R(1)  |  R(1)  |    |    |    |
 * |---------------------------------------------------------------------------------|
 * |  13   |      | R(3) | R(3) | R(3) | R(3) |      | R(3)  |  R(3)  |    |    |    |
 * |---------------------------------------------------------------------------------|
 * |  14   |      | R(4) | R(4) | R(4) | R(4) |      | R(4)  |  R(4)  |    |    |    |
 * |---------------------------------------------------------------------------------|
 * |  15   |      | R(7) | R(7) | R(7) | R(7) |      | R(7)  |  R(7)  |    |    |    |
 * |---------------------------------------------------------------------------------|
 */
static Elem_PT parse_table[LINE_PARSE_TABLE][COLUMNS_PARSE_TABLE] =
{
		{ {SHIFT, 4},   {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {SHIFT, 3},   {EMPTY_ELEM}, {EMPTY_ELEM}, {GOTO, 1},    {GOTO, 2},    {GOTO, 5}    },
		{ {EMPTY_ELEM}, {SHIFT, 6},   {SHIFT, 7},   {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {ACCEPT, 0},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM} },
		{ {EMPTY_ELEM}, {REDUCE, 2},  {REDUCE, 2},  {SHIFT, 8},   {SHIFT, 9},   {EMPTY_ELEM}, {REDUCE, 2},  {REDUCE, 2},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM} },
		{ {SHIFT, 4},   {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {SHIFT, 3},   {EMPTY_ELEM}, {EMPTY_ELEM}, {GOTO, 10},   {GOTO, 2},    {GOTO, 5}    },
		{ {EMPTY_ELEM}, {REDUCE, 6},  {REDUCE, 6},  {REDUCE, 6},  {REDUCE, 6},  {EMPTY_ELEM}, {REDUCE, 6},  {REDUCE, 6},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM} },
		{ {EMPTY_ELEM}, {REDUCE, 5},  {REDUCE, 5},  {REDUCE, 5},  {REDUCE, 5},  {EMPTY_ELEM}, {REDUCE, 5},  {REDUCE, 5},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM} },
		{ {SHIFT, 4},   {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {SHIFT, 3},   {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {GOTO, 11},   {GOTO, 5}    },
		{ {SHIFT, 4},   {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {SHIFT, 3},   {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {GOTO, 12},   {GOTO, 5}    },
		{ {SHIFT, 4},   {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {SHIFT, 3},   {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {GOTO, 13}   },
		{ {SHIFT, 4},   {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {SHIFT, 3},   {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {GOTO, 14}   },
		{ {EMPTY_ELEM}, {SHIFT, 6},   {SHIFT, 7},   {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {SHIFT, 15},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM} },
		{ {EMPTY_ELEM}, {REDUCE, 0},  {REDUCE, 0},  {SHIFT, 8},   {SHIFT, 9},   {EMPTY_ELEM}, {REDUCE, 0},  {REDUCE, 0},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM} },
		{ {EMPTY_ELEM}, {REDUCE, 1},  {REDUCE, 1},  {SHIFT, 8},   {SHIFT, 9},   {EMPTY_ELEM}, {REDUCE, 1},  {REDUCE, 1},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM} },
		{ {EMPTY_ELEM}, {REDUCE, 3},  {REDUCE, 3},  {REDUCE, 3},  {REDUCE, 3},  {EMPTY_ELEM}, {REDUCE, 3},  {REDUCE, 3},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM} },
		{ {EMPTY_ELEM}, {REDUCE, 4},  {REDUCE, 4},  {REDUCE, 4},  {REDUCE, 4},  {EMPTY_ELEM}, {REDUCE, 4},  {REDUCE, 4},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM} },
		{ {EMPTY_ELEM}, {REDUCE, 7},  {REDUCE, 7},  {REDUCE, 7},  {REDUCE, 7},  {EMPTY_ELEM}, {REDUCE, 7},  {REDUCE, 7},  {EMPTY_ELEM}, {EMPTY_ELEM}, {EMPTY_ELEM} }
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

static void reduce(int number)
{
	Node *node = new_node(production_table[number][0]);

	for (int i = 1; i < COLUMNS_PRODUCTION_TABLE; ++i)
	{
		if (production_table[number][i] != EMPTY)
		{
			Elem_PS *elem_PS = stack_pop(parse_stack);
			node->n[i - 1] = elem_PS->node;
		}
	}

	Elem_PS *elem_PS = stack_top(parse_stack);
	int      heading = production_table[number][0];
	int      state   = parse_table[elem_PS->state][heading].number;

	stack_push(parse_stack, new_elem_PS(state, node));
}

Node *SLR_parser()
{
	token       = get_next_token();
	parse_stack = new_stack();

	stack_push(parse_stack, new_elem_PS(0, NULL));  // push state 0 into the stack

	while (1)
	{
		Elem_PS *elem_PS = stack_top(parse_stack);
		Elem_PT  elem_PT = parse_table[elem_PS->state][token.type];

		if (elem_PT.action == SHIFT)
			shift(elem_PT.number);
		else if (elem_PT.action == REDUCE)
			reduce(elem_PT.number);
		else if (elem_PT.action == ACCEPT)
			break;
		else
			error("syntax error");
	}

	Elem_PS *elem_PS = stack_top(parse_stack);

	return elem_PS->node;
}

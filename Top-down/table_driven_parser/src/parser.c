#include "../include/parser.h"

#define EMPTY -1

#define COLUMNS_PARSE_TABLE 6
#define LINE_PARSE_TABLE 5
#define COLUMNS_PRODUCTION_TABLE 4
#define LINE_PRODUCTION_TABLE 8

// Non-terminals
enum
{
	EXPR = 10,
	EXPR_PRIME,
	TERM,
	TERM_PRIME,
	FACT
};

// Actions
enum
{
	PLUS_ATC = 20,
	MULT_ACT,
};

static Token token;
static Stack *parse_stack;
static Stack *value_stack;

/*
 * Parse table
 * |-----------------------------------------------------------------|
 * |     |   NUM   |    +    |    *    |    (    |    )    |    $    |
 * |-----------------------------------------------------------------|
 * |  E  | E->TE'  |         |         | E->TE'  |         |         |
 * |-----------------------------------------------------------------|
 * |  E' |         | E'->+TE'|         |         | E'->e   | E'->e   |
 * |-----------------------------------------------------------------|
 * |  T  | T->FT'  |         |         | T->FT'  |         |         |
 * |-----------------------------------------------------------------|
 * |  T' |         | T'->e   | T'->*FT'|         | T'->e   | T'->e   |
 * |-----------------------------------------------------------------|
 * |  F  | F->NUM  |         |         | F->(E)  |         |         |
 * |-----------------------------------------------------------------|
 */
static int parse_table[LINE_PARSE_TABLE][COLUMNS_PARSE_TABLE] =
{
		{ 1,     EMPTY, EMPTY, 1,     EMPTY, EMPTY },
		{ EMPTY, 2,     EMPTY, EMPTY, 7,     7     },
		{ 3,     EMPTY, EMPTY, 3,     EMPTY, EMPTY },
		{ EMPTY, 7,     4,     EMPTY, 7,     7     },
		{ 5,     EMPTY, EMPTY, 6,     EMPTY, EMPTY },
};

/*
 * Production table
 * |---------------------------------------------------|
 * |   | Element 1 | Element 2 | Element 3 | Element 4 |
 * |---------------------------------------------------|
 * | 0 |     E     |           |           |           |
 * |---------------------------------------------------|
 * | 1 |     E'    |     T     |           |           |
 * |---------------------------------------------------|
 * | 2 | PLUS_ACT  |     E'    |     T     |     +     |
 * |---------------------------------------------------|
 * | 3 |     T'    |     F     |           |           |
 * |---------------------------------------------------|
 * | 4 | MULT_ACT  |     T'    |     F     |     *     |
 * |---------------------------------------------------|
 * | 5 |    NUM    |           |           |           |
 * |---------------------------------------------------|
 * | 6 |     )     |     E     |     (     |           |
 * |---------------------------------------------------|
 * | 7 |           |           |           |           |
 * |---------------------------------------------------|
 */
static int production_table[LINE_PRODUCTION_TABLE][COLUMNS_PRODUCTION_TABLE] =
{
		{ EXPR,       EMPTY,      EMPTY, EMPTY },
		{ EXPR_PRIME, TERM,       EMPTY, EMPTY },
		{ PLUS_ATC,   EXPR_PRIME, TERM,  PLUS  },
		{ TERM_PRIME, FACT,       EMPTY, EMPTY },
		{ MULT_ACT,   TERM_PRIME, FACT,  MULT  },
		{ NUM,        EMPTY,      EMPTY, EMPTY },
		{ RP,         EXPR,       LP,    EMPTY },
		{ EMPTY,      EMPTY,      EMPTY, EMPTY }
};

// Getting index from non terminal
static int nonterminal_to_index(int nonterminal)
{
	return nonterminal - EXPR;
}

int LL_parser()
{
	parse_stack = new_stack();
	value_stack = new_stack();
	stack_push(parse_stack, EXPR);

	token = get_next_token();

	while (parse_stack->length > 0)
	{
		int top_elem = stack_top(parse_stack);

		// Perform action
		if (top_elem >= PLUS_ATC)
		{
			int value1 = stack_pop(value_stack);
			int value2 = stack_pop(value_stack);

			switch (top_elem)
			{
				case PLUS_ATC:
					value1 = value1 + value2;
					break;
				case MULT_ACT:
					value1 = value1 * value2;
					break;
				default:
					error();
					break;
			}

			stack_push(value_stack, value1);
			stack_pop(parse_stack);
		}
		// Predict
		else if (top_elem >= EXPR)
		{
			// getting index from parser_table
			int index = parse_table[nonterminal_to_index(top_elem)][token.type];

			if (index == EMPTY)
				error();

			stack_pop(parse_stack);

			for (int i = 0; i < COLUMNS_PRODUCTION_TABLE; i++)
			{
				int elem = production_table[index][i];
				if (elem != EMPTY)
					stack_push(parse_stack, elem);
			}
		}
		else if (top_elem == token.type)
		{
			if (top_elem == NUM)
				stack_push(value_stack, token.value);

			stack_pop(parse_stack);
			token = get_next_token();
		}
		else
		{
			error();
		}
	}

	return stack_top(value_stack);
}

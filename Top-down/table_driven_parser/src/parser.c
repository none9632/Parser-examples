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
static int parse_table[LINE_PARSE_TABLE][COLUMNS_PARSE_TABLE];
static int production_table[LINE_PRODUCTION_TABLE][COLUMNS_PRODUCTION_TABLE];
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
static void init_parser_table()
{
	for (int i = 0; i < LINE_PARSE_TABLE; i++)
	{
		switch (i)
		{
			case 0:
				parse_table[i][NUM] = 1;
				parse_table[i][PLUS] = EMPTY;
				parse_table[i][STAR] = EMPTY;
				parse_table[i][LP] = 1;
				parse_table[i][RP] = EMPTY;
				parse_table[i][EOI] = EMPTY;
				break;
			case 1:
				parse_table[i][NUM] = EMPTY;
				parse_table[i][PLUS] = 2;
				parse_table[i][STAR] = EMPTY;
				parse_table[i][LP] = EMPTY;
				parse_table[i][RP] = 7;
				parse_table[i][EOI] = 7;
				break;
			case 2:
				parse_table[i][NUM] = 3;
				parse_table[i][PLUS] = EMPTY;
				parse_table[i][STAR] = EMPTY;
				parse_table[i][LP] = 3;
				parse_table[i][RP] = EMPTY;
				parse_table[i][EOI] = 7;
				break;
			case 3:
				parse_table[i][NUM] = EMPTY;
				parse_table[i][PLUS] = 7;
				parse_table[i][STAR] = 4;
				parse_table[i][LP] = EMPTY;
				parse_table[i][RP] = 7;
				parse_table[i][EOI] = 7;
				break;
			case 4:
				parse_table[i][NUM] = 5;
				parse_table[i][PLUS] = EMPTY;
				parse_table[i][STAR] = EMPTY;
				parse_table[i][LP] = 6;
				parse_table[i][RP] = EMPTY;
				parse_table[i][EOI] = 7;
				break;
		}
	}
}

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
static void init_production_table()
{
	for (int i = 0; i < LINE_PRODUCTION_TABLE; i++)
	{
		switch (i)
		{
			case 0:
				production_table[i][0] = EXPR;
				production_table[i][1] = EMPTY;
				production_table[i][2] = EMPTY;
				production_table[i][3] = EMPTY;
				break;
			case 1:
				production_table[i][0] = EXPR_PRIME;
				production_table[i][1] = TERM;
				production_table[i][2] = EMPTY;
				production_table[i][3] = EMPTY;
				break;
			case 2:
				production_table[i][0] = PLUS_ATC;
				production_table[i][1] = EXPR_PRIME;
				production_table[i][2] = TERM;
				production_table[i][3] = PLUS;
				break;
			case 3:
				production_table[i][0] = TERM_PRIME;
				production_table[i][1] = FACT;
				production_table[i][2] = EMPTY;
				production_table[i][3] = EMPTY;
				break;
			case 4:
				production_table[i][0] = MULT_ACT;
				production_table[i][1] = TERM_PRIME;
				production_table[i][2] = FACT;
				production_table[i][3] = STAR;
				break;
			case 5:
				production_table[i][0] = NUM;
				production_table[i][1] = EMPTY;
				production_table[i][2] = EMPTY;
				production_table[i][3] = EMPTY;
				break;
			case 6:
				production_table[i][0] = RP;
				production_table[i][1] = EXPR;
				production_table[i][2] = LP;
				production_table[i][3] = EMPTY;
				break;
			case 7:
				production_table[i][0] = EMPTY;
				production_table[i][1] = EMPTY;
				production_table[i][2] = EMPTY;
				production_table[i][3] = EMPTY;
				break;
		}
	}
}

// Getting index from non terminal
static int nonterminal_to_index(int nonterminal)
{
	return nonterminal - EXPR;
}

int LL_parser()
{
	init_parser_table();
	init_production_table();

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
		// Match
		else
		{
			if (top_elem == token.type)
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
	}

	return stack_top(value_stack);
}

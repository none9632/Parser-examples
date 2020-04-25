#include "../include/parser.h"

#define EMPTY -1
#define EMPTY_ELEM EMPTY, EMPTY

#define _2_21_ 2
#define _3_20_ 3
#define _4_18_ 4
#define _5_19_ 5
#define _6_16_ 6
#define _7_17_ 7
#define _8_25_ 8
#define _9_26_ 9
#define _10_22_ 10
#define _11_23_ 11
#define _12_24_ 12
#define _13_27_ 13
#define _14_28_ 14
#define _15_29_ 15

#define COLUMNS_PARSE_TABLE 11
#define LINE_PARSE_TABLE 16
#define COLUMNS_PRODUCTION_TABLE 4
#define LINE_PRODUCTION_TABLE 8

// Non-terminals
enum
{
	EXPR = 8,
	TERM,
	FACT
};

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
Elem;

static Token token;
static Stack *parse_stack;
static Stack *value_stack;

/*
 * Parse table
 * |---------------------------------------------------------------------------------------------------------------|
 * |-------|                                     Action                                    |         Goto          |
 * |---------------------------------------------------------------------------------------------------------------|
 * | State |   NUM   |    +    |    -    |    *    |    /    |    (    |    )     |   $    |   E   |   T   |   F   |
 * |---------------------------------------------------------------------------------------------------------------|
 * |   0   | S(4_18) |         |         |         |         | S(5_19) |          |        |   1   | 2_21  | 3_20  |
 * |---------------------------------------------------------------------------------------------------------------|
 * |   1   |         | S(6_16) | S(7_17) |         |         |         |          | accept |       |       |       |
 * |---------------------------------------------------------------------------------------------------------------|
 * | 2_21  |         |  R(2)   |  R(2)   | S(8_25) | S(9_26) |         |   R(2)   |  R(2)  |       |       |       |
 * |---------------------------------------------------------------------------------------------------------------|
 * | 3_20  |         |  R(5)   |  R(5)   |   R(5)  |  R(5)   |         |   R(5)   |  R(5)  |       |       |       |
 * |---------------------------------------------------------------------------------------------------------------|
 * | 4_18  |         |  R(6)   |  R(6)   |   R(6)  |  R(6)   |         |   R(6)   |  R(6)  |       |       |       |
 * |---------------------------------------------------------------------------------------------------------------|
 * | 5_19  | S(4_18) |         |         |         |         | S(5_19) |          |        | 10_22 | 2_21  | 3_20  |
 * |---------------------------------------------------------------------------------------------------------------|
 * | 6_16  | S(4_18) |         |         |         |         | S(5_19) |          |        |       | 11_23 | 3_20  |
 * |---------------------------------------------------------------------------------------------------------------|
 * | 7_17  | S(4_18) |         |         |         |         | S(5_19) |          |        |       | 12_24 | 3_20  |
 * |---------------------------------------------------------------------------------------------------------------|
 * | 8_25  | S(4_18) |         |         |         |         | S(5_19) |          |        |       |       | 13_27 |
 * |---------------------------------------------------------------------------------------------------------------|
 * | 9|26  | S(4_18) |         |         |         |         | S(5_19) |          |        |       |       | 14_28 |
 * |---------------------------------------------------------------------------------------------------------------|
 * | 10_22 |         | S(6_16) | S(7_17) |         |         |         | S(15_29) |        |       |       |       |
 * |---------------------------------------------------------------------------------------------------------------|
 * | 11_23 |         |  R(0)   |  R(0)   | S(8_25) | S(9_26) |         |   R(0)   |  R(0)  |       |       |       |
 * |---------------------------------------------------------------------------------------------------------------|
 * | 12_24 |         |  R(1)   |  R(1)   | S(8_25) | S(9_26) |         |   R(1)   |  R(1)  |       |       |       |
 * |---------------------------------------------------------------------------------------------------------------|
 * | 13_27 |         |  R(3)   |  R(3)   |   R(3)  |  R(3)   |         |   R(3)   |  R(3)  |       |       |       |
 * |---------------------------------------------------------------------------------------------------------------|
 * | 14_28 |         |  R(4)   |  R(4)   |   R(4)  |  R(4)   |         |   R(4)   |  R(4)  |       |       |       |
 * |---------------------------------------------------------------------------------------------------------------|
 * | 15_29 |         |  R(7)   |  R(7)   |   R(7)  |  R(7)   |         |   R(7)   |  R(7)  |       |       |       |
 * |---------------------------------------------------------------------------------------------------------------|
 */
static Elem parse_table[LINE_PARSE_TABLE][COLUMNS_PARSE_TABLE] =
{
		{ {SHIFT, _4_18_}, {EMPTY_ELEM},    {EMPTY_ELEM},    {EMPTY_ELEM},    {EMPTY_ELEM},    {SHIFT, _5_19_}, {EMPTY_ELEM},     {EMPTY_ELEM}, {GOTO, 1},       {GOTO, _2_21_},  {GOTO, _3_20_}  },
		{ {EMPTY_ELEM},    {SHIFT, _6_16_}, {SHIFT, _7_17_}, {EMPTY_ELEM},    {EMPTY_ELEM},    {EMPTY_ELEM},    {EMPTY_ELEM},     {ACCEPT, 0},  {EMPTY_ELEM},    {EMPTY_ELEM},    {EMPTY_ELEM}    },
		{ {EMPTY_ELEM},    {REDUCE, 2},     {REDUCE, 2},     {SHIFT, _8_25_}, {SHIFT, _9_26_}, {EMPTY_ELEM},    {REDUCE, 2},      {REDUCE, 2},  {EMPTY_ELEM},    {EMPTY_ELEM},    {EMPTY_ELEM}    },
		{ {EMPTY_ELEM},    {REDUCE, 5},     {REDUCE, 5},     {REDUCE, 5},     {REDUCE, 5},     {EMPTY_ELEM},    {REDUCE, 5},      {REDUCE, 5},  {EMPTY_ELEM},    {EMPTY_ELEM},    {EMPTY_ELEM}    },
		{ {EMPTY_ELEM},    {REDUCE, 6},     {REDUCE, 6},     {REDUCE, 6},     {REDUCE, 6},     {EMPTY_ELEM},    {REDUCE, 6},      {REDUCE, 6},  {EMPTY_ELEM},    {EMPTY_ELEM},    {EMPTY_ELEM}    },
		{ {SHIFT, _4_18_}, {EMPTY_ELEM},    {EMPTY_ELEM},    {EMPTY_ELEM},    {EMPTY_ELEM},    {SHIFT, _5_19_}, {EMPTY_ELEM},     {EMPTY_ELEM}, {GOTO, _10_22_}, {GOTO, _2_21_},  {GOTO, _3_20_}  },
		{ {SHIFT, _4_18_}, {EMPTY_ELEM},    {EMPTY_ELEM},    {EMPTY_ELEM},    {EMPTY_ELEM},    {SHIFT, _5_19_}, {EMPTY_ELEM},     {EMPTY_ELEM}, {EMPTY_ELEM},    {GOTO, _11_23_}, {GOTO, _3_20_}  },
		{ {SHIFT, _4_18_}, {EMPTY_ELEM},    {EMPTY_ELEM},    {EMPTY_ELEM},    {EMPTY_ELEM},    {SHIFT, _5_19_}, {EMPTY_ELEM},     {EMPTY_ELEM}, {EMPTY_ELEM},    {GOTO, _12_24_}, {GOTO, _3_20_}  },
		{ {SHIFT, _4_18_}, {EMPTY_ELEM},    {EMPTY_ELEM},    {EMPTY_ELEM},    {EMPTY_ELEM},    {SHIFT, _5_19_}, {EMPTY_ELEM},     {EMPTY_ELEM}, {EMPTY_ELEM},    {EMPTY_ELEM},    {GOTO, _13_27_} },
		{ {SHIFT, _4_18_}, {EMPTY_ELEM},    {EMPTY_ELEM},    {EMPTY_ELEM},    {EMPTY_ELEM},    {SHIFT, _5_19_}, {EMPTY_ELEM},     {EMPTY_ELEM}, {EMPTY_ELEM},    {EMPTY_ELEM},    {GOTO, _14_28_} },
		{ {EMPTY_ELEM},    {SHIFT, _6_16_}, {SHIFT, _7_17_}, {EMPTY_ELEM},    {EMPTY_ELEM},    {EMPTY_ELEM},    {SHIFT, _15_29_}, {EMPTY_ELEM}, {EMPTY_ELEM},    {EMPTY_ELEM},    {EMPTY_ELEM}    },
		{ {EMPTY_ELEM},    {REDUCE, 0},     {REDUCE, 0},     {SHIFT, _8_25_}, {SHIFT, _9_26_}, {EMPTY_ELEM},    {REDUCE, 0},      {REDUCE, 0},  {EMPTY_ELEM},    {EMPTY_ELEM},    {EMPTY_ELEM}    },
		{ {EMPTY_ELEM},    {REDUCE, 1},     {REDUCE, 1},     {SHIFT, _8_25_}, {SHIFT, _9_26_}, {EMPTY_ELEM},    {REDUCE, 1}, 	  {REDUCE, 1},  {EMPTY_ELEM},    {EMPTY_ELEM},    {EMPTY_ELEM}    },
		{ {EMPTY_ELEM},    {REDUCE, 3},     {REDUCE, 3},     {REDUCE, 3},     {REDUCE, 3},     {EMPTY_ELEM},    {REDUCE, 3},      {REDUCE, 3},  {EMPTY_ELEM},    {EMPTY_ELEM},    {EMPTY_ELEM}    },
		{ {EMPTY_ELEM},    {REDUCE, 4},     {REDUCE, 4},     {REDUCE, 4},     {REDUCE, 4},     {EMPTY_ELEM},    {REDUCE, 4},      {REDUCE, 4},  {EMPTY_ELEM},    {EMPTY_ELEM},    {EMPTY_ELEM}    },
		{ {EMPTY_ELEM},    {REDUCE, 7},     {REDUCE, 7},     {REDUCE, 7},     {REDUCE, 7},     {EMPTY_ELEM},    {REDUCE, 7},      {REDUCE, 7},  {EMPTY_ELEM},    {EMPTY_ELEM},    {EMPTY_ELEM}    }
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
		{ EXPR, EXPR, PLUS,  TERM  },
		{ EXPR, EXPR, MINUS, TERM  },
		{ EXPR, TERM, EMPTY, EMPTY },
		{ TERM, TERM, MULT,  FACT  },
		{ TERM, TERM, DIV,   FACT  },
		{ TERM, FACT, EMPTY, EMPTY },
		{ FACT, NUM,  EMPTY, EMPTY },
		{ FACT, LP,   EXPR,  RP    }
};

int LALR_parser()
{
	token = get_next_token();
	value_stack = new_stack();
	parse_stack = new_stack();

	stack_push(parse_stack, 0);  // push state 0 into the stack

	while (1)
	{
		int state = stack_top(parse_stack);
		Elem *elem = &parse_table[state][token.type];

		if (elem->action == SHIFT)
		{
			stack_push(parse_stack, elem->number);

			if (token.type == NUM)
				stack_push(value_stack, token.value);

			token = get_next_token();
		}
		else if (elem->action == REDUCE)
		{
			for (int i = 1; i < COLUMNS_PRODUCTION_TABLE; i++)
			{
				if (production_table[elem->number][i] != EMPTY)
					stack_pop(parse_stack);
			}

			if (elem->number == 0 ||
			    elem->number == 1 ||
			    elem->number == 3 ||
			    elem->number == 4)
			{
				int value1 = stack_pop(value_stack);
				int value2 = stack_pop(value_stack);

				switch (elem->number)
				{
					case 0: value1 = value1 + value2; break;
					case 1: value1 = value2 - value1; break;
					case 3: value1 = value1 * value2; break;
					case 4: value1 = value2 / value1; break;
				}

				stack_push(value_stack, value1);
			}

			state = stack_top(parse_stack);

			int heading = production_table[elem->number][0];
			stack_push(parse_stack, parse_table[state][heading].number);
		}
		else if (elem->action == ACCEPT)
		{
			break;
		}
		else
		{
			error();
		}
	}

	return stack_pop(value_stack);
}

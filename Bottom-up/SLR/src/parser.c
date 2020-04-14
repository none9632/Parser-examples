#include "../include/parser.h"

#define COLUMNS_PARSE_TABLE 6
#define LINE_PARSE_TABLE 5

static Token token;
static int parse_table[LINE_PARSE_TABLE][COLUMNS_PARSE_TABLE];
static Stack *parse_stack;

static void init_parse_table()
{

}

static void error()
{
	printf("ERROR\n");
	exit(EXIT_FAILURE);
}

static void match_token(int type)
{
	if (token.type != type)
		error();
	token = get_next_token();
}

int SLR_parser()
{
	token = get_next_token();
	int result = 0;

	return result;
}

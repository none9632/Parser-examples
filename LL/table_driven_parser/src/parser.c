#include "../include/parser.h"
#include "../include/lexer.h"

static Token token;

static void init_parser_table()
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

int LL_parser()
{
	init_parser_table();

	token = get_next_token();
	int result = 0;

	return result;
}

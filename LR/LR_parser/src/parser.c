#include "../include/parser.h"
#include "../include/lexer.h"

static Token token;

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

int LR_parser()
{
	token = get_next_token();
	int result = 0;

	return result;
}

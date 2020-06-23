#include "../include/parser.h"

static Token token;

static int expr();

static void match_token(int type)
{
	if (token.type != type)
		error("syntax error");
	token = get_next_token();
}

static int fact()
{
	int result = 0;

	switch (token.type)
	{
		case NUM:
			result = token.value;
			token = get_next_token();
			break;

		case LP:
			match_token(LP);
			result = expr();
			match_token(RP);
			break;

		default:
			error("syntax error");
			break;
	}

	return result;
}

static int term()
{
	int result = fact();
	int buf_result;
	int saved_char;

	while (token.type == ASTERISK || token.type == SLASH)
	{
		saved_char = token.type;
		token = get_next_token();
		buf_result = fact();

		switch (saved_char)
		{
			case ASTERISK: result *= buf_result; break;
			case SLASH:    result /= buf_result; break;
		}
	}

	return result;
}

static int expr()
{
	int result = term();
	int buf_result;
	int saved_char;

	while (token.type == PLUS || token.type == MINUS)
	{
		saved_char = token.type;
		token = get_next_token();
		buf_result = term();

		switch (saved_char)
		{
			case PLUS:  result += buf_result; break;
			case MINUS: result -= buf_result; break;
		}
	}

	return result;
}

int LL_parser()
{
	token = get_next_token();
	int result = expr();

	if (token.type != EOI)
		error("syntax error");

	return result;
}

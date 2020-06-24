#include "../include/parser.h"

static Token token;

static Node *expr();

static void match_token(int type)
{
	if (token.type != type)
		error("syntax error");
	token = get_next_token();
}

static Node *fact()
{
	Node *node = new_node();

	switch (token.type)
	{
		case NUM:
			node->kind  = K_NUM;
			node->value = token.value;
			token       = get_next_token();
			break;

		case LP:
			match_token(LP);
			node = expr();
			match_token(RP);
			break;

		default:
			error("syntax error");
			break;
	}

	return node;
}

static Node *term()
{
	Node *node = fact();
	int buf_char;

	while (token.type == ASTERISK || token.type == SLASH)
	{
		Node *buf_node = new_node();

		buf_char = token.type;
		token    = get_next_token();

		buf_node->lhs = node;
		buf_node->rhs = fact();
		node          = buf_node;

		switch (buf_char)
		{
			case ASTERISK: node->kind = K_MULT; break;
			case SLASH:    node->kind = K_DIV;  break;
		}
	}

	return node;
}

static Node *expr()
{
	Node *node = term();
	int buf_char;

	while (token.type == PLUS || token.type == MINUS)
	{
		Node *buf_node = new_node();

		buf_char = token.type;
		token    = get_next_token();

		buf_node->lhs = node;
		buf_node->rhs = term();
		node          = buf_node;

		switch (buf_char)
		{
			case PLUS:  node->kind = K_ADD; break;
			case MINUS: node->kind = K_SUB; break;
		}
	}

	return node;
}

Node *LL_parser()
{
	token = get_next_token();

	Node *node = expr();

	if (token.type != EOI)
		error("syntax error");

	return node;
}

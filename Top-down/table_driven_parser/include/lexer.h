#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "error.h"

// Terminal symbols
enum
{
	NUM,        // Number literal
	PLUS,       // '+'
	ASTERISK,   // '*'
	LP,         // '('
	RP,         // ')'
	EOI         // End of input
};

// Non-terminal symbols
enum
{
	EXPR = 10,
	EXPR_PRIME,
	TERM,
	TERM_PRIME,
	FACT,
	EPSILON
};

typedef struct token
{
	int type;
	int value;
}
Token;

Token get_next_token ();
void  init_lexer     (char *input_str);

#endif
#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

enum
{
	NUM,        // Number literal
	PLUS,       // '+'
	MINUS,      // '-'
	MULT,       // '*'
	DIV,        // '/'
	LP,         // '('
	RP,         // ')'
	EOI,        // End of input
	UNKNOWN     // Unknown literal
};

typedef struct token
{
	int type;
	int value;
}
Token;

Token get_next_token();
void init_lexer(char *input_str);
void error();

#endif
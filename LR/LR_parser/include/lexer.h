#ifndef LEXER_H
#define LEXER_H

enum
{
	NUMBER,     // Number literal
	PLUS,       // '+'
	MINUS,      // '-'
	MULT,       // '*'
	DIVISION,   // '/'
	LP,         // '('
	RP,         // ')'
	UNKNOWN,    // Unknown literal
	EOI         // End of input
};

typedef struct token
{
	int type;
	int value;
}Token;

Token get_next_token();
void init_lexer(char *input_str);

#endif
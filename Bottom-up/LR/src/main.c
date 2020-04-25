#include <stdlib.h>
#include <stdio.h>

#include "../include/lexer.h"
#include "../include/parser.h"

void usage()
{
	printf("Usage: ./SLR_parser <expression>\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	int result;

	if (argc != 2)
		usage();

	init_lexer(argv[1]);
	result = LR_parser();
	printf("%i\n", result);
}

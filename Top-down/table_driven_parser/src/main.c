#include <stdlib.h>
#include <stdio.h>

#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/node.h"

void usage()
{
	printf("Usage: ./LL_parser <expression>\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	if (argc != 2)
		usage();

	init_lexer(argv[1]);
	Node *tree = LL_parser();
	start_print_node(tree);
}

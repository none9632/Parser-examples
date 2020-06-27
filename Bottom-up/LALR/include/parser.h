#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "node.h"
#include "error.h"
#include "stack.h"

Node *LALR_parser();

#endif
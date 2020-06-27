#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "error.h"
#include "node.h"
#include "stack.h"

Node *SLR_parser();

#endif
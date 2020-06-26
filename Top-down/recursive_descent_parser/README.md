# Recursive-Descent Parser

This is an simple example of a recursive-descent parser.

## Build and run
    $ make
    $ ./LL_parser <expression>

## Testing
    $ make test

## Grammar
    E -> T { (+|-) T }
    T -> F { (*|/) F }
    F -> NUM | (E)

# Table driven parser

This is an simple example of a table driven LL(1) parser.

## Build and run
    make
    ./LL_parser <expression>
    
## Testing
    make test (or ./test.sh)

## Grammar
    stmt -> expr | (epsilon)
    expr -> term expr'
    expr' -> + term expr' | - term expr' | (epsilon)
    term -> fact term'
    term' -> * fact term' | / fact term' | (epsilon)
    fact -> NUM | ( expr )

# LL parser

This is an simple example of a recursive descent parser.

## Build and run
    make
    ./LL_parser <expression>
    
## Testing
    make test (or ./test.sh)

## Grammar
    stmt -> expr | (epsilon)
    expr -> term | term ('+'|'-') expr
    term -> fact | fact ('*'|'/') term
    fact -> NUM | ( expr )

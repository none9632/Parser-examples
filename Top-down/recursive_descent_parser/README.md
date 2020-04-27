# Recursive-Descent Parser

This is an simple example of a recursive-descent parser.

## Build and run
    make
    ./LL_parser <expression>
    
## Testing
    make test (or ./test.sh)

## Grammar
    E -> TE'
    E' -> +TE' | -TE' | (epsilon)
    T -> FT'
    T' -> *FT' | /FT' | (epsilon)
    F -> NUM | (E)

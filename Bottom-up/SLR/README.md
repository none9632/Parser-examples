# SLR parser

## Build and run
    make
    ./SLR_parser <expression>
    
## Testing
    make test (or ./test.sh)

## Grammar
    E -> E+T | E-T | T
    T -> T*F | T/F | F
    F -> NUM | (E)

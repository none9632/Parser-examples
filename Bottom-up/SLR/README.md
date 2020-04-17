# SLR parser

This is an simple example of a SLR(1) parser.

## Build and run
    make
    ./SLR_parser <expression>
    
## Testing
    make test (or ./test.sh)

## Grammar
    E -> E + T      (0)
    E -> E - T      (1)
    E -> T          (2)
    T -> T * F      (3)
    T -> T / F      (4)
    T -> F          (5)
    F -> NUM        (6)
    F -> (E)        (7)
    
## Example
Here is an example parse of the string 1+2*3:

Parse stack | Value stack | Input | Parser action
------------|-------------|-------|--------------
S(0) | NULL | 1+2*3$ | Shift S(4) 
S(0)S(4) | 1 | +2*3$ | Reduce R(6) 
S(0)S(5) | 1 | +2*3$ | Reduce R(5)
S(0)S(2) | 1 | +2*3$ | Reduce R(2)
S(0)S(1) | 1 | +2*3$ | Shift S(6)
S(0)S(1)S(6) | 1 | 2*3$ | Shift S(4)
S(0)S(1)S(6)S(4) | 1 2 | *3$ | Reduce R(6)
S(0)S(1)S(6)S(5) | 1 2 | *3$ | Reduce R(5)
S(0)S(1)S(6)S(12) | 1 2 | *3$ | Shift S(8)
S(0)S(1)S(6)S(12)S(8) | 1 2 | $ | Shift S(4)
S(0)S(1)S(6)S(12)S(8)S(13) | 1 2 3 | $ | Reduce R(3)
S(0)S(1)S(6)S(11) | 1 6 | $ | Reduce R(0)
S(0)S(1) | 7 | $ | Accept

### Annotation

In order to understand what to do next, the parser uses `parse_table` and `produciton_table`.

#####Shift
* puts the state on `parse_stack` according to `parse_table`
* if the input `token.type` is a number then puts `token.value` on `value_stack`

#####Reduce
* removes states equal to the number of elements in the production from `parse_stack`
* puts the state on `parse_stack` according to `parse_table`
* if production is 0, 1, 3 or 4 then
    * removes two values from `value_stack`
    * performs an arithmetic operation according to the production
    * puts the received value back on `value_stack`

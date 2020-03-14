# Table-driven parser

This is an simple example of a table-driven LL(1) parser. 

## Build and run
    make
    ./LL_parser <expression>
    
## Testing
    make test (or ./test.sh)

## Grammar
    E -> TE'
    E' -> +TE' | (epsilon)
    T -> FT'
    T' -> *FT' | (epsilon)
    F -> NUM | (E)

## Example

Here is an example parse of the string `1+2*3`:

Parse stack | Value stack | Input | Parser action
------------|-------------|-------|--------------
E | NULL | 1+2*3$ | Predict E->TE'
E'T | NULL | 1+2*3$ | Predict T->FT'
E'T'F | NULL | 1+2*3$ | Predict F->NUM
E'T'NUM | NULL | 1+2*3$ | Match NUM
E'T' | 1 | +2*3$ | Predict T->e
E' | 1 | +2*3$ | Predict E'->+TE'
(PLUS_ACT)E'T+ | 1 | +2*3$ | Match +
(PLUS_ACT)E'T | 1 | 2*3$ | Predict T->FT'
(PLUS_ACT)E'T'F | 1 | 2*3$ | Predict F->NUM
(PLUS_ACT)E'T'NUM | 1 | 2*3$ | Match NUM
(PLUS_ACT)E'T' | 1 2 | *3$ | Predict T'->*FT'
(PLUS_ACT)E'(MULT_ACT)T'F* | 1 2 | *3$ | Match *
(PLUS_ACT)E'(MULT_ACT)T'F | 1 2 | 3$ | Predict F->NUM
(PLUS_ACT)E'(MULT_ACT)T'NUM | 1 2 | 3$ | Match NUM
(PLUS_ACT)E'(MULT_ACT)T' | 1 2 3 | $ | Predict T'->e
(PLUS_ACT)E'(MULT_ACT) | 1 2 3 | $ | Perform action MULT_ACT
(PLUS_ACT)E' | 1 6 | $ | Predict E'->e
| NULL | 7 | $ | Success, output top value stack

### Annotation

Perform action
* pop two values from `value_stack`
* push result of the perform action in `value_stack`
* pop top value from `parse_stack`

Predict
* getting index from `parser_table`
* if index is empty
    * error
* else
    * pop top value from `parse_stack`
    * push production from `production_table` in `parse_stack`

Match
* if top value from `parse_stack` matches the input value then
    * if top was `NUM` then push value in `value_stack`
    * pop top value from `parse_stack`
    * get next token
* else
    * error

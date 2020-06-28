# SLR(1), LR(1) and LALR(1) parsers

This is an simple examples of a SLR(1), LR(1) and LALR(1) parsers. The parsers data code
is very similar, the only difference is in `parse_table`. All three programs generate and
output an abstract syntax tree.

#### Annotation

`graph.drawio` - goto-graph(*maybe it's called something else*), opens with [drawio](https://app.diagrams.net)

## Build and run

    $ make
    $ ./SLR_parser(or ./LR_parser or ./LALR_parser) <expression>
    
## Testing

    $ make test

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

Example of how SLR(1) parses the string `1+2*3`:

Parse stack                | Input  | Parser action
---------------------------|--------|--------------
S(0)                       | 1+2*3$ | Shift S(4)
S(0)S(4)                   | +2*3$  | Reduce R(6)
S(0)S(5)                   | +2*3$  | Reduce R(5)
S(0)S(2)                   | +2*3$  | Reduce R(2)
S(0)S(1)                   | +2*3$  | Shift S(6)
S(0)S(1)S(6)               | 2*3$   | Shift S(4)
S(0)S(1)S(6)S(4)           | *3$    | Reduce R(6)
S(0)S(1)S(6)S(5)           | *3$    | Reduce R(5)
S(0)S(1)S(6)S(12)          | *3$    | Shift S(8)
S(0)S(1)S(6)S(12)S(8)      | $      | Shift S(4)
S(0)S(1)S(6)S(12)S(8)S(13) | $      | Reduce R(3)
S(0)S(1)S(6)S(11)          | $      | Reduce R(0)
S(0)S(1)                   | $      | Accept

#### Output

    E
    ├── E
    │   └── T
    │       └── F
    │           └── 1
    ├── +
    └── T
        ├── T
        │   └── F
        │       └── 2
        ├── *
        └── F
            └── 3

## Description

In order to understand what to do next, the parser uses `parse_table` and `produciton_table`.
SLR, LR and LALR parsers work on the same principle, the difference is only in `parse_table`
and that is why I did not give examples of how LR and LALR work.

#### Annotation
In parse_stack puts node, but I didn't portray it because I don't know how to do it.

#### Shift

Puts the state and `node` on `parse_stack` according to `parse_table`. If the input is 
a number then puts `token.value` in `node->value`.

#### Reduce

Removes states and nodes equal to the number of elements in the production from `parse_stack`
and puts the deleted nodes from `parse_stack` in new node. Puts the new state and new `node`
on `parse_stack` according to `parse_table`.
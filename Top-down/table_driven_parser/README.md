# Table-driven parser

This is an simple example of a table-driven LL(1) parser. The program generates and 
outputs an abstract syntax tree.

## Build and run

    $ make
    $ ./LL_parser <expression>
    
## Testing

    $ make test

## Grammar

    E  -> TE'
    E' -> +TE' | -TE' | (epsilon)
    T  -> FT'
    T' -> *FT' | /FT' | (epsilon)
    F  -> NUM | (E)

## Example

Here is an example parse of the string `1+2*3`:

Parse stack | Input  | Parser action
------------|--------|--------------
E           | 1+2*3$ | Predict E->TE'
E'T         | 1+2*3$ | Predict T->FT'
E'T'F       | 1+2*3$ | Predict F->NUM
E'T'NUM     | 1+2*3$ | Match NUM
E'T'        | +2*3$  | Predict T->e
E'          | +2*3$  | Predict E'->+TE'
E'T+        | +2*3$  | Match +
E'T         | 2*3$   | Predict T->FT'
E'T'F       | 2*3$   | Predict F->NUM
E'T'NUM     | 2*3$   | Match NUM
E'T'        | *3$    | Predict T'->*FT'
E'T'F*      | *3$    | Match *
E'T'F       | 3$     | Predict F->NUM
E'T'NUM     | 3$     | Match NUM
E'T'        | $      | Predict T'->e
E'          | $      | Predict E'->e
NULL        | $      | Success, output top value stack

#### Output

    E
    ├── T
    │   ├── F
    │   │   └── 1
    │   └── T'
    │       └── epsilon
    └── E'
        ├── +
        ├── T
        │   ├── F
        │   │   └── 2
        │   └── T'
        │       ├── *
        │       ├── F
        │       │   └── 3
        │       └── T'
        │           └── epsilon
        └── E'
            └── epsilon

## Description

#### Annotation
In parse_stack puts node, but I didn't portray it because I don't know how to do it.

#### Predict

Replaces the nonterminal character in `parse_table` with the production that is in
`production_table`.

#### Match

Checks whether the types in the input and in `parse_stack` match. If there is a match,
it deletes the upper element in `parse_stack`, if not, it is a syntax error.

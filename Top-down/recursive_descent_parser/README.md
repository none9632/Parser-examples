# Recursive-Descent Parser

This is an simple example of a recursive-descent parser. The program generates and
outputs an abstract syntax tree.

## Build and run
    $ make
    $ ./LL_parser <expression>

## Testing
    $ make test

## Grammar
    E -> T { (+|-) T }
    T -> F { (*|/) F }
    F -> NUM | (E)

## Example

Here is an example parse of the string `1+2*3`:

#### Output 

    +
    ├── 1
    └── *
        ├── 2
        └── 3
        
## Discription

Each production in the grammar corresponds to a function that generates the required
tree. Functions are called recursively until the end of the file is reached or until
an error is detected.

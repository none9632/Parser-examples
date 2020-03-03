#!/bin/bash

program_path=./cmake-build-debug/LL
red="\033[38;2;255;0;0m"
green="\033[38;2;0;255;0m"

function test
{
    output=$($program_path $1)
    if [ $output = $2 ]
    then
        echo -e " $1 => $output $green OK\e[0m"
    else
        echo -e " $1 => $output $red ERROR\e[0m"
    fi
}

test "0" "0"
test "1+1" "2"
test "1+(2*3)" "7"
test "2*3+4" "10"
test "2*3+4*5" "26"
test "50/10" "5"
test "3/2" "1"
test "6*3/2" "9"
test "(2+3)*(4+5)" "45"
test "1+2+3+4+5+6+7+8+9+10+11+12+13+14+15+16+17" "153"


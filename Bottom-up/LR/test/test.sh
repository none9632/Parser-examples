#!/bin/bash

program_path=../LR_parser
red="\033[38;2;255;0;0m"
green="\033[38;2;0;255;0m"
results=0

g_start=$(date +%s.%N)

function test
{
    start=$(date +%s.%N)
    output=$($program_path "$1")
    dif=$(echo "$(date +%s.%N) - $start" | bc)
    right_output=$(cat $2)

    printf " %.4fs |" $dif

    if [ "$output" = "$right_output" ]
    then
        echo -e " -$green OK \e[0m-       $1"
    else
        results=1
        echo -e " -$red ERROR \e[0m-    $1"
    fi
}

echo "--------------------------------------------------------------------"
echo " Start program testing"
echo "--------------------------------------------------------------------"

if [ -e $program_path ]
then
    test "0" output/test1
    test "1+2" output/test2
    test "1-(2*3)" output/test3
    test "2*3+4" output/test4
    test "2*3-4*5" output/test5
    test "50/10" output/test6
    test "6-3/2" output/test7
    test "(2+3)*(4-5)" output/test8
    test "1+2+3+4+5+6+7+8+9+10+11+12+13+14+15+16+17" output/test9
    test "12*(32)" output/test10
    test "12 * (   32   )" output/test11
    test "12(32)" output/test12
    test "5++" output/test13
    test "532/" output/test14
    test "()234" output/test15
    test "word" output/test16
else
    results=1
    echo ""
    echo "    $program_path: No such file"
    echo ""
fi

echo "--------------------------------------------------------------------"

g_dif=$(echo "$(date +%s.%N) - $g_start" | bc)
printf " %.4fs |" $g_dif

if [ $results -eq 1 ]
then
    echo -e " results:$red fail\e[0m"
else
    echo -e " results:$green success\e[0m"
fi

echo "--------------------------------------------------------------------"

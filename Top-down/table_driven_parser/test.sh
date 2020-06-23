#!/bin/bash

program_path=./LL_parser
red="\033[38;2;255;0;0m"
green="\033[38;2;0;255;0m"
results=0

g_start=$(date +%s.%N)

function test
{
    start=$(date +%s.%N)
    output=$($program_path "$1")
    dif=$(echo "$(date +%s.%N) - $start" | bc)

     printf " %.4fs |" $dif

    if [ "$output" = "$2" ]
    then
        echo -e " -$green OK \e[0m-       $1 => $output"
    else
        results=1
        echo -e " -$red ERROR \e[0m-    $1 => $output"
    fi
}

echo "--------------------------------------------------------------------"
echo " Start program testing"
echo "--------------------------------------------------------------------"

if [ -e $program_path ]
then
    test "0" "0"
    test "1+1" "2"
    test "1+(2*3)" "7"
    test "2*3+4" "10"
    test "2*3+4*5" "26"
    test "(2+3)*(4+5)" "45"
    test "1+2+3+4+5+6+7+8+9+10+11+12+13+14+15+16+17" "153"
    test "12*(32)" "384"
    test "12 * (   32   )" "384"
    test "12(32)" "error: syntax error"
    test "5++" "error: syntax error"
    test "()234" "error: syntax error"
    test "word" "error: unknown character"
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

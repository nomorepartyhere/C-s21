#!/bin/bash

array=(b e n s t v)

declare -a filenames=(
"1.txt"
"2.txt"
"3.txt"
"1.txt 2.txt"
"2.txt 3.txt"
"1.txt 2.txt 3.txt"
)

FAIL=0
SUCCESS=0

function test {
for f in "${filenames[@]}"
do
    echo "diff <(./s21_cat -$1 $f) <(cat -$1 $f)"
    diff <(./s21_cat -$1 $f) <(cat -$1 $f) > log.txt 2>&1
    error=$?
    if [ $error -eq 2 ] 
    then
        echo "There was something wrong with the diff command"
    elif [ $error -eq 1 ]
    then
        echo -e "\033[31mFAIL -$1 $f\033[0m"
        datetime=$(date +"%Y-%m-%d %H:%M")
        echo "$datetime FAIL -$1 $f" >> fail.txt
        (( FAIL++ ))
    else
        echo "SUCCESS -$1 $f"
        (( SUCCESS++ ))
    fi
done
}


echo "One flag"
for i in "${array[@]}"
do
test $i
done

rm -rf log.txt

echo "Total: $SUCCESS SUCCESS, $FAIL FAIL"
